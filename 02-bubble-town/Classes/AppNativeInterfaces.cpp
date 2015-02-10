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

#include "AppDelegate.h"

AppDelegate* BtGetApp()
{
    cocos2d::Application* app = cocos2d::Application::getInstance();
    if (!app)
        return nullptr;

    return dynamic_cast<AppDelegate*>(app);
}

void AppNativeInterfaces::EmitMsg_GotoScene(const std::string& sceneName)
{
    BtEmitMessage(BTMSG_GotoScene, sceneName);
}

double AppNativeInterfaces::GetCurrentGameTime()
{
    AppDelegate* ad = BtGetApp();
    return ad ? ad->GetCurTime() : 0.0;
}

bool AppNativeInterfaces::CopyFile(const std::string& src, const std::string& dest)
{

    return true;
}