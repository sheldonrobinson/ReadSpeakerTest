// Copyright 2022 ReadSpeaker AB. All Rights Reserved.

using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;

public class RSGameLinux : ModuleRules
{
    public RSGameLinux(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        string libDirDirectory = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Source/ThirdParty/RSGame/Platforms/Linux/Binaries\"))";
        string libDirectory = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Source/ThirdParty/RSGame/Platforms/Linux/Binaries/librsgame.so\"))";

        string libDirDefinition = "RSGAME_LINUX_LIBDIR=" + "TCHAR_TO_UTF8(*" + libDirDirectory + ")";
        string libDefinition = "RSGAME_LINUX_LIB=" + "TCHAR_TO_UTF8(*" + libDirectory + ")";

        PublicDefinitions.Add(libDirDefinition);
        PublicDefinitions.Add(libDefinition);

        PublicDefinitions.Add("RSGAME_LINUX");

        if (Target.Platform == UnrealTargetPlatform.Linux)
		{
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Binaries", "librsgame.a"));

            RuntimeDependencies.Add("$(ModuleDir)/Binaries/librsgame.so");

			PublicDelayLoadDLLs.Add("rsgame.so");
		}
    }
}
