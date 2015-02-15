/*!
 * \file BtGuiUtil.h
 * \date 2014/12/23 9:57
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtGuiUtil 
*/

#pragma once

#include "BtCoreDef.h"

const int   BtGui_DefaultMenuFontSize   = 16;
BtConstStr  BtGui_DefaultMenuFont       = "Arial";

class BtTextMenuBuilder
{
public:
    enum eItemAlign
    {
        Left,
        Center,
        Right,
    };

    enum eMenuAlign
    {
        Horizontally,
        Vertically,
    };

    BtTextMenuBuilder();

    void AddItem(const std::string& itemText) { m_itemTexts.push_back(itemText); }
    void AddItem(const std::string& itemText, cocos2d::ccMenuCallback handler);
    void AddItems(BtConstStr items[], int itemCnt);

    void SetItemAlign(eItemAlign val) { m_itemAlignment = val; }
    void SetMenuAlign(eMenuAlign val) { m_menuAlignment = val; }
    void SetHandler(cocos2d::ccMenuCallback handler) { m_handler = handler; }
    void SetTextSize(int val) { m_textSize = val; }

    cocos2d::Menu* Build();

private:
    std::vector<std::string> m_itemTexts;
    eItemAlign m_itemAlignment;
    eMenuAlign m_menuAlignment;
    cocos2d::ccMenuCallback  m_handler;
    std::map<std::string, cocos2d::ccMenuCallback> m_namedHandlers;

    int m_textSize;
};


typedef std::function<void()> buttonHandler_t;

void BtSetButtonHandler(cocos2d::ui::Button* button, buttonHandler_t handler, bool onReleased = false);
