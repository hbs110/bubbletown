/*!
 * \file BtSceneUtil.cpp
 * \date 2014/12/25 16:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtSceneUtil
*/

#include "stdafx.h"
#include "BtSceneUtil.h"

#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"

#include "Scenes/Scene_Bubble/BtBubbleScene.h"
#include "Scenes/Scene_Town/BtTownScene.h"
#include "Scenes/Scene_World/BtWorldScene.h"

#include "AppStartScene.h"
#include "AppMacros.h"

namespace 
{
    typedef std::function<cocos2d::Scene* ()> sceneCreator_t;
    static std::map<std::string, sceneCreator_t> s_sceneCreatorLut;
    static void BtInitSceneCreation()
    {
        s_sceneCreatorLut[BTSCN_Start] = &BtCreateScene<AppStartScene>;
        s_sceneCreatorLut[BTSCN_Bubble] = &BtCreateScene<BtBubbleScene>;
        s_sceneCreatorLut[BTSCN_Town] = &BtCreateScene<BtTownScene>;
        s_sceneCreatorLut[BTSCN_World] = &BtCreateScene<BtWorldScene>;
    }

    static cocos2d::Scene* BtCreateSceneByName(const std::string& sceneName)
    {
        auto it = s_sceneCreatorLut.find(sceneName);
        if (it == s_sceneCreatorLut.end())
            return nullptr;

        return it->second();
    }
}

bool BtHandleMsg_GotoScene(BtMsg& msg)
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


cocos2d::Node* BtCreateDefaultUIElements(const cocos2d::ccMenuCallback& closeButtonHandler, const std::string& title)
{
    auto root = cocos2d::Node::create();
    if (!root)
        return nullptr;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto closeItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png", closeButtonHandler);
    if (!closeItem)
        return nullptr;
    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    if (!menu)
        return nullptr;
    menu->setPosition(cocos2d::Vec2::ZERO);
    root->addChild(menu, 1);

    // only create title label when it's specified
    if (title.length())
    {
        auto label = cocos2d::LabelTTF::create(title, "Arial", TITLE_FONT_SIZE);
        label->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height);
        root->addChild(label, 1);
    }

    return root;
}

