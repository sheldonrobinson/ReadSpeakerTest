// Copyright 2022 ReadSpeaker AB. All Rights Reserved.

using System.IO;
using System.Collections.Generic;
using UnrealBuildTool;
using System.Linq.Expressions;

public class RSBuilder : ModuleRules
{
    public RSBuilder(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
    }

    enum PlatformFlag
    {
        Unavailable = 0, Unused = 1, Used = 2
    }

    class ExportSetting
    {
        public string id;
        public PlatformFlag winx64flag;
        public PlatformFlag linuxx64flag;
        public PlatformFlag androidflag;
        public PlatformFlag ps4flag;
        public PlatformFlag ps5flag;
        public PlatformFlag xsxflag;
        public PlatformFlag nswitchflag;

        public bool UsedForPlatform(string platform)
        {
            if (platform == "winx64")
                return winx64flag == PlatformFlag.Used;
            else if (platform == "linuxx64")
                return linuxx64flag == PlatformFlag.Used;
            else if (platform == "android")
                return androidflag == PlatformFlag.Used;
            else if (platform == "ps4")
                return ps4flag == PlatformFlag.Used;
            else if (platform == "ps5")
                return ps5flag == PlatformFlag.Used;
            else if (platform == "xsx")
                return xsxflag == PlatformFlag.Used;
            else if (platform == "switch")
                return nswitchflag == PlatformFlag.Used;
            else
                return false;
        }
    }

    static string FirstLetterToUpper(string str)
    {
        if (str == null)
            return null;

        if (str.Length > 1)
            return char.ToUpper(str[0]) + str.Substring(1);

        return str.ToUpper();
    }

    public static void AddDirectoryToRuntimeDependencies(string path, RuntimeDependencyList runtimeDependencies, bool recursive, StagedFileType type)
    {
        foreach (string inFile in Directory.GetFiles(path))
        {   
            runtimeDependencies.Add(inFile, type);
        }
        if (recursive)
        {
            foreach (string dir in Directory.GetDirectories(path))
                AddDirectoryToRuntimeDependencies(dir, runtimeDependencies, recursive, type);
        }
    }

    public static void AddDirectoryToRuntimeDependenciesRootReplace(string path, RuntimeDependencyList runtimeDependencies, bool recursive, StagedFileType type, string inRoot, string outRoot)
    {
        foreach (string inFile in Directory.GetFiles(path))
        {
            string outFile = inFile.Replace(inRoot, outRoot);
            runtimeDependencies.Add(outFile, inFile, type);
        }
        if (recursive)
        {
            foreach (string dir in Directory.GetDirectories(path))
                AddDirectoryToRuntimeDependencies(dir, runtimeDependencies, recursive, type);
        }
    }

    public static List<string> GetUsedVoicesForPlatform(string voicePath, string settingsFile, string platform)
    {
        List<ExportSetting> exportSettings = ParseSetting(settingsFile);

        List<string> voicePaths = new List<string>();

        if (exportSettings == null)
            return null;

        foreach (string voiceDir in Directory.GetDirectories(voicePath))
        {
            foreach (string typeDir in Directory.GetDirectories(voiceDir))
            {
                string[] frags = typeDir.Split(Path.DirectorySeparatorChar);

                string id = FirstLetterToUpper(frags[frags.Length - 2]) + " " + FirstLetterToUpper(frags[frags.Length - 1]);

                foreach (ExportSetting setting in exportSettings)
                {
                    if (setting.id == id && setting.UsedForPlatform(platform))
                    {
                        voicePaths.Add(typeDir);
                    }
                }
            }
        }

        return voicePaths;
    }

    static List<ExportSetting> ParseSetting(string path)
    {
        if (!File.Exists(path))
            return null;

        string[] lines = File.ReadAllLines(path);
        List<ExportSetting> tmp = new List<ExportSetting>();

        foreach (string line in lines)
        {
            if (line.Contains("export"))
            {
                // Note
                // We don't use regex because of incompatibilities with UE5.
                string[] frags = line.Split('{');
                string exportData = frags[1].Substring(0, frags[1].Length - 1);
                string[] frags2 = exportData.Split(',');
                string[] frags3 = frags2[0].Split('=');
                string id = frags3[1].Trim();

                string[] frags4 = frags2[1].Split('=');
                string flags = frags4[1].Trim();

                ExportSetting setting = new ExportSetting();
                setting.id = id;

                for (int i = 0; i < flags.Length; i++)
                {
                    PlatformFlag flag = PlatformFlag.Unavailable;
                    char c = flags[i];
                    switch (c)
                    {
                        case '0':
                            flag = PlatformFlag.Unavailable;
                            break;
                        case '1':
                            flag = PlatformFlag.Unused;
                            break;
                        case '2':
                            flag = PlatformFlag.Used;
                            break;
                        default:
                            throw new System.Exception("Faulty settingsfile");
                    }
                    if (i == 0)
                    {
                        setting.winx64flag = flag;
                    }
                    else if (i == 1)
                    {
                        setting.linuxx64flag = flag;
                    }
                    else if (i == 2)
                    {
                        setting.androidflag = flag;
                    }
                    else if (i == 3)
                    {
                        setting.ps4flag = flag;
                    }
                    else if (i == 4)
                    {
                        setting.ps5flag = flag;
                    }
                    else if (i == 5)
                    {
                        setting.xsxflag = flag;
                    }else if (i == 6)
                    {
                        setting.nswitchflag = flag;
                    }
                }
                tmp.Add(setting);
            }
        }
        return tmp;
    }
}
