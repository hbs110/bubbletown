/*!
 * \file AppNativeInterfaces.cpp
 * \date 2015/02/10 11:00
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of AppNativeInterfaces
*/

#include "stdafx.h"
#include "AppNativeInterfaces.h"

#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"

#include "Scenes/Scene_Town/BtTownScene.h"

#include "AppDelegate.h"
#include "AppUtil.h"

void AppNativeInterfaces::GotoScene(const std::string& sceneName, const std::string& sceneConfig)
{
    BtMsg msg((int)BtMsgID::GotoScene, sceneName);
    msg.m_extraParams.push_back(sceneConfig);
    BtMsgDispatcher::Get()->receive(msg);
}

double AppNativeInterfaces::GetCurrentGameTime()
{
    AppDelegate* ad = AppUtil::GetApp();
    return ad ? ad->GetCurTime() : 0.0;
}

void AppNativeInterfaces::SetPlayerProperty(const std::string& playerProperty, int value, bool flush)
{
    BtTownScene* town = AppUtil::GetTownSceneIfIn();
    if (town)
    {
        BtTownSceneUI* ui = town->GetUI();
        ui->setPlayerProperty(playerProperty, value, flush);
    }
}

void BtRegisterNativeInerfaces()
{
    BtLuaService::Get()->RegisterFunction("goto_scene", &AppNativeInterfaces::GotoScene);
    BtLuaService::Get()->RegisterFunction("get_current_time", &AppNativeInterfaces::GetCurrentGameTime);
    BtLuaService::Get()->RegisterFunction("set_player_property", &AppNativeInterfaces::SetPlayerProperty);
}

