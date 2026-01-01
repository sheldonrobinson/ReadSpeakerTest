// Copyright 2022 ReadSpeaker AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Input/SCheckBox.h"
#include "ReadSpeakerTTS.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

#define LOCTEXT_NAMESPACE "SMyCombo"

enum PlatformFlag { Used = 2, Unused = 1, Unavailable = 0 };

class FTTSVoiceExportSettings {
public:
	FString id;
	PlatformFlag winx64;
	PlatformFlag linuxx64;
	PlatformFlag android;
	PlatformFlag ps4;
	PlatformFlag ps5;
	PlatformFlag xsx;
	PlatformFlag nswitch;

	FTTSVoiceExportSettings(FString id, PlatformFlag winx64flag, PlatformFlag linuxx64flag, PlatformFlag androidflag, PlatformFlag ps4flag, PlatformFlag ps5flag, PlatformFlag xsx, PlatformFlag nswitch);
};


class TTSSETTINGSWINDOW_API FTTSSettingsWindowModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	static FTTSSettingsWindowModule Load();
	static FReply ScanForVoiceEngines();
	static TArray<FTTSVoiceExportSettings> GetPreliminaryExportSettings();
	static void SetPreliminarySettingsFromVTPath(TArray<FTTSVoiceExportSettings> *PreliminarySettings, FString Path, FTTSVoiceExportSettings Template);

	void PluginButtonClicked();
	void SynchronizeExportSettings(TArray<FTTSVoiceExportSettings> exportSettings);
	void Save();

	bool verboseDebug;
	TArray<FTTSVoiceExportSettings> exportSettings;
	
private:

	void RegisterMenus();
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	void OnDebugStateChanged(ECheckBoxState InState);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};

class STTSDebugCheckBox : public SCheckBox {
public:
	SLATE_BEGIN_ARGS(STTSDebugCheckBox) : _TTSSettingsWindowModule()
	{
	}
	
	SLATE_ARGUMENT(FTTSSettingsWindowModule, TTSSettingsWindowModule);

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs) {
		TTSSettingsWindowModule = InArgs._TTSSettingsWindowModule;

		ECheckBoxState current;

		if (TTSSettingsWindowModule.verboseDebug) {
			current = ECheckBoxState::Checked;
		}
		else {
			current = ECheckBoxState::Unchecked;
		}

		SetIsChecked(current);

		OnCheckStateChanged.BindLambda([&](ECheckBoxState st) -> void {
			OnChange(st);
		});

		CreateAccessibleWidget();

	}

	void OnChange(ECheckBoxState st) {
		if (st == ECheckBoxState::Checked) {
			TTSSettingsWindowModule.verboseDebug = true;
		}
		else {
			TTSSettingsWindowModule.verboseDebug = false;
		}
		TTSSettingsWindowModule.Save();
	}

private:
	FTTSSettingsWindowModule TTSSettingsWindowModule;
};

class STTSExportGrid : public SGridPanel
{
public:
	SLATE_BEGIN_ARGS(STTSExportGrid) : _TTSSettingsWindowModule()
	{
	}
	SLATE_ARGUMENT(class TSharedPtr<FTTSSettingsWindowModule>, TTSSettingsWindowModule);

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		TTSSettingsWindowModule = InArgs._TTSSettingsWindowModule;
		TArray<FString> platformStrings = { "Windows x64", "Linux x64", "Android", "PS4", "PS5", "XSX", "Switch"};
		TArray<FTTSVoiceExportSettings> exportSettings = TTSSettingsWindowModule->exportSettings;
		
		int rows = exportSettings.Num() + 1;
		int columns = platformStrings.Num() + 1;

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (i == 0 && j == 0) {
					continue;
				}
				else if (i == 0) {
					AddSlot(j, i, Layer(0))[
						SNew(STextBlock).Text(FText::AsCultureInvariant(platformStrings[j - 1]))
					].HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(10, 0);
				}
				else if (j == 0) {
					AddSlot(j, i, Layer(0))[
						SNew(STextBlock).Text(FText::AsCultureInvariant(exportSettings[i - 1].id))
					].HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(0, 5);
				}
				else {
					FTTSVoiceExportSettings setting = exportSettings[i - 1];
					bool checked = false;
					bool enabled = false;
					if (j == 1) {
						checked = setting.winx64 == PlatformFlag::Used;
						enabled = setting.winx64 != PlatformFlag::Unavailable;
					}
					else if (j == 2) {
						checked = setting.linuxx64 == PlatformFlag::Used;
						enabled = setting.linuxx64 != PlatformFlag::Unavailable;
					}
					else if (j == 3) {
						checked = setting.android == PlatformFlag::Used;
						enabled = setting.android != PlatformFlag::Unavailable;
					}
					else if (j == 4) {
						checked = setting.ps4 == PlatformFlag::Used;
						enabled = setting.ps4 != PlatformFlag::Unavailable;
					}
					else if (j == 5) {
						checked = setting.ps5 == PlatformFlag::Used;
						enabled = setting.ps5 != PlatformFlag::Unavailable;
					}
					else if (j == 6) {
						checked = setting.xsx == PlatformFlag::Used;
						enabled = setting.xsx != PlatformFlag::Unavailable;
					}
					else if (j == 7) {
						checked = setting.nswitch == PlatformFlag::Used;
						enabled = setting.nswitch != PlatformFlag::Unavailable;
					}
					AddSlot(j, i, Layer(0))[
						SNew(SCheckBox).HAlign(HAlign_Center).IsChecked(checked).IsEnabled(enabled).OnCheckStateChanged_Lambda([&, i, j](ECheckBoxState st) -> void {
							FTTSVoiceExportSettings &exportSetting = TTSSettingsWindowModule->exportSettings[i - 1];
							PlatformFlag newFlag;
							if (st == ECheckBoxState::Checked) {
								newFlag = PlatformFlag::Used;
							}
							else if (st == ECheckBoxState::Unchecked) {
								newFlag = PlatformFlag::Unused;
							}
							else {
								newFlag = PlatformFlag::Unavailable;
							}
							if (j == 1) {
								exportSetting.winx64 = newFlag;
							}
							else if (j == 2) {
								exportSetting.linuxx64 = newFlag;
							}
							else if (j == 3) {
								exportSetting.android = newFlag;
							}
							else if (j == 4) {
								exportSetting.ps4 = newFlag;
							}
							else if (j == 5) {
								exportSetting.ps5 = newFlag;
							}
							else if (j == 6) {
								exportSetting.xsx = newFlag;
							}
							else if (j == 7) {
								exportSetting.nswitch = newFlag;
							}
							TTSSettingsWindowModule->Save();
						})
					].HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(15, 5);
				}
			}
		}
	}

private:
	TSharedPtr<FTTSSettingsWindowModule> TTSSettingsWindowModule;
};

#undef LOCTEXT_NAMESPACE