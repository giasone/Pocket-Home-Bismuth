/**
 * @file CategoryPopupEditor.h
 * 
 * CategoryPopupEditor provides a user interface for editing a list of category
 * strings.
 */

#pragma once
#include "ListEditor.h"
#include "PopupEditorComponent.h"


class CategoryPopupEditor : public PopupEditorComponent{
public:
    /**
     * @param config      Component settings needed to setup the editor
     *                     component.
     * 
     * @param categories  An initial list of editable category strings to show.
     * 
     * @param onConfirm   This runs when the user presses the confirm button. 
     *                     The final list of edited categories will be passed 
     *                     to it as its only parameter.
     */
    CategoryPopupEditor(
            ComponentConfigFile& config,
            StringArray categories,
            std::function<void(StringArray) > onConfirm);
    
    virtual ~CategoryPopupEditor() { }

private:
    //Holds and allows updates to the list of categories 
    ListEditor categoryList;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CategoryPopupEditor)
};
