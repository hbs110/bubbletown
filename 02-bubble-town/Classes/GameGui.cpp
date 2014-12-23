/*!
 * \file GameGui.cpp
 * \date 2014/07/25 16:40
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of GameGui
*/

#include "stdafx.h"
#include "GameGui.h"
#include "GameGuiDef.h"

#include "Core/BtGuiUtil.h"

bool GameGui::init(BtGuiListener* listener)
{
    const char* itemTexts[] = {
        MENU_Bubble,
        MENU_Town,
    }; 

    cocos2d::Vector<cocos2d::MenuItem*> items;
    for (auto it : itemTexts)
    {
        items.pushBack(BtGuiUtil::CreateMenuItem(it, listener));
    }

    m_menu = cocos2d::Menu::createWithArray(items);
    m_menu->alignItemsVertically();

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    cocos2d::Vec2 menuPos;
    menuPos.x = origin.x + visibleSize.width / 2;
    menuPos.y = origin.y + visibleSize.height / 4;
    m_menu->setPosition(menuPos);

    return true;
}
