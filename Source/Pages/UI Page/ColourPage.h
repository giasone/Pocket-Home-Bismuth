/**
 * @file ColorPage.h
 */
#pragma once
#include "ConfigFile.h"
#include "PageComponent.h"
#include "SwitchComponent.h"

class ColourPage : public PageComponent
{
public:
    ColourPage(ComponentConfigFile& config);
    virtual ~ColourPage() {}
private:
    void pageResized() override;

    class ColourListModel : public ListBoxModel, public ConfigFile::Listener
    {
    public:
        ColourListModel(ComponentConfigFile& config);
        virtual ~ColourListModel();
        int getNumRows() override;

        void listResized(ListBox& list);

        void paintListBoxItem(int rowNumber, Graphics& g, int width, int height,
                bool rowIsSelected) override;

        Component* refreshComponentForRow(int rowNumber, bool isRowSelected,
                Component* toUpdate) override;

        void selectedRowsChanged(int lastRowSelected) override;


    private:
        ComponentConfigFile& config;
        void configValueChanged(ConfigFile* config, String key) override;
        int textHeight;
        StringArray colourKeys;
        Array<Colour> colours;
    };

    ComponentConfigFile& config;

    ColourListModel listModel;
    ListBox colourList;
    SwitchComponent testSwitch;

};

