// Copyright 2022 ReadSpeaker AB. All Rights Reserved.

using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;
using System.Linq.Expressions;

public class RSEnabledPlatforms : ModuleRules
{
    public RSEnabledPlatforms(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        foreach (string dir in Directory.GetDirectories(Path.Combine(PluginDirectory, "Source", "ThirdParty", "RSGame", "Platforms")))
        {
            string dirName = Path.GetFileNameWithoutExtension(dir);
            PublicDependencyModuleNames.Add("RSGame" + dirName);
        }

        foreach (string dir in Directory.GetDirectories(Path.Combine(PluginDirectory, "Source", "ThirdParty", "VTAPI", "Platforms")))
        {
            string dirName = Path.GetFileNameWithoutExtension(dir);
            PublicDependencyModuleNames.Add("VTAPI" + dirName);
        }
    }
}
