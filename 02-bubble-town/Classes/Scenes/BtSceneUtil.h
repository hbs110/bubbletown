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

// emit the 'GotoScene' message
void BtMsgGotoScene_Emit(BtConstStr sceneName);

// handler of the 'GotoScene' message, should be subscribed at the beginning of the game
bool BtMsgGotoScene_Handle(BtMsg& msg);
