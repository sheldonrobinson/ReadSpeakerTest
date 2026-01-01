// Copyright 2022 ReadSpeaker AB. All Rights Reserved.

using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;
using System.Linq.Expressions;

public class VTAPIWin64 : ModuleRules
{
    public VTAPIWin64(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;


        string libDirDirectory = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Source/ThirdParty/VTAPI/Platforms/Win64/Binaries\"))";
        string libDirectory = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Source/ThirdParty/VTAPI/Platforms/Win64/Binaries/libvtapi.dll\"))";
        string dbDirectory = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Resources/ReadSpeaker/WinLinux\"))";
        string licPath = "FPaths::Combine(*(IPluginManager::Get().FindPlugin(\"ReadSpeakerTTS\")->GetBaseDir()), TEXT(\"Resources/ReadSpeaker/WinLinux/verification.txt\"))";

        string sourceDbDirectory = Path.Combine(PluginDirectory, "Resources", "ReadSpeaker", "WinLinux").Replace('\\', '/');

        string libDirDefinition = "VTAPI_WIN64_LIBDIR="  + "TCHAR_TO_UTF8(*" + libDirDirectory + ")";
        string libDefinition = "VTAPI_WIN64_LIB=" + "TCHAR_TO_UTF8(*" + libDirectory + ")";
        string dbDefinition = "VTAPI_WIN64_DB=" + "TCHAR_TO_UTF8(*" + dbDirectory + ")";
        string licDefinition = "VTAPI_WIN64_LIC=" + "TCHAR_TO_UTF8(*" + licPath + ")";

        PublicDefinitions.Add(libDirDefinition);
        PublicDefinitions.Add(libDefinition);
        PublicDefinitions.Add(dbDefinition);
        PublicDefinitions.Add(licDefinition);

        PublicDefinitions.Add("VTAPI_WIN64");

        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Binaries", "libvtapi.lib"));

            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtapi.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/librsttswrapper.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtconv.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvteffect.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtplay.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtsave.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libvtssml.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp874.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp932.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp936.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp949.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp950.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1250.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1251.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1252.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1254.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1255.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1256.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1257.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp1258.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp28604.dll");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/libcp57002.dll");

            RuntimeDependencies.Add(Path.Combine(sourceDbDirectory, "vtpath.ini"));

            PublicDelayLoadDLLs.Add("libvtapi.dll");

            string settingsFile = Path.Combine(PluginDirectory, "Resources", "TTSSettings.ini");

            if (Target.Type != TargetType.Editor)
            {
                List<string> voicePaths = RSBuilder.GetUsedVoicesForPlatform(sourceDbDirectory, settingsFile, "winx64");

                foreach (string path in voicePaths)
                {
                    RSBuilder.AddDirectoryToRuntimeDependencies(path, RuntimeDependencies, true, StagedFileType.NonUFS);
                }
            }
        }
    }
}
