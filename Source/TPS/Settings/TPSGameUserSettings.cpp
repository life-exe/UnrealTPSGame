// My game copyright

#include "Settings/TPSGameUserSettings.h"
#include "Settings/TPSGameSetting.h"

#define BIND_SETTINGS_FUNC(FUNC) \
    [&](int32 Level)             \
    {                            \
        FUNC(Level);             \
        ApplySettings(false);    \
    }

#define LOCTEXT_NAMESPACE "GameUserSettings"

UTPSGameUserSettings::UTPSGameUserSettings()
{
    const TArray<FSettingOption> VFXOptions =  //
        {
            {LOCTEXT("VFXQualityLow_Loc", "Low"), 0},        //
            {LOCTEXT("VFXQualityMedium_Loc", "Medium"), 1},  //
            {LOCTEXT("VFXQualityHigh_Loc", "High"), 2},      //
            {LOCTEXT("VFXQualityEpic_Loc", "Epic"), 3}       //
        };

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("AntiAliasing_Loc", "Anti-Aliasing"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetAntiAliasingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetAntiAliasingQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Textures_Loc", "Textures"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetTextureQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetTextureQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("GlobalIllumination_Loc", "Global Illumination"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetGlobalIlluminationQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetGlobalIlluminationQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Shadows_Loc", "Shadows"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetShadowQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetShadowQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("PostProcessing_Loc", "Post Processing"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetPostProcessingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetPostProcessingQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Reflections_Loc", "Reflections"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetReflectionQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetReflectionQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Effects_Loc", "Effects"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetVisualEffectQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetVisualEffectQuality));
        VideoSettings.Add(Setting);
    }
}

UTPSGameUserSettings* UTPSGameUserSettings::Get()
{
    return GEngine ? Cast<UTPSGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

const TArray<UTPSGameSetting*>& UTPSGameUserSettings::GetVideoSettings() const
{
    return VideoSettings;
}

void UTPSGameUserSettings::RunBenchmark()
{
    RunHardwareBenchmark();
    ApplySettings(false);
    OnVideoSettingsUpdated.Broadcast();
}

#undef LOCTEXT_NAMESPACE
