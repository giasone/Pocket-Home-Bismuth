#pragma once
#include "Config/DataKey.h"

/**
 * @file   ComponentDataKeys.h
 *
 * @brief  Provides the keys for all parameters in the component.json
 *         configuration file.
 */
namespace ComponentConfigKeys
{
    //######################### Text Size Keys #################################
    static const Config::DataKey smallTextKey
        ("small text", Config::DataKey::doubleType);
    static const Config::DataKey mediumTextKey
        ("medium text", Config::DataKey::doubleType);
    static const Config::DataKey largeTextKey
        ("large text", Config::DataKey::doubleType);

    //######################### UI Component Data ##############################
    //Defines all component types managed in the config file
    static const juce::Identifier scrollingAppMenuKey
            ("scrolling app menu");
    static const juce::Identifier pagedAppMenuKey
            ("paged app menu");
    static const juce::Identifier menuFrameKey
            ("menu frame");
    static const juce::Identifier batteryIconKey
            ("battery");
    static const juce::Identifier batteryPercentKey
            ("battery percent text");
    static const juce::Identifier clockLabelKey
            ("time");
    static const juce::Identifier wifiIconKey
            ("wifi");
    static const juce::Identifier powerButtonKey
            ("power button");
    static const juce::Identifier settingsButtonKey
            ("settings button");
    static const juce::Identifier popupMenuKey
            ("popup menu");
    static const juce::Identifier pageLeftKey
            ("left arrow button");
    static const juce::Identifier pageRightKey
            ("right arrow button");
    static const juce::Identifier pageUpKey
            ("up arrow button");
    static const juce::Identifier pageDownKey
            ("down arrow button");
    static const juce::Identifier settingsListBtnKey
            ("settings list button");
    static const juce::Identifier spinnerKey
            ("loading spinner");
    
    static const std::vector<Config::DataKey>& basicKeys =
    {
        smallTextKey,
        mediumTextKey,
        largeTextKey
    };
    
    static const juce::Array<juce::Identifier> componentKeys =
    {
        scrollingAppMenuKey,
        pagedAppMenuKey,
        menuFrameKey,
        batteryIconKey,
        batteryPercentKey,
        clockLabelKey,
        wifiIconKey,
        powerButtonKey,
        settingsButtonKey,
        popupMenuKey,
        pageLeftKey,
        pageRightKey,
        pageUpKey,
        pageDownKey,
        settingsListBtnKey,
        spinnerKey
    };
}
