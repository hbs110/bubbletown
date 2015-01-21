/*!
 * \file BtSceneUtil.h
 * \date 2014/12/25 16:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtSceneUtil 
*/

#pragma once

#include "Core/BtCoreDef.h"
#include "Core/BtMsgDispatcher.h"

BtConstStr BTSCN_Start  = "StartScene";
BtConstStr BTSCN_Bubble = "BubbleScene";
BtConstStr BTSCN_Town   = "TownScene";
BtConstStr BTSCN_World  = "WorldScene";

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

// handler of the 'GotoScene' message, should be subscribed at the beginning of the game
bool BtHandleMsg_GotoScene(BtMsg& msg);

// reusable helper funcion (close button)
cocos2d::Node* BtCreateDefaultUIElements(const cocos2d::ccMenuCallback& closeButtonHandler, const std::string& title = "");


