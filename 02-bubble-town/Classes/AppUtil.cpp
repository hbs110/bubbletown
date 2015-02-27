/*!
 * \file AppUtil.cpp
 * \date 2015/02/10 11:00
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of AppUtil
*/

#include "stdafx.h"
#include "AppUtil.h"

#include "AppDelegate.h"
#include "Scenes/Scene_Town/BtTownScene.h"

AppDelegate* AppUtil::GetApp()
{
    cocos2d::Application* app = cocos2d::Application::getInstance();
    if (!app)
        return nullptr;

    return dynamic_cast<AppDelegate*>(app);
}

BtTownScene* AppUtil::GetTownSceneIfIn()
{
    auto director = cocos2d::Director::getInstance();
    if (!director)
        return nullptr;

    cocos2d::Scene* curScene = director->getRunningScene();
    if (!curScene || !curScene->getChildrenCount())
        return nullptr;

    cocos2d::Node* curLayer = curScene->getChildByName(BTSCN_town);
    if (!curLayer)
        return nullptr;

    return dynamic_cast<BtTownScene*>(curLayer);
}
