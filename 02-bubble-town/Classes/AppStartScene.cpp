/*!
 * \file AppStartScene.cpp
 * \date 2015/01/21 9:56
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of AppStartScene
*/

#include "stdafx.h"
#include "AppStartScene.h"
#include "AppMacros.h"

#include "Core/BtGuiUtil.h"
#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"

#include "Services/BtLuaService.h"
#include "Services/BtGui_utouch.h"

const char* const MENU_Bubble  = "Bubble";
const char* const MENU_Town    = "Town";
const char* const MENU_World   = "World";

void BtQuitGame(cocos2d::Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    cocos2d::Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

// on "init" you need to initialize your instance
bool AppStartScene::init()
{
    if ( !Layer::init() )
        return false;
    
    auto root = cocos2d::Node::create();
    if (!root)
        return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto closeItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png", std::bind(&BtQuitGame, std::placeholders::_1));
    if (!closeItem)
        return false;
    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    if (!menu)
        return false;
    menu->setPosition(cocos2d::Vec2::ZERO);
    root->addChild(menu, 1);

    auto label = cocos2d::LabelTTF::create("Start", "Arial", TITLE_FONT_SIZE);
    label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height);
    root->addChild(label, 1);

    BtTextMenuBuilder mb;
    mb.AddItem(MENU_Bubble, std::bind([](cocos2d::Ref*) { BT_POST_LUA_AND_FLUSH(BtMsgID::StartNextLevel, ""); }, std::placeholders::_1));
    mb.AddItem(MENU_Town, std::bind([](cocos2d::Ref*) { BT_POST_LUA_AND_FLUSH(BtMsgID::GotoScene, BTSCN_town); }, std::placeholders::_1));
    auto menuMain = mb.Build();
    if (menuMain)
    {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        cocos2d::Vec2 menuPos;
        menuPos.x = origin.x + visibleSize.width / 2;
        menuPos.y = origin.y + visibleSize.height / 4;
        menuMain->setPosition(menuPos);
        root->addChild(menuMain, 1);
    }

    {
        auto node = BtGui_utouch::LoadLayout("ui/titlescreen");
        node->setAnchorPoint(cocos2d::Vec2::ZERO);
        node->setPosition(origin);
        root->addChild(node, 1);
    }

    addChild(root, 1);
    return true;
}
