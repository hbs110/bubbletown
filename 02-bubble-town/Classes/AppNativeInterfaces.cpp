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

void AppNativeInterfaces::EmitMsg_GotoScene(const std::string& sceneName)
{
    BtEmitMessage(BTMSG_GotoScene, sceneName);
}
