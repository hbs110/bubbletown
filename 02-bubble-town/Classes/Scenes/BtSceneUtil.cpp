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

