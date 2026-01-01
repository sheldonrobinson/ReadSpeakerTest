// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ReadSpeakerTest : ModuleRules
{
	public ReadSpeakerTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ReadSpeakerTTS", "OVRLipSync" });

        PrivateDependencyModuleNames.AddRange(new string[] { "ReadSpeakerTTS", "OVRLipSync" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
