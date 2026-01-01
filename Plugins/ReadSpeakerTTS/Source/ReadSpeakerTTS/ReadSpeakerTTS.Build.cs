// Copyright 2022 ReadSpeaker AB. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class ReadSpeakerTTS : ModuleRules
{
	public ReadSpeakerTTS(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "InputCore",
                "CoreUObject",
                "Engine",
				"AudioExtensions",
                "Projects",
                "SignalProcessing",
                "RSEnabledPlatforms",
                "RSGameInterface",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
            }
        );

        RuntimeDependencies.Add("$(PluginDir)/Resources/TTSSettings.ini");

        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "UnrealEd",
                    "PropertyEditor"
                }
            );
        }

        if (Target.Platform == UnrealTargetPlatform.Android)
        {

            PublicIncludePathModuleNames.AddRange(
                new string[]
                {
                    "Launch"
                }
            );
        }
    }
}
