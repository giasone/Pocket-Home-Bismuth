#include "DateTimePage.h"
#include "Config/MainKeys.h"
#include "Config/MainFile.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "DateTimePage";

/* Localized text value keys: */
static const juce::Identifier dateTimeTitleKey      = "dateTimeTitle";
static const juce::Identifier clockModeTextKey      = "clockMode";
static const juce::Identifier Mode24HourTextKey     = "mode24Hour";
static const juce::Identifier mode12HourTextKey     = "mode12Hour";
static const juce::Identifier hideClockTextKey      = "hideClock";
static const juce::Identifier setSystemClockTextKey = "setSystemClock";
static const juce::Identifier failedLaunchTextKey   = "failedLaunch";
static const juce::Identifier failedToRunTextKey    = "failedToRun";
static const juce::Identifier checkCommandTextKey   = "checkCommand";

DateTimePage::DateTimePage() :
Locale::TextUser(localeClassKey),
PageComponent("DateTimePage"),
titleLabel("dateTimeTitle", localeText(dateTimeTitleKey)),
setClockMode("setClockMode"),
reconfigureBtn(localeText(setSystemClockTextKey)),
clockModeLabel("modeLabel", localeText(clockModeTextKey))
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("DateTimePage");
#    endif
    setBackButton(PageComponent::leftBackButton);
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout(
    {
        Row(30, { RowItem(&titleLabel, 20) }),
        Row(20,{
                RowItem(&clockModeLabel, 30),
                RowItem(&setClockMode, 10)
            }),
        Row(50)
    });
    layout.setYMarginFraction(0.05);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(3);
    setLayout(layout);
    
    reconfigureBtn.addListener(this);
    titleLabel.setJustificationType(Justification::centred);

    setClockMode.addItem(localeText(Mode24HourTextKey), 1);
    setClockMode.addItem(localeText(mode12HourTextKey), 2);
    setClockMode.addItem(localeText(hideClockTextKey), 3);
    setClockMode.addListener(this);
    Config::MainFile mainConfig;
    if (mainConfig.getShowClock())
    {
        if (mainConfig.get24HourEnabled())
        {
            setClockMode.setSelectedId(1,
                    NotificationType::dontSendNotification);
        }
        else
        {
            setClockMode.setSelectedId(2,
                    NotificationType::dontSendNotification);
        }
    }
    else
    {
        setClockMode.setSelectedId(3, NotificationType::dontSendNotification);
    }
}

/**
 * Runs reconfigureCommand in the terminal to update system time when
 * the user presses the reconfigure button.
 */
void DateTimePage::pageButtonClicked(juce::Button* button)
{
    using juce::String;
    if (button == &reconfigureBtn)
    {
        Config::MainFile mainConfig;
        String configureTime = mainConfig.getTermLaunchPrefix()
                + reconfigureCommand;
        int ret = system(configureTime.toRawUTF8());
        if (ret != 0)
        {
            juce::AlertWindow::showMessageBox(
                    juce::AlertWindow::WarningIcon,
                    localeText(failedLaunchTextKey),
                    localeText(failedToRunTextKey)
                    + "\n" + configureTime + "\n"
                    + localeText(checkCommandTextKey));
        }
    }
}

/**
 * Changes the clock mode saved in the ComponentConfigFile when the
 * user selects a new mode with the setClockMode combo box.
 */
void DateTimePage::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox != &setClockMode)
    {
        DBG("DateTimePage::" << __func__ << ": responding to ComboBox "
                << comboBox->getName() << ", this should not happen!");
        return;
    }
    bool showClock = (comboBox->getSelectedId() != 3);
    bool use24HrMode = (comboBox->getSelectedId() == 1);
    Config::MainFile mainConfig;
    if (showClock)
    {
        mainConfig.set24HourEnabled(use24HrMode);
    }
    mainConfig.setShowClock(showClock);
}

