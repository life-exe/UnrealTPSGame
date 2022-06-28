// My game copyright

#include "UI/PauseMenu/VideoSettingsWidget.h"
#include "Settings/TPSGameUserSettings.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "UI/PauseMenu/SettingOptionWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogVideoSettingsWidget, All, All);

void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto* UserSettings = UTPSGameUserSettings::Get();
    if (!UserSettings)
    {
        UE_LOG(LogVideoSettingsWidget, Error, TEXT("TPSGameUserSettings is nullptr"));
        return;
    }

    UserSettings->LoadSettings();
    const auto VideoSettings = UserSettings->GetVideoSettings();

    check(VideoSettingsContainer);
    VideoSettingsContainer->ClearChildren();

    for (auto* Setting : VideoSettings)
    {
        const auto SettingWidget = CreateWidget<USettingOptionWidget>(this, SettingOptionWidgetClass);
        check(SettingWidget);
        SettingWidget->Init(Setting);
        VideoSettingsContainer->AddChild(SettingWidget);
    }

    check(RunBenchmarkButton);
    RunBenchmarkButton->OnClicked.AddDynamic(this, &ThisClass::OnBenchmark);

    UserSettings->OnVideoSettingsUpdated.AddUObject(this, &ThisClass::OnVideoSettingsUpdated);
}

void UVideoSettingsWidget::OnBenchmark()
{
    if (auto* UserSettings = UTPSGameUserSettings::Get())
    {
        UserSettings->RunBenchmark();
    }
}

void UVideoSettingsWidget::OnVideoSettingsUpdated()
{
    if (!VideoSettingsContainer) return;
    for (auto* Widget : VideoSettingsContainer->GetAllChildren())
    {
        if (auto* SettingOptionWidget = Cast<USettingOptionWidget>(Widget))
        {
            SettingOptionWidget->UpdateTexts();
        }
    }
}
