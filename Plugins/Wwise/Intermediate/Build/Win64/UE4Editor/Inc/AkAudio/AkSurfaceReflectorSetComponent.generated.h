// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef AKAUDIO_AkSurfaceReflectorSetComponent_generated_h
#error "AkSurfaceReflectorSetComponent.generated.h already included, missing '#pragma once' in AkSurfaceReflectorSetComponent.h"
#endif
#define AKAUDIO_AkSurfaceReflectorSetComponent_generated_h

#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_60_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FEdgeInfo_Statics; \
	AKAUDIO_API static class UScriptStruct* StaticStruct();


template<> AKAUDIO_API UScriptStruct* StaticStruct<struct FEdgeInfo>();

#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_33_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FAkPoly_Statics; \
	AKAUDIO_API static class UScriptStruct* StaticStruct(); \
	FORCEINLINE static uint32 __PPO__SurfaceArea() { return STRUCT_OFFSET(FAkPoly, SurfaceArea); }


template<> AKAUDIO_API UScriptStruct* StaticStruct<struct FAkPoly>();

#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_SPARSE_DATA
#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execUpdateSurfaceReflectorSet); \
	DECLARE_FUNCTION(execRemoveSurfaceReflectorSet); \
	DECLARE_FUNCTION(execSendSurfaceReflectorSet);


#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execUpdateSurfaceReflectorSet); \
	DECLARE_FUNCTION(execRemoveSurfaceReflectorSet); \
	DECLARE_FUNCTION(execSendSurfaceReflectorSet);


#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUAkSurfaceReflectorSetComponent(); \
	friend struct Z_Construct_UClass_UAkSurfaceReflectorSetComponent_Statics; \
public: \
	DECLARE_CLASS(UAkSurfaceReflectorSetComponent, UAkAcousticTextureSetComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/AkAudio"), NO_API) \
	DECLARE_SERIALIZER(UAkSurfaceReflectorSetComponent)


#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_INCLASS \
private: \
	static void StaticRegisterNativesUAkSurfaceReflectorSetComponent(); \
	friend struct Z_Construct_UClass_UAkSurfaceReflectorSetComponent_Statics; \
public: \
	DECLARE_CLASS(UAkSurfaceReflectorSetComponent, UAkAcousticTextureSetComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/AkAudio"), NO_API) \
	DECLARE_SERIALIZER(UAkSurfaceReflectorSetComponent)


#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UAkSurfaceReflectorSetComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UAkSurfaceReflectorSetComponent) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UAkSurfaceReflectorSetComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAkSurfaceReflectorSetComponent); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UAkSurfaceReflectorSetComponent(UAkSurfaceReflectorSetComponent&&); \
	NO_API UAkSurfaceReflectorSetComponent(const UAkSurfaceReflectorSetComponent&); \
public:


#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UAkSurfaceReflectorSetComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UAkSurfaceReflectorSetComponent(UAkSurfaceReflectorSetComponent&&); \
	NO_API UAkSurfaceReflectorSetComponent(const UAkSurfaceReflectorSetComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UAkSurfaceReflectorSetComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAkSurfaceReflectorSetComponent); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UAkSurfaceReflectorSetComponent)


#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_PRIVATE_PROPERTY_OFFSET
#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_82_PROLOG
#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_PRIVATE_PROPERTY_OFFSET \
	Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_SPARSE_DATA \
	Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_RPC_WRAPPERS \
	Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_INCLASS \
	Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_PRIVATE_PROPERTY_OFFSET \
	Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_SPARSE_DATA \
	Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_RPC_WRAPPERS_NO_PURE_DECLS \
	Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_INCLASS_NO_PURE_DECLS \
	Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h_85_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class AkSurfaceReflectorSetComponent."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> AKAUDIO_API UClass* StaticClass<class UAkSurfaceReflectorSetComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Glitter_Plugins_Wwise_Source_AkAudio_Classes_AkSurfaceReflectorSetComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
