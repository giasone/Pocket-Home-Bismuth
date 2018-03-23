#include "Utils.h"
#include "PokeLookAndFeel.h"
#include "MainConfigFile.h"
#include "I2CBus.h"
#include "FelPage.h"

FelPage::FelPage(ComponentConfigFile& config) :
PageComponent(config,"FelPage",{
    {2,
        {
            {&infoLine1, 1}
        }},
    {2,
        {
            {nullptr, 1},
            {&yesButton, 2},
            {nullptr, 1}
        }},
    {1,
        {
            {nullptr, 1}
        }},
    {2,
        {
            {nullptr, 1},
            {&noButton, 2},
            {nullptr, 1}
        }},
    {1,
        {
            {nullptr, 1}
        }},
    {1,
        {
            {&infoLine2, 1}
        }}
},nullptr,false),
debounce(false),
infoLine1(config,"infoLine1", "Reboot into software flashing mode?"),
yesButton("Yes"),
noButton("No"),
infoLine2(config,"infoLine2", "For instructions, visit pcflash.getchip.com")
{

#    if JUCE_DEBUG
    setName("FelPage");
#    endif
    //TODO: configurable page backgrounds
    setColour(backgroundColourId, Colours::black);
    infoLine1.setJustificationType(Justification::centred);
    infoLine2.setJustificationType(Justification::centred);
    yesButton.addListener(this);
    noButton.addListener(this);
    addAndShowLayoutComponents();
}

/**
 * Handle button clicks, either restarting into Fel mode or closing the 
 * page.
 * @param should be either &yesButton or &noButton
 */
void
FelPage::pageButtonClicked(Button *button)
{
    if (button == &noButton)
    {
        removeFromStack(Animation::slideInFromLeft);
    }
    else if (button == &yesButton && !debounce)
    {
        debounce = true;
        try
        {
            I2CBus i2c;
            i2c.enableFelMode();
            ChildProcess c;
            MainConfigFile config;
            c.start(config.getConfigValue<String>
                    (MainConfigFile::restartCommandKey));
            c.waitForProcessToFinish(10000);
        }
        catch (I2CBus::I2CException e)
        {
            DBG("FelPage::" << __func__ << e.getErrorMessage());
        }
    }
}