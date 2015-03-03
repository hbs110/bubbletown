/*!
 * \file AppMsgHandlers.cpp
 * \date 2015/03/03 13:51
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of AppMsgHandlers
*/

#include "stdafx.h"
#include "AppMsgHandlers.h"

#include "Core/BtCoreUtil.h"

#include "Services/BtLuaService.h"

#include "Scenes/BtBaseScene.h"
#include "Scenes/BtSceneFactory.h"

#include "AppUtil.h"

void AppMsgHandlers::registerGlobally()
{
    BtMsgDispatcher::Get()->subscribe((int) BtMsgID::GotoScene, &AppMsgHandlers::onGotoScene, (btflags) BtMsgFlag::Sys);
    BtMsgDispatcher::Get()->subscribe((int) BtMsgID::UI_ButtonPressed, &AppMsgHandlers::onButtonPressed, (btflags) BtMsgFlag::Sys);
    BtMsgDispatcher::Get()->subscribe((int) BtMsgID::UI_ButtonReleased, &AppMsgHandlers::onButtonReleased, (btflags) BtMsgFlag::Sys);
}

bool AppMsgHandlers::onGotoScene(BtMsg& msg)
{
    auto director = cocos2d::Director::getInstance();
    if (!director)
        return false;

    if (msg.m_extraParams.size() != 1)
        return false;

    cocos2d::Scene* scene = BtSceneFactory::Get()->Create(msg.m_info);
    BT_EXPECT_RET_V2(scene, tfm::format("scene ('%s') creation failed.", msg.m_info), false);

    for (auto c : scene->getChildren())
    {
        BtBaseScene* scene = dynamic_cast<BtBaseScene*>(c);
        if (scene)
        {
            scene->preEnter(msg.m_extraParams[0]);
        }
    }

    if (director->getRunningScene())
    {
        director->replaceScene(scene);
    }
    else
    {
        director->runWithScene(scene);
    }
    return true;
}

bool AppMsgHandlers::onButtonPressed(BtMsg& msg)
{
    std::string scnName = AppUtil::GetCurrentSceneName();
    BT_POST_LUA_AND_FLUSH(BtMsgID::UI_ButtonPressed, scnName, msg.m_info);
    return true;
}

bool AppMsgHandlers::onButtonReleased(BtMsg& msg)
{
    BT_POST_LUA_AND_FLUSH(BtMsgID::UI_ButtonReleased, AppUtil::GetCurrentSceneName(), msg.m_info);
    return true;
}

