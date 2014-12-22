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

MenuItemLabel* CreateMenuItemLabel(const char* val, GameGuiListener* listener)
{
    auto label = LabelTTF::create(val, "Arial", DEFAULT_MENU_FONT_SIZE);
    auto item = MenuItemLabel::create(label, CC_CALLBACK_1(GameGuiListener::OnMenuItem, listener));
    return item;
}

bool GameGui::init(GameGuiListener* listener)
{
    const char* itemTexts[] = {
        MENU_Bubble,
        MENU_Town,
    }; 

    Vector<MenuItem*> items;
    for (auto it : itemTexts)
    {
        items.pushBack(CreateMenuItemLabel(it, listener));
    }

    m_menu = Menu::createWithArray(items);
    m_menu->alignItemsVertically();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    Vec2 menuPos;
    menuPos.x = origin.x + visibleSize.width / 2;
    menuPos.y = origin.y + visibleSize.height / 4;
    m_menu->setPosition(menuPos);

    return true;
}
