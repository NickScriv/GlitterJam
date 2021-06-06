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

#include "AkGroupValue.h"

#include "AkAudioDevice.h"

void UAkGroupValue::Load()
{
	Super::Load();

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		GetPathName(nullptr, packagePath);

		if (auto* audioDevice = FAkAudioDevice::Get())
		{
			audioDevice->OnLoadSwitchValue.Broadcast(packagePath);
		}

		if (GroupShortID == 0)
		{
			if (auto AudioDevice = FAkAudioDevice::Get())
			{
				FString GroupName;
				GetName().Split(TEXT("-"), &GroupName, nullptr);
				auto idFromName = AudioDevice->GetIDFromString(GroupName);
				GroupShortID = idFromName;
			}
		}
	}
}

void UAkGroupValue::BeginDestroy()
{
	Super::BeginDestroy();

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		if (auto* audioDevice = FAkAudioDevice::Get())
		{
			audioDevice->OnUnloadSwitchValue.Broadcast(packagePath);

			if (GEngine)
			{
				GEngine->ForceGarbageCollection();
			}
		}
	}
}
