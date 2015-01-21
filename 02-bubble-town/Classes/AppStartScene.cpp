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

#include "Scenes/BtSceneUtil.h"

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
    
    auto root = BtCreateDefaultUIElements(std::bind(&BtQuitGame, std::placeholders::_1), BTSCN_Start);
    if (root)
        addChild(root, 1);
    
    BtTextMenuBuilder mb;
    mb.AddItem(MENU_Bubble, std::bind([] (cocos2d::Ref*) { BtEmitMessage(BTMSG_GotoScene, BTSCN_Bubble); }, std::placeholders::_1));
    mb.AddItem(MENU_Town, std::bind([] (cocos2d::Ref*) { BtEmitMessage(BTMSG_GotoScene, BTSCN_Town); }, std::placeholders::_1));
    mb.AddItem(MENU_World, std::bind([] (cocos2d::Ref*) { BtEmitMessage(BTMSG_GotoScene, BTSCN_World); }, std::placeholders::_1));
    auto menuMain = mb.Build();
    if (menuMain)
    {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        cocos2d::Vec2 menuPos;
        menuPos.x = origin.x + visibleSize.width / 2;
        menuPos.y = origin.y + visibleSize.height / 4;
        menuMain->setPosition(menuPos);
        addChild(menuMain, 1);
    }

    return true;
}
