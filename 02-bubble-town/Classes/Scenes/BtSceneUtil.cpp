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

    if (msg.m_info == BTSCN_Start)
    {
        director->replaceScene(AppStartScene::scene());
        return true;
    }
    else if (msg.m_info == BTSCN_Test)
    {
        director->replaceScene(BtTestScene::scene());
        return true;
    }
    else if (msg.m_info == BTSCN_Town)
    {
        director->replaceScene(BtTownScene::scene());
        return true;
    }
    else if (msg.m_info == BTSCN_World)
    {
        director->replaceScene(BtWorldScene::scene());
        return true;
    }
    else if (msg.m_info == BTSCN_TexturePool)
    {
        director->replaceScene(TexturePoolTestScene::scene());
        return true;
    }
    else if (msg.m_info == BTSCN_TexturePool_Anim)
    {
        director->replaceScene(TexturePoolTestScene_Anim::scene());
        return true;
    }

    return false;
}

