/*!
 * \file BtSceneUtil.cpp
 * \date 2014/12/25 16:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtSceneUtil
*/

#include "stdafx.h"
#include "BtSceneUtil.h"

#include "MsgHandling/BtMsgDef.h"
#include "MsgHandling/BtMsgDispatcher.h"

#include "AppStartScene.h"
#include "BtTestScene.h"
#include "BtTownScene.h"
#include "BtWorldScene.h"

#include "TexturePool/TexturePoolTestScene.h"
#include "TexturePool/TexturePoolTestScene_Anim.h"

#include "AppMacros.h"

namespace 
{
    typedef std::function<cocos2d::Scene* ()> sceneCreator_t;
    static std::map<std::string, sceneCreator_t> s_sceneCreatorLut;
    static void BtInitSceneCreation()
    {
        s_sceneCreatorLut[BTSCN_Start] = &BtCreateScene<AppStartScene>;
        s_sceneCreatorLut[BTSCN_Test] = &BtCreateScene<BtTestScene>;
        s_sceneCreatorLut[BTSCN_Town] = &BtCreateScene<BtTownScene>;
        s_sceneCreatorLut[BTSCN_World] = &BtCreateScene<BtWorldScene>;
        s_sceneCreatorLut[BTSCN_TexturePool] = &BtCreateScene<TexturePoolTestScene>;
        s_sceneCreatorLut[BTSCN_TexturePool_Anim] = &BtCreateScene<TexturePoolTestScene_Anim>;
    }

    static cocos2d::Scene* BtCreateSceneByName(const std::string& sceneName)
    {
        auto it = s_sceneCreatorLut.find(sceneName);
        if (it == s_sceneCreatorLut.end())
            return nullptr;

        return it->second();
    }
}

void BtMsgGotoScene_Emit(BtConstStr sceneName)
{
    if (!BtMsgDispatcher::Get())
        return;

    BtMsg msg(BTMSG_GotoScene, sceneName);
    BtMsgDispatcher::Get()->Notify(msg);
}


bool BtMsgGotoScene_Handle(BtMsg& msg)
{
    auto director = cocos2d::Director::getInstance();
    if (!director)
        return false;

    if (s_sceneCreatorLut.empty())
        BtInitSceneCreation();

    cocos2d::Scene* scene = BtCreateSceneByName(msg.m_info);
    if (!scene)
    {
        CCLOGERROR("The creator of scene ('%s') not found, scene creation failed.", msg.m_info.c_str());
        return false;
    }

    director->replaceScene(scene);
    return true;
}

void BtStdHandler_BackToMainMenu(cocos2d::Ref* sender)
{
    BtMsgGotoScene_Emit(BTSCN_Start);
}

void BtStdHandler_QuitGame(cocos2d::Ref* sender)
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

cocos2d::Menu* BtCreateDefaultUIElements(const cocos2d::ccMenuCallback& closeButtonHandler, const std::string& title)
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto closeItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png", closeButtonHandler);
    if (!closeItem)
        return nullptr;

    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    if (!menu)
        return nullptr;

    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));
    menu->setPosition(cocos2d::Vec2::ZERO);

    // only create title label when it's specified
    if (title.length())
    {
        auto label = cocos2d::LabelTTF::create(title, "Arial", TITLE_FONT_SIZE);
        label->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height);
        menu->addChild(label);
    }

    return menu;
}

