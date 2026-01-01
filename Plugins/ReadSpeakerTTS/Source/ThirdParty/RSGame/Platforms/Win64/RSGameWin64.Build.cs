// Copyright 2022 ReadSpeaker AB. All Rights Reserved.

using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;

public class RSGameWin64 : ModuleRules
{
    public RSGameWin64(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        string libDirDirectory = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Source/ThirdParty/RSGame/Platforms/Win64/Binaries\"))";
        string libDirectory = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Source/ThirdParty/RSGame/Platforms/Win64/Binaries/rsgame.dll\"))";

        string libDirDefinition = "RSGAME_WIN64_LIBDIR=" + "TCHAR_TO_UTF8(*" + libDirDirectory + ")";
        string libDefinition = "RSGAME_WIN64_LIB=" + "TCHAR_TO_UTF8(*" + libDirectory + ")";

        PublicDefinitions.Add(libDirDefinition);
        PublicDefinitions.Add(libDefinition);

        PublicDefinitions.Add("RSGAME_WIN64");

        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Binaries", "rsgame.lib"));

            RuntimeDependencies.Add("$(ModuleDir)/Binaries/rsgame.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/vcruntime140.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/msvcr120.dll");

			PublicDelayLoadDLLs.Add("rsgame.dll");
		}
    }
}
