/*!
 * \file BtGuiUtil.cpp
 * \date 2014/12/23 9:57
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtGuiUtil
*/

#include "stdafx.h"
#include "BtGuiUtil.h"

#include "BtGuiDef.h"

cocos2d::MenuItemLabel* BtGuiUtil::CreateMenuItem(const char* val, BtGuiListener* listener)
{
    auto label = cocos2d::LabelTTF::create(val, "Arial", BtGui_DefaultMenuFontSize);
    auto item = cocos2d::MenuItemLabel::create(label, CC_CALLBACK_1(BtGuiListener::OnMenuItem, listener));
    return item;
}

cocos2d::Menu* BtGuiUtil::CreateMenu(const char* itemTexts[], int itemCnt, BtGuiListener* listener)
{
    cocos2d::Vector<cocos2d::MenuItem*> items;
    for (int i = 0; i < itemCnt; ++i)
    {
        items.pushBack(BtGuiUtil::CreateMenuItem(itemTexts[i], listener));
    }

    auto menu = cocos2d::Menu::createWithArray(items);
    menu->alignItemsVertically();
    return menu;
}
