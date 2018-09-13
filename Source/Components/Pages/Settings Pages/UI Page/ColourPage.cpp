#include "TempTimer.h"
#include "ColourConfigFile.h"
#include "ColourConfigKeys.h"
#include "ColourPage.h"

ColourPage::ColourPage() :
PageComponent("ColourPage"),
listModel(),
colourList("colourList", &listModel)
{
    using namespace juce;
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(40, 
        { 
            RowItem(&colourList, 60),
            RowItem(&colourPicker, 40)
        })
    });
    layout.setYMarginFraction(0.1);
    layout.setYPaddingWeight(3);
    layout.setXPaddingWeight(3);
    setBackButton(PageComponent::BackButtonType::leftBackButton);
    setLayout(layout);
    colourList.setOutlineThickness(2);
    colourPicker.setSelectionCallback([this](Colour newColour)
    {
        SparseSet<int> selected = colourList.getSelectedRows();
        if(!selected.isEmpty())
        {
            Identifier selectedKey = listModel.getRowText(selected[0]);
            ColourConfigFile config;
            config.setColour(selectedKey, newColour);
        }
        repaint();
    });
}

void ColourPage::pageResized()
{
    listModel.listResized(colourList);
    colourList.updateContent();
    colourList.repaint();
}

ColourPage::ColourListModel::ColourListModel() : 
colourKeys(ColourConfigKeys::getColourKeys())
{
    using namespace juce;
    ColourConfigFile config;
    for(const Identifier& key : colourKeys)
    {

    }
    config.addListener(this, config.getColourIds());
    DBG(__func__ << ": adding " << colourKeys.size() << " colors");
    for (const String& key : colourKeys)
    {
        colours.add(config.getColour(key));
    }
    DBG(__func__ << ": found " << colours.size() << " colors");
}

ColourPage::ColourListModel::~ColourListModel() { }

int ColourPage::ColourListModel::getNumRows()
{
    return colourKeys.size();
}

/*
 * Gets the text of a specific row item.
 */
juce::String ColourPage::ColourListModel::getRowText(int index) const
{
    using namespace juce;
    return colourKeys[index];
}

void ColourPage::ColourListModel::listResized(juce::ListBox& list)
{
    ComponentConfigFile config;
    textHeight 
            = config.getFontHeight(ComponentConfigFile::TextSize::smallText);
    list.setRowHeight(textHeight * 1.5);
    list.updateContent();
    list.repaint();
}

void ColourPage::ColourListModel::paintListBoxItem(
        int rowNumber, 
        juce::Graphics& g,
        int width,
        int height,
        bool rowIsSelected)
{
    using namespace juce;
    ColourConfigFile config;
    config.addListener(this, config.getColourIds());
    Rectangle<int> fillArea(0, 0, width, height);
    float checkSize = fillArea.getHeight() / 4;
    g.fillCheckerBoard(fillArea.toFloat(), checkSize, checkSize,
            config.getColour(ColourPicker::checkerboardLight),
            config.getColour(ColourPicker::checkerboardDark));
    g.setColour(colours[rowNumber]);
    g.fillRect(fillArea);
    fillArea.reduce(height / 5, height / 5);
    g.setColour((colours[rowNumber].getAlpha() < 0x88) ?
            config.getColour(ColourPicker::checkerboardLight).contrasting() 
            : colours[rowNumber].contrasting());
    if (rowIsSelected)
    {
        g.fillRoundedRectangle(fillArea.toFloat(), textHeight / 5);
        g.setColour((colours[rowNumber].getAlpha() < 0x88) ?     
            config.getColour(ColourPicker::checkerboardLight)
            : colours[rowNumber]);
    }
    g.setOpacity(1);
    g.setFont(Font(textHeight));
    g.drawText(colourKeys[rowNumber], fillArea, Justification::centred);
}

juce::Component* ColourPage::ColourListModel::refreshComponentForRow(
        int rowNumber,
        bool isRowSelected, 
        juce::Component* toUpdate)
{
    if (toUpdate != nullptr)
    {
        delete toUpdate;
    }
    return nullptr;
}

void ColourPage::ColourListModel::selectedRowsChanged(int lastRowSelected) { }

void ColourPage::ColourListModel::colourValueChanged
(int colourID, juce::String colourKey, juce::Colour newColour)
{
    ColourConfigFile config;
    int colourIndex = colourKeys.indexOf(colourKey);
    if (colourIndex >= 0)
    {
        colours.set(colourIndex, newColour);
        DBG("ColourPage::" << __func__ << ": Setting color " << colourKey 
                << " index " << colourIndex << " to " << newColour.toString());
    }
    else
    {
        DBG("ColourPage::" << __func__ << ": Unknown color key " <<colourKey);
    }
}
