// Copyright 2022 ReadSpeaker AB. All Rights Reserved.

using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;

public class RSGameInterface : ModuleRules
{
    public RSGameInterface(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        PublicSystemIncludePaths.Add("$(ModuleDir)/Public");
    }
}
