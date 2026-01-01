#pragma once

#include "CoreMinimal.h"
#include "OVRLipSyncConstants.generated.h"

UENUM(BlueprintType)
enum class OVRLipSyncProviderKind : uint8
{
	Original = 0 UMETA(DisplayName = "Original"),
	Enhanced = 1 UMETA(DisplayName = "Enhanced"),
	EnhancedWithLaughter = 2 UMETA(DisplayName = "Enhanced with Laughter"),
};

