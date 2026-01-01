// Copyright 2022 ReadSpeaker AB. All Rights Reserved.

using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;
using System.Linq.Expressions;

public class VTAPILinux : ModuleRules
{
    public VTAPILinux(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        string libDirDirectory = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Source/ThirdParty/VTAPI/Platforms/Linux/Binaries\"))";
        string libDirectory = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Source/ThirdParty/VTAPI/Platforms/Linux/Binaries/libvtapi.so\"))";
        string dbDirectory = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Resources/ReadSpeaker/WinLinux\"))";
        string licPath = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Resources/ReadSpeaker/WinLinux/verification.txt\"))";

        string sourceDbDirectory = Path.Combine(PluginDirectory, "Resources", "ReadSpeaker", "WinLinux").Replace('\\', '/');

        string libDirDefinition = "VTAPI_LINUX_LIBDIR=" + "TCHAR_TO_UTF8(*" + libDirDirectory + ")";
        string libDefinition = "VTAPI_LINUX_LIB=" + "TCHAR_TO_UTF8(*" + libDirectory + ")";
        string dbDefinition = "VTAPI_LINUX_DB=" + "TCHAR_TO_UTF8(*" + dbDirectory + ")";
        string licDefinition = "VTAPI_LINUX_LIC=" + "TCHAR_TO_UTF8(*" + licPath + ")";

        PublicDefinitions.Add(libDirDefinition);
        PublicDefinitions.Add(libDefinition);
        PublicDefinitions.Add(dbDefinition);
        PublicDefinitions.Add(licDefinition);

        PublicDefinitions.Add("VTAPI_LINUX");

        if (Target.Platform == UnrealTargetPlatform.Linux)
		{
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Binaries", "libvtapi.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Binaries", "libslicense.a"));

            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtapi.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtapi.so.4");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtapi.so.4.5");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/librsttswrapper.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtconv.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvteffect.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtjni.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtsave.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtssml.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp874.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp932.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp936.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp949.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp950.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1250.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1251.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1252.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1254.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1255.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1256.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1257.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1258.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp28604.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp57002.so");

            RuntimeDependencies.Add(Path.Combine(sourceDbDirectory, "vtpath.ini"));

            PublicDelayLoadDLLs.Add("libvtapi.so");

            string settingsFile = Path.Combine(PluginDirectory, "Resources", "TTSSettings.ini");

            if (Target.Type != TargetType.Editor)
            {
                List<string> voicePaths = RSBuilder.GetUsedVoicesForPlatform(sourceDbDirectory, settingsFile, "linuxx64");

                foreach (string path in voicePaths)
                {
                    RSBuilder.AddDirectoryToRuntimeDependencies(path, RuntimeDependencies, true, StagedFileType.NonUFS);
                }
            }
        }
    }
}
