/*!
 * \file BtSceneUtil.h
 * \date 2014/12/25 16:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtSceneUtil 
*/

#pragma once

#include "Core/BtCoreDef.h"
#include "MsgHandling/BtMsgDispatcher.h"

BtConstStr BTSCN_Start  = "StartScene";
BtConstStr BTSCN_Test   = "TestScene";
BtConstStr BTSCN_Town   = "TownScene";
BtConstStr BTSCN_World  = "WorldScene";
BtConstStr BTSCN_TexturePool        = "TexturePoolScene";
BtConstStr BTSCN_TexturePool_Anim   = "TexturePoolScene_Anim";


// the default scene creation process
template <typename T>
cocos2d::Scene* BtCreateScene()
{
    T *layer = T::create(); // create() should guarantee the layer pointer is autoreleased 
    if (!layer)
        return nullptr;

    auto scene = cocos2d::Scene::create();  // Scene::create() ensures creating autorelease object
    if (!scene)
        return nullptr;

    scene->addChild(layer);
    return scene;
}

// emit the 'GotoScene' message
void BtMsgGotoScene_Emit(BtConstStr sceneName);

// handler of the 'GotoScene' message, should be subscribed at the beginning of the game
bool BtMsgGotoScene_Handle(BtMsg& msg);


