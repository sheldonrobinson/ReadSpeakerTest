// Copyright 2022 ReadSpeaker AB. All Rights Reserved.

using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;
using System.Linq.Expressions;
using System.Linq;

public class VTAPIAndroid : ModuleRules
{
    public VTAPIAndroid(ReadOnlyTargetRules Target) : base(Target)
	{
        Type = ModuleType.External;

        string relativeTargetDbDirectory = "FString(FApp::GetName())" + " + " + "\"/\"" + " + " + "FString(FApp::GetName())" + " + " + "FString(\"/Plugins/ReadSpeakerTTS/Resources/ReadSpeaker/Android\")";
        string relativeLicDirectory = relativeTargetDbDirectory;

        string sourceDbDirectory = Path.Combine(PluginDirectory, "Resources", "ReadSpeaker", "Android").Replace('\\', '/');

        string dbDefinition = "VTAPI_ANDROID_DB(BasePath) " + "(TCHAR_TO_UTF8(*(BasePath + " + relativeTargetDbDirectory + ")));";
        string licDirDefinition = "VTAPI_ANDROID_LICDIR(BasePath) " + "(TCHAR_TO_UTF8(*(BasePath + " + relativeLicDirectory + ")));";
        string sourceDbDefinition = "VTAPI_ANDROID_SOURCEDB=" + "\"" + sourceDbDirectory + "\"";

        PublicDefinitions.Add(dbDefinition);
        PublicDefinitions.Add(licDirDefinition);
        PublicDefinitions.Add(sourceDbDefinition);

        PublicDefinitions.Add("VTAPI_ANDROID");

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/librsttswrapper.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libvtconv.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libvtjni.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libvtsave.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libvtssml.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libvtslicense.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp874.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp932.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp936.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp949.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp950.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp1250.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp1251.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp1252.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp1254.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp1255.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp1256.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp1257.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp1258.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp28604.so");
            RuntimeDependencies.Add("$(ModuleDir)/Binaries/arm64-v8a/libcp57002.so");

            PublicIncludePathModuleNames.AddRange(
                new string[]
                {
                    "Launch"
                }
            );

            PublicAddAdditionalLibrariesDirectory(Path.Combine(ModuleDirectory, "Binaries", "arm64-v8a"), false);
            PublicAddAdditionalLibrariesDirectory(Path.Combine(ModuleDirectory, "Binaries", "armeabi-v7a"), false);
            
            RuntimeDependencies.Add("$(PluginDir)/Resources/ReadSpeaker/Android/vtpath.ini");

            string settingsFile = Path.Combine(PluginDirectory, "Resources", "TTSSettings.ini");

            List<string> voicePaths = RSBuilder.GetUsedVoicesForPlatform(sourceDbDirectory, settingsFile, "android");

            foreach (string path in voicePaths)
            {
                RSBuilder.AddDirectoryToRuntimeDependencies(path, RuntimeDependencies, true, StagedFileType.NonUFS);
            }

            string generatedPath = GenerateUPL();
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", generatedPath);
        }
    }

    string GenerateUPL()
    {
        string generatedPath = Path.Combine(ModuleDirectory, "ReadSpeakerTTS_UPL_generated.xml");

        List<string> upl = File.ReadAllLines(Path.Combine(ModuleDirectory, "ReadSpeakerTTS_UPL.xml")).ToList();
        List<string> libAdditions = new List<string>();

        libAdditions.AddRange(GenerateLibAdditionsFromDirectory(Path.Combine(ModuleDirectory, "Binaries", "arm64-v8a")));
        libAdditions.AddRange(GenerateLibAdditionsFromDirectory(Path.Combine(ModuleDirectory, "Binaries", "armeabi-v7a")));

        libAdditions = libAdditions.Distinct().ToList();
        int insertionIndex = upl.FindIndex((x) => x.Contains("*VTLIBS*"));

        upl.InsertRange(insertionIndex, libAdditions);
        File.WriteAllLines(generatedPath, upl);

        return generatedPath;
    }

    List<string> GenerateLibAdditionsFromDirectory(string directory)
    {
        List<string> libAdditions = new List<string>();
        List<string> files = Directory.GetFiles(directory).ToList();
        foreach (string file in files)
        {
            if (file.Contains("libvt_"))
            {
                int k = file.IndexOf("libvt_");
                string tmp = file.Substring(k);
                string libName = tmp.Substring(3, tmp.Length - 6);
                string libAddition = "System.loadLibrary(\"" + libName + "\");";
                libAdditions.Add(libAddition);
            }
        }
        return libAdditions;
    }

    void PublicAddAdditionalLibrariesDirectory(string path, bool recursive)
    {
        foreach (string file in Directory.GetFiles(path))
        {
            if (file.EndsWith(".so"))
            {
                PublicAdditionalLibraries.Add(file);
            }
        }
        if (recursive)
        {
            foreach (string dir in Directory.GetDirectories(path))
            {
                PublicAddAdditionalLibrariesDirectory(dir, recursive);
            }
        }
    }
}
