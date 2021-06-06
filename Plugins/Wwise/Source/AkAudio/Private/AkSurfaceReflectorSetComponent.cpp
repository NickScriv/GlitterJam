/*******************************************************************************
The content of the files in this repository include portions of the
AUDIOKINETIC Wwise Technology released in source code form as part of the SDK
package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use these files in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Copyright (c) 2021 Audiokinetic Inc.
*******************************************************************************/

#include "AkSurfaceReflectorSetComponent.h"
#include "AkSpatialAudioVolume.h"
#include "AkAudioDevice.h"
#include "AkRoomComponent.h"
#include "AkReverbDescriptor.h"
#include "AkComponentHelpers.h"
#include "Components/BrushComponent.h"
#include "Model.h"
#include "EditorSupportDelegates.h"
#include "Engine/Polys.h"

#include "GameFramework/Volume.h"
#include "AkSettings.h"
#include "EngineUtils.h"
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>

#if WITH_EDITOR
#include "Editor.h"
#include "AkAudioStyle.h"
#include "EditorModeTools.h"
#include "EditorModeManager.h"
#include "EditorModes.h"

#if UE_4_25_OR_LATER
#include "EditorGeometry.h"
#include "GeometryEdMode.h"
#else
#include "Editor/GeometryMode/Public/EditorGeometry.h"
#include "Editor/GeometryMode/Public/GeometryEdMode.h"
#endif

#endif

#include "UObject/Object.h"
#include "Engine/GameEngine.h"

#if WITH_EDITOR
FText FAkPoly::GetPolyText(bool includeOcclusion) const
{
	if (!EnableSurface)
		return FText();

	FString textureName = FString("None");

	if (Texture)
		textureName = Texture->GetName();

	if (!includeOcclusion)
		return FText::FromString(FString::Format(TEXT("{0}"), { textureName }));

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MaximumFractionalDigits = 2;
	NumberFormat.MinimumFractionalDigits = 1;
	FString transmissionLossValueString = FText::AsNumber(Occlusion, &NumberFormat).ToString();
	return FText::FromString(FString::Format(TEXT("{0}{1}{2}"), { textureName, LINE_TERMINATOR, transmissionLossValueString }));
}
#endif

UAkSurfaceReflectorSetComponent::UAkSurfaceReflectorSetComponent(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Property initialization
	bWantsOnUpdateTransform = true;

	bEnableSurfaceReflectors = 1;

	bEnableDiffraction = 1;

	bEnableDiffractionOnBoundaryEdges = 0;
	
#if WITH_EDITOR
	PropertyChangedHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.AddUObject(this, &UAkSurfaceReflectorSetComponent::OnPropertyChanged);
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	WasSelected = false;
#endif
}

void UAkSurfaceReflectorSetComponent::BeginDestroy()
{
	Super::BeginDestroy();
#if WITH_EDITOR
	FCoreUObjectDelegates::OnObjectPropertyChanged.Remove(PropertyChangedHandle);
#endif
}

void UAkSurfaceReflectorSetComponent::OnRegister()
{
	Super::OnRegister();
	InitializeParentBrush();
}

void UAkSurfaceReflectorSetComponent::InitializeParentBrush(bool fromTick /* = true */)
{
	AVolume* Parent = Cast<AVolume>(GetOwner());
	if (Parent)
	{
		ParentBrush = Parent->Brush;
#if WITH_EDITOR
		// For runtime viewports, delay the UpdatePolys call until post-registration in tick.
		// When editing geometry in the editor, the brush vertex data can sometimes be out of date at this point, causing the 
		// EdgeMap to be incorrect, which results in the visualizer drawing incorrect edges.
		SchedulePolysUpdate();
#endif
	}
	else
	{
		bEnableSurfaceReflectors = false;
		ParentBrush = nullptr;
		if (!fromTick)
		{
			FString actorString = FString("NONE");
			FString actorClass = FString("NONE");
			if (GetOwner() != nullptr)
			{
				actorString = GetOwner()->GetName();
				UClass* ownerClass = GetOwner()->GetClass();
				if (ownerClass != nullptr)
					actorClass = ownerClass->GetName();
			}
			UE_LOG(LogAkAudio, Error, TEXT("UAkSurfaceReflectorSetComponent::InitializeParentBrush : Error. Actor %s, of type %s, has an AkSurfaceReflectorSetComponent attached (%s)."), *actorString, *actorClass, *UObject::GetName());
			UE_LOG(LogAkAudio, Error, TEXT("UAkSurfaceReflectorSetComponent requires to be attached to an actor inheriting from AVolume."));
		}
	}
}

void UAkSurfaceReflectorSetComponent::OnUnregister()
{
#if WITH_EDITOR
	if (!HasAnyFlags(RF_Transient))
	{
		DestroyTextVisualizers();
	}
#endif
	Super::OnUnregister();
}

#if WITH_EDITOR

TSet<int> UAkSurfaceReflectorSetComponent::GetSelectedFaceIndices() const
{
	TSet<int> selectedFaceIndices;

	// Determine if we are in geometry edit mode.
	if (GLevelEditorModeTools().IsModeActive(FEditorModeID(TEXT("EM_Geometry"))))
	{
		// If we are in geometry mode, go through the list of geometry objects
		// and find our current brush and update its source data as it might have changed 
		// in RecomputePoly
		if (ABrush* ownerBrush = Cast<ABrush>(GetOwner()))
		{
			USceneComponent* parentComp = GetAttachParent();
			if (ParentBrush != nullptr)
			{
				FEdModeGeometry* GeomMode = (FEdModeGeometry*)GLevelEditorModeTools().GetActiveMode(FEditorModeID(TEXT("EM_Geometry")));
				FEdModeGeometry::TGeomObjectIterator GeomModeIt = GeomMode->GeomObjectItor();
				const float tolerance = 0.001f;
				const FTransform& attachTransform = parentComp->GetComponentTransform();
				for (; GeomModeIt; ++GeomModeIt)
				{
					FGeomObjectPtr Object = *GeomModeIt;
					if (Object->GetActualBrush() == ownerBrush)
					{
						// selectedGeometry is a list of selected geometry elements. They can be vertices, edges, or polys
						TArray<FGeomBase*> selectedGeometry = Object->SelectionOrder;
						for (FGeomBase* selection : selectedGeometry)
						{
							if (!selection->IsVertex())
							{
								// There is no way to distinguish an edge from a poly, and we are unable to downcast.
								// Check the normal and mid point against the normal and mid point of each face in our model.
								// If we find the corresponding face, add its index to the selectedFaceIndices list.
								for (int32 NodeIdx = 0; NodeIdx < ParentBrush->Nodes.Num() && NodeIdx < PolyMidPoints.Num(); ++NodeIdx)
								{
									FVector localPolyMidPoint = attachTransform.InverseTransformPosition(PolyMidPoints[NodeIdx]);
									if (FMath::IsNearlyEqual((selection->GetNormal() - ParentBrush->Nodes[NodeIdx].Plane).Size(), 0.0f, tolerance)
										&& FMath::IsNearlyEqual((selection->GetMid() - localPolyMidPoint).Size(), 0.0f, tolerance))
									{
										selectedFaceIndices.Add(NodeIdx);
										break;
									}
								}
							}
						}
						break;
					}
				}
			}
		}
	}

	return selectedFaceIndices;
}

bool UAkSurfaceReflectorSetComponent::TexturesDiffer() const
{
	if (AcousticPolys.Num() == 0)
		return false;

	UAkAcousticTexture* texture = AcousticPolys[0].Texture;
	for (const FAkPoly poly : AcousticPolys)
	{
		if (poly.Texture != texture)
			return true;
	}
	
	return false;
}

void UAkSurfaceReflectorSetComponent::OnPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent)
{
	bool changeAffectsThis = false;
	if (AActor* actor = Cast<AActor>(ObjectBeingModified))
	{
		if (actor == GetOwner())
		{
			changeAffectsThis = true;
		}
	}
	if (USceneComponent* component = Cast<USceneComponent>(ObjectBeingModified))
	{
		if (component->GetOwner() == GetOwner())
		{
			changeAffectsThis = true;
		}
	}
	if (changeAffectsThis)
	{
		// The start of a UI interaction will trigger a EPropertyChangeType::Interactive.
		// This will be followed by a EPropertyChangeType::ValueSet at the end of the interaction.
		if (PropertyChangedEvent.ChangeType == EPropertyChangeType::Interactive)
		{
			UserInteractionInProgress = true;
		}
		if (UserInteractionInProgress && PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet)
		{
			UserInteractionInProgress = false;
		}
	}
}

void UAkSurfaceReflectorSetComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	InitializeParentBrush();
		
	if (ParentBrush != nullptr)
	{
		UpdatePolys();
	}

	if (AssociatedRoom && !Cast<UAkRoomComponent>(AssociatedRoom->GetComponentByClass(UAkRoomComponent::StaticClass())))
	{
		UE_LOG(LogAkAudio, Warning, TEXT("%s: The Surface Reflector Set's Associated Room is not of type UAkRoomComponent."), *GetOwner()->GetName());
	}

	const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UAkSurfaceReflectorSetComponent, AcousticPolys))
	{
		RegisterAllTextureParamCallbacks();
		RecalculateHFDamping();
	}
}

void UAkSurfaceReflectorSetComponent::PostEditUndo()
{
	OnRefreshDetails.ExecuteIfBound(); 
	Super::PostEditUndo(); 
}

FText UAkSurfaceReflectorSetComponent::GetPolyText(int32 PolyIdx) const
{
	if (PolyIdx >= AcousticPolys.Num())
		return FText();

	return AcousticPolys[PolyIdx].GetPolyText(bEnableSurfaceReflectors);
}

void UAkSurfaceReflectorSetComponent::UpdateText(bool Visible)
{
	bool bReallyVisible = GetWorld() && GetWorld()->WorldType == EWorldType::Editor && Visible;
	if (GetOwner() != nullptr)
	{
		const AAkSpatialAudioVolume* SpatialAudioVolume = Cast<const AAkSpatialAudioVolume>(GetOwner());
		if (SpatialAudioVolume != nullptr && SpatialAudioVolume->FitToGeometry)
			bReallyVisible &= (!SpatialAudioVolume->IsDragging || SpatialAudioVolume->FitFailed);
	}
	for (int32 i = 0; i < TextVisualizers.Num(); i++)
	{
		if (TextVisualizers[i])
		{
			TextVisualizers[i]->SetText(GetPolyText(i));
			TextVisualizers[i]->SetVisibility(bReallyVisible);
		}
	}
	UpdateTextPositions();
}

void UAkSurfaceReflectorSetComponent::SurfacePropertiesChanged()
{
	UpdateText(GetOwner() && GetOwner()->IsSelected());
	RegisterAllTextureParamCallbacks();
	RecalculateHFDamping();
}

void UAkSurfaceReflectorSetComponent::DestroyTextVisualizers()
{
	for (int32 i = 0; i < TextVisualizers.Num(); i++)
	{
		if(TextVisualizers[i])
			TextVisualizers[i]->DestroyComponent();
	}

	TextVisualizers.Empty();
}

void UAkSurfaceReflectorSetComponent::SchedulePolysUpdate()
{
	if (GetWorld()->ShouldTick())
	{
		PolysNeedUpdate = true;
	}
	else
	{
		if (ParentBrush && ParentBrush->Nodes.Num() != AcousticPolys.Num())
		{
			UpdatePolys();
		}
	}
}

void UAkSurfaceReflectorSetComponent::UpdatePolys()
{
	if (!ParentBrush || HasAnyFlags(RF_Transient) || UserInteractionInProgress)
	{
		return;
	}

	int32 NumTextures = AcousticPolys.Num();
	int32 NumPolys = ParentBrush->Nodes.Num();
	const bool shouldRefreshDetails = NumTextures != NumPolys;
	if (NumPolys > NumTextures)
	{
		AcousticPolys.AddDefaulted(NumPolys - NumTextures);
	}
	else if(NumPolys < NumTextures)
	{
		AcousticPolys.RemoveAt(NumPolys, NumTextures - NumPolys);
	}

	FTransform ToWorld = GetOwner()->ActorToWorld();

	for (int32 NodeIdx = 0; NodeIdx < ParentBrush->Nodes.Num(); ++NodeIdx)
	{
		FAkPoly& poly = AcousticPolys[NodeIdx];
		if (ParentBrush->Nodes[NodeIdx].NumVertices > 2)
		{
			TArray<FVector> WorldVertices;

			int32 VertStartIndex = ParentBrush->Nodes[NodeIdx].iVertPool;

			FVector centroidPosition = FVector::ZeroVector;
			for (int32 VertIdx = 0; VertIdx < ParentBrush->Nodes[NodeIdx].NumVertices; ++VertIdx)
			{
				FVector vert = ToWorld.TransformPosition(ParentBrush->Points[ParentBrush->Verts[VertStartIndex + VertIdx].pVertex]);
				centroidPosition += vert;
				WorldVertices.Emplace(vert);
			}
			centroidPosition /= (float)ParentBrush->Nodes[NodeIdx].NumVertices;
			float area = 0.0f;
			for (int vIndex = 0; vIndex < WorldVertices.Num() - 1; ++vIndex)
			{
				area += FAkReverbDescriptor::TriangleArea(centroidPosition, WorldVertices[vIndex], WorldVertices[vIndex + 1]);
			}
			area += FAkReverbDescriptor::TriangleArea(centroidPosition, WorldVertices[WorldVertices.Num() - 1], WorldVertices[0]);
			poly.SetSurfaceArea(area);
		}
	}

	// Always recreate all text visualizers as indexes may have changed.
	DestroyTextVisualizers();
	UMaterialInterface* mat = Cast<UMaterialInterface>(FAkAudioStyle::GetAkForegroundTextMaterial());

	for (int32 i = 0; i < AcousticPolys.Num(); i++)
	{
		FString VizName = GetOwner()->GetName() + GetName() + TEXT("TextViz ") + FString::FromInt(i);
		if (AcousticPolys[i].EnableSurface)
		{
			int32 idx = TextVisualizers.Add(NewObject<UTextRenderComponent>(GetOuter(), *VizName));
			if (TextVisualizers[idx])
			{
				if (mat != nullptr)
					TextVisualizers[idx]->SetTextMaterial(mat);
				TextVisualizers[idx]->RegisterComponentWithWorld(GetWorld());
				TextVisualizers[idx]->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
				TextVisualizers[idx]->bIsEditorOnly = true;
				TextVisualizers[idx]->bSelectable = false;
				TextVisualizers[idx]->bAlwaysRenderAsText = true;
				TextVisualizers[idx]->SetHorizontalAlignment(EHTA_Center);
				TextVisualizers[idx]->SetVerticalAlignment(EVRTA_TextCenter);
			}
		}
		else
		{
			TextVisualizers.Add(nullptr);
		}
	}

	UpdateEdgeMap();
	UpdateText(GetOwner() && GetOwner()->IsSelected());
	RegisterAllTextureParamCallbacks();
	RecalculateHFDamping();
	PolysNeedUpdate = false;
	if (shouldRefreshDetails)
		OnRefreshDetails.ExecuteIfBound();
}

void UAkSurfaceReflectorSetComponent::UpdateEdgeMap()
{
	EdgeMap.Empty();
	PolyMidPoints.Empty();
	PolyWorldSpaceNormals.Empty();
	const AAkSpatialAudioVolume* SpatialAudioVolume = Cast<const AAkSpatialAudioVolume>(GetOwner());
	USceneComponent* parentComp = GetAttachParent();
	if (ParentBrush != nullptr && SpatialAudioVolume != nullptr && parentComp != nullptr)
	{
		const FTransform attachTransform = parentComp->GetComponentTransform();
		for (int32 NodeIdx = 0; NodeIdx < ParentBrush->Nodes.Num() && NodeIdx < AcousticPolys.Num(); ++NodeIdx)
		{
			FVector PolyMidPoint(0, 0, 0);
			FVector PolyWorldSpaceNormal(0, 0, 0);
			FEdgeInfo EdgeInfo;
			FPlane& Plane = ParentBrush->Nodes[NodeIdx].Plane;
			EdgeInfo.Normal = FVector(Plane.X, Plane.Y, Plane.Z);
			EdgeInfo.IsEnabled = AcousticPolys[NodeIdx].EnableSurface;
			int32 VertStartIndex = ParentBrush->Nodes[NodeIdx].iVertPool;


			// Look for non-parallel lines to use to calculate the normal in world space.
			// Non-uniform scaling of dimensions will skew the normals stored in the brush, so we need to recaluclate them here
			// taking scaling into account.
			bool worldSpaceNormalFound = false;
			FVert BurshVert0 = ParentBrush->Verts[VertStartIndex];
			FVert BrushVert1 = ParentBrush->Verts[VertStartIndex + 1];
			FVector v0 = attachTransform.TransformPosition(ParentBrush->Points[BurshVert0.pVertex]);
			FVector v1 = attachTransform.TransformPosition(ParentBrush->Points[BrushVert1.pVertex]);
			FVector previousEdge = v1 - v0;

			// Add edges to map for edges visualization
			int32 NumVertices = ParentBrush->Nodes[NodeIdx].NumVertices;
			for (int32 Idx0 = NumVertices - 1, Idx1 = 0;
				Idx1 < NumVertices;
				Idx0 = Idx1, Idx1++)
			{
				BurshVert0 = ParentBrush->Verts[VertStartIndex + Idx0];
				BrushVert1 = ParentBrush->Verts[VertStartIndex + Idx1];

				PolyMidPoint += attachTransform.TransformPosition(ParentBrush->Points[BrushVert1.pVertex]);

				EdgeInfo.V0 = attachTransform.TransformPosition(ParentBrush->Points[BurshVert0.pVertex]);
				EdgeInfo.V1 = attachTransform.TransformPosition(ParentBrush->Points[BrushVert1.pVertex]);

				if (!worldSpaceNormalFound)
				{
					FVector edge = EdgeInfo.V1 - EdgeInfo.V0;
					// If the current edge is not parallel to the previous, use them to calculate the normal (taking scaling into account).
					if (FVector::DotProduct(edge, previousEdge) < 0.9f)
					{
						PolyWorldSpaceNormal = FVector::CrossProduct(edge, previousEdge);
						PolyWorldSpaceNormal.Normalize();
						PolyWorldSpaceNormals.Add(PolyWorldSpaceNormal);
						worldSpaceNormalFound = true;
					}
					previousEdge = edge;
				}

				int64 EdgeHash = EdgeInfo.GetHash();

				FEdgeInfo* Found = EdgeMap.Find(EdgeHash);
				if (Found)
				{
					if (FVector::DotProduct(Found->Normal, EdgeInfo.Normal) > 0.99f)
						Found->IsFlat = true;
					else
						Found->IsBoundary = !(Found->IsEnabled && EdgeInfo.IsEnabled);

					Found->IsEnabled |= EdgeInfo.IsEnabled;
				}
				else
				{
					EdgeMap.Add(EdgeHash, EdgeInfo);
				}
			}
			ensure(worldSpaceNormalFound);
			PolyMidPoint /= (float)ParentBrush->Nodes[NodeIdx].NumVertices;
			PolyMidPoints.Add(PolyMidPoint);
		}
	}
}

void UAkSurfaceReflectorSetComponent::AlignTextWithCenter(int faceIndex)
{
	if (faceIndex < TextVisualizers.Num()
		&& faceIndex < PolyMidPoints.Num()
		&& faceIndex < PolyWorldSpaceNormals.Num())
	{
		UTextRenderComponent* TextComp = TextVisualizers[faceIndex];
		if (ParentBrush != nullptr && TextComp != nullptr)
		{
			TextComp->SetWorldScale3D(FVector::OneVector);
			TextComp->SetWorldLocation(PolyMidPoints[faceIndex]);
			FRotator lookAt = FLookAtMatrix(PolyMidPoints[faceIndex], PolyMidPoints[faceIndex] + PolyWorldSpaceNormals[faceIndex], FVector::UpVector).Rotator();
			TextComp->SetWorldLocationAndRotationNoPhysics(PolyMidPoints[faceIndex], lookAt);
		}
	}
}

void UAkSurfaceReflectorSetComponent::UpdateTextPositions()
{
	const AAkSpatialAudioVolume* SpatialAudioVolume = Cast<const AAkSpatialAudioVolume>(GetOwner());
	if (ParentBrush != nullptr && SpatialAudioVolume != nullptr)
	{
		// For each text visualizer, find an appropriate edge on the face and align that visualizer with the edge.
		for (int32 NodeIdx = 0; NodeIdx < ParentBrush->Nodes.Num() && NodeIdx < TextVisualizers.Num(); ++NodeIdx)
		{
			UTextRenderComponent* TextComp = TextVisualizers[NodeIdx];
			if (TextComp != nullptr)
			{
				AlignTextWithCenter(NodeIdx);
			}
		}
	}
}

void UAkSurfaceReflectorSetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!ParentBrush)
	{
		InitializeParentBrush(true);
	}

	if (PolysNeedUpdate || (ParentBrush && ParentBrush->Nodes.Num() != AcousticPolys.Num()))
	{
		UpdatePolys();
	}

	if (GetOwner()->IsSelected() && !WasSelected)
	{
		WasSelected = true;
		UpdateText(true);
	}

	if (!GetOwner()->IsSelected() && WasSelected)
	{
		WasSelected = false;
		UpdateText(false);
	}
}
#endif // WITH_EDITOR

bool UAkSurfaceReflectorSetComponent::MoveComponentImpl(
	const FVector& Delta,
	const FQuat& NewRotation,
	bool bSweep,
	FHitResult* Hit,
	EMoveComponentFlags MoveFlags,
	ETeleportType Teleport)
{
	if (USceneComponent* Parent = GetAttachParent())
		if (AkComponentHelpers::DoesMovementRecenterChild(this, Parent, Delta))
			Super::MoveComponentImpl(Delta, NewRotation, bSweep, Hit, MoveFlags, Teleport);

	return false;
}

void UAkSurfaceReflectorSetComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	UpdateSurfaceReflectorSet();
#if WITH_EDITOR
	UpdateEdgeMap();
	UpdateText(GetOwner() && GetOwner()->IsSelected());
#endif
}

void UAkSurfaceReflectorSetComponent::GetTexturesAndSurfaceAreas(TArray<FAkAcousticTextureParams>& textures, TArray<float>& surfaceAreas) const
{
	const UAkSettings* AkSettings = GetDefault<UAkSettings>();
	if (AkSettings != nullptr)
	{
		if (AcousticPolys.Num() > 0)
		{
			for (const FAkPoly& poly : AcousticPolys)
			{
				if (poly.Texture)
				{
					surfaceAreas.Add(poly.GetSurfaceArea() / AkComponentHelpers::UnrealUnitsPerSquaredMeter(this));
					const FAkAcousticTextureParams* params = AkSettings->GetTextureParams(poly.Texture->ShortID);
					if (params != nullptr)
					{
						textures.Add(*params);
					}
					else
					{
						textures.Add(FAkAcousticTextureParams());
					}
				}
			}
		}
	}
}

bool UAkSurfaceReflectorSetComponent::ShouldSendGeometry() const
{
	if (ParentBrush == nullptr)
		return false;

	UAkRoomComponent* siblingRoom = AkComponentHelpers::GetChildComponentOfType<UAkRoomComponent>(*GetAttachParent());
	if (!bEnableSurfaceReflectors && !(siblingRoom && siblingRoom->bEnable))
		return false;

	return UAkAcousticTextureSetComponent::ShouldSendGeometry();
}

void UAkSurfaceReflectorSetComponent::BeginPlay()
{
	Super::BeginPlay();

	SendSurfaceReflectorSet();
}

void UAkSurfaceReflectorSetComponent::SendSurfaceReflectorSet() 
{
	if (GetWorld() && GetWorld()->bIsTearingDown)
		return;

	FAkAudioDevice* AkAudioDevice = FAkAudioDevice::Get();

	if (AkAudioDevice && ShouldSendGeometry())
	{
		TArray<AkVertex> VertsToSend;
		TArray<AkAcousticSurface> SurfacesToSend;
		TArray<AkTriangle> TrianglesToSend;
		TArray<AK::SpatialAudio::String> triangleNames;
		FString ParentName = GetOwner()->GetName();
		// Some clarifications: 
		// - All of the brush's vertices are held in the UModel->Verts array (elements of type FVert)
		// - FVert contains pVertex, which points to the UModel->Points array (actual coords of the point in actor space)
		// - Polygons are represented by the UModel->Nodes array (elements of type FBspNode).
		// - FBspNode contains iVertPool, which represents the index in the UModel->Verts at which the node's verts start
		// - FBspNode contains NumVertices, the number of vertices that compose this node.
		//
		// For more insight on how all of these tie together, look at UModel::BuildVertexBuffers().

		FTransform OwnerToWorld = GetOwner()->ActorToWorld();

		for (int32 pt = 0; pt < ParentBrush->Points.Num(); ++pt)
		{
			const FVector& VertexInActorSpace = ParentBrush->Points[pt];
			FVector v = OwnerToWorld.TransformPosition(VertexInActorSpace);
			AkVertex akvtx;
			akvtx.X = v.X;
			akvtx.Y = v.Y;
			akvtx.Z = v.Z;
			VertsToSend.Add(akvtx);
		}

		
		for (int32 NodeIdx = 0; NodeIdx < ParentBrush->Nodes.Num(); ++NodeIdx)
		{
			if (AcousticPolys.Num() > NodeIdx)
			{
				if (ParentBrush->Nodes[NodeIdx].NumVertices > 2 && (AcousticPolys[NodeIdx].EnableSurface || !bEnableSurfaceReflectors))
				{
					AkAcousticSurface NewSurface;
					FString TriangleName;
					if (AcousticPolys[NodeIdx].Texture != nullptr)
					{
						TriangleName = ParentName + GetName() + FString(TEXT("_")) + AcousticPolys[NodeIdx].Texture->GetName() + FString::FromInt(NodeIdx);
					}
					else
					{
						TriangleName = ParentName + GetName() + FString(TEXT("_")) + FString::FromInt(NodeIdx);
					}

					NewSurface.textureID = AcousticPolys[NodeIdx].Texture != nullptr ? FAkAudioDevice::Get()->GetIDFromString(AcousticPolys[NodeIdx].Texture->GetName()) : 0;
					NewSurface.transmissionLoss = AcousticPolys[NodeIdx].Occlusion;
					int32 newIdx = triangleNames.Add(TCHAR_TO_ANSI(*TriangleName));
					triangleNames[newIdx].AllocCopy(); //the conversion macro TCHAR_TO_ANSI will reuse the same buffer, so we need a local copy.
					NewSurface.strName = triangleNames[newIdx].Get();
					SurfacesToSend.Add(NewSurface);

					//

					int32 VertStartIndex = ParentBrush->Nodes[NodeIdx].iVertPool;

					const FVert* Vert0 = &ParentBrush->Verts[VertStartIndex + 0];
					const FVert* Vert1 = &ParentBrush->Verts[VertStartIndex + 1];

					for (int32 VertexIdx = 2; VertexIdx < ParentBrush->Nodes[NodeIdx].NumVertices; ++VertexIdx)
					{
						const FVert* Vert2 = &ParentBrush->Verts[VertStartIndex + VertexIdx];

						AkTriangle NewTriangle;
						NewTriangle.point0 = (AkVertIdx)Vert0->pVertex;
						NewTriangle.point1 = (AkVertIdx)Vert1->pVertex;
						NewTriangle.point2 = (AkVertIdx)Vert2->pVertex;
						NewTriangle.surface = (AkSurfIdx)(SurfacesToSend.Num()-1);
						TrianglesToSend.Add(NewTriangle);

						Vert1 = Vert2;
					}
				}
			}
		}

		if (TrianglesToSend.Num() > 0 && VertsToSend.Num() > 0)
		{
			AkGeometryParams params;
			params.NumSurfaces = SurfacesToSend.Num();
			params.NumTriangles = TrianglesToSend.Num();
			params.NumVertices = VertsToSend.Num();
			params.Surfaces = SurfacesToSend.GetData();
			params.Triangles = TrianglesToSend.GetData();
			params.Vertices = VertsToSend.GetData();
			params.EnableDiffraction = bEnableDiffraction;
			params.EnableDiffractionOnBoundaryEdges = bEnableDiffractionOnBoundaryEdges;
			params.EnableTriangles = bEnableSurfaceReflectors;

			if (AssociatedRoom)
			{
				UAkRoomComponent* room = Cast<UAkRoomComponent>(AssociatedRoom->GetComponentByClass(UAkRoomComponent::StaticClass()));

				if (room != nullptr)
					params.RoomID = room->GetRoomID();
			}

			SendGeometryToWwise(params);
		}
	}
}

void UAkSurfaceReflectorSetComponent::RemoveSurfaceReflectorSet()
{
	RemoveGeometryFromWwise();
}

void UAkSurfaceReflectorSetComponent::UpdateSurfaceReflectorSet()
{
	SendSurfaceReflectorSet();
	if (ReverbDescriptor != nullptr)
	{
		DampingEstimationNeedsUpdate = true;
	}
}

void UAkSurfaceReflectorSetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
		RemoveSurfaceReflectorSet();
}

#if WITH_EDITOR
bool UAkSurfaceReflectorSetComponent::ContainsTexture(const FGuid& textureID)
{
	for (const auto& poly : AcousticPolys)
		if (poly.Texture != nullptr && poly.Texture->ID == textureID)
			return true;
	return false;
}

void UAkSurfaceReflectorSetComponent::RegisterAllTextureParamCallbacks()
{
	for (const auto& poly : AcousticPolys)
		if (poly.Texture != nullptr && TextureDelegateHandles.Find(poly.Texture->ID) == nullptr)
			RegisterTextureParamChangeCallback(poly.Texture->ID);
}

TWeakObjectPtr<UPhysicalMaterial> AssignPolygonTexturesFromSamples(const TArray<FVector>& Vertices, const TArray<FVector>& Points, const TArray<FVector>& Normals, const TArray< TWeakObjectPtr<UPhysicalMaterial> >& Materials, int Num)
{
	const float kNormalAgreement = 0.866f; // ~30 degrees

	TMap<TWeakObjectPtr<UPhysicalMaterial>, int> Votes;

	const FVector* Vert0 = &Vertices[0];
	const FVector* Vert1 = &Vertices[1];

	for (int32 VertexIdx = 2; VertexIdx < Vertices.Num(); ++VertexIdx)
	{
		const FVector* Vert2 = &Vertices[VertexIdx];

		FVector e0 = *Vert1 - *Vert0;
		FVector e1 = *Vert2 - *Vert0;

		float d00 = FVector::DotProduct(e0, e0);
		float d01 = FVector::DotProduct(e0, e1);
		float d10 = FVector::DotProduct(e1, e0);
		float d11 = FVector::DotProduct(e1, e1);
		float denom = d00 * d11 - d01 * d01;

		// n defined such that the normal faces inwards.
		FVector n = FVector::CrossProduct(e1, e0);
		n.Normalize();

		for (int i = 0; i < Num; ++i)
		{
			const FVector& pt = Points[i];
			const FVector& norm = Normals[i];

			// We want some amount of agreement between the hit normal and the triangle normal.
			if (FVector::DotProduct(n, norm) < kNormalAgreement)
				continue;

			// Init tally to 0 if the normal points the right way.
			int Tally = 0;

			//project point on to triangle.
			float proj = FVector::DotProduct(n, pt - *Vert0);

			FVector pt_proj = pt - proj * n;
			FVector vToPt = pt_proj - *Vert0;

			float d20 = FVector::DotProduct(vToPt, e0);
			float d21 = FVector::DotProduct(vToPt, e1);

			// convert to barycentric coords to see if the point projects into the triangle.
			float u = (d00 * d21 - d01 * d20) / denom;
			if (u > 0.f && u < 1.f)
			{
				float v = (d11 * d20 - d01 * d21) / denom;
				if (v > 0.f && v < 1.f)
				{
					if (u + v < 1.f)
					{
						// Assign another point to the surface if the point projects into the triangle
						Tally++;
					}
				}
			}

			int* Count = Votes.Find(Materials[i]);
			if (Count == nullptr)
			{
				Count = &Votes.Add(Materials[i]);
				*Count = 0;
			}

			if (Count != nullptr)
			{
				*Count += Tally;
			}
		}

		Vert1 = Vert2;
	}

	// Tally the votes
	if (Votes.Num() > 0)
	{
		auto MaxVotes = *Votes.begin();
		auto it = Votes.begin();
		++it;

		while (it != Votes.end())
		{
			if (it->Value > MaxVotes.Value)
				MaxVotes = *it;

			++it;
		}

		// Return the material with the max number of points.
		return MaxVotes.Key;
	}

	return nullptr;
}

void UAkSurfaceReflectorSetComponent::AssignAcousticTexturesFromSamples(const TArray<FVector>& Points, const TArray<FVector>& Normals, const TArray< TWeakObjectPtr<class UPhysicalMaterial> >& Materials, int Num)
{
	check(Points.Num() == Materials.Num());

	AcousticPolys.Empty();

	FTransform ToWorld = GetOwner()->ActorToWorld();

	for (int32 NodeIdx = 0; NodeIdx < ParentBrush->Nodes.Num(); ++NodeIdx)
	{
		FAkPoly poly;
		poly.EnableSurface = false;

		if (ParentBrush->Nodes[NodeIdx].NumVertices > 2)
		{
			TArray<FVector> WorldVertices;

			int32 VertStartIndex = ParentBrush->Nodes[NodeIdx].iVertPool;

			for (int32 VertIdx = 0; VertIdx < ParentBrush->Nodes[NodeIdx].NumVertices; ++VertIdx)
			{
				WorldVertices.Emplace(ToWorld.TransformPosition(ParentBrush->Points[ParentBrush->Verts[VertStartIndex + VertIdx].pVertex]));
			}

			TWeakObjectPtr<UPhysicalMaterial> Material = AssignPolygonTexturesFromSamples(WorldVertices, Points, Normals, Materials, Num);

			if (Material.IsValid())
			{
				GetDefault<UAkSettings>()->GetAssociatedAcousticTexture(Material.Get(), poly.Texture);
				GetDefault<UAkSettings>()->GetAssociatedOcclusionValue(Material.Get(), poly.Occlusion);
				poly.EnableSurface = true;
			}
		}

		AcousticPolys.Add(poly);
		if (poly.Texture != nullptr)
			RegisterTextureParamChangeCallback(poly.Texture->ID);
	}

	OnRefreshDetails.ExecuteIfBound();
	// Update text visualizers.
	SchedulePolysUpdate();
}

#endif
