/*
    !!! 此文件由脚本生成，请勿直接修改 !!!

    如需修改，请修改文件 Resources/lua/shared/def.lua ，并运行 gen_cpp_defs.py 重新生成此头文件
*/

#pragma once

#include "BtCoreDef.h"


// 消息定义

enum class BtMsgID
{
	//////////////////////////////////////-
	// Game events

	GotoScene = 0,  			

	//////////////////////////////////////-
	// Level events

	RestartLevel = 1,			
	StartNextLevel = 2,			
	LevelEntered = 3,
	LevelLeft = 4,
	LevelRewards = 5,

	//////////////////////////////////////-
	// Town Building

	// construction 
	ConstructBuilding = 20,
	CancelConstructing = 23,
	ConstructingFinished = 26,
	// moving 
	MoveBuilding = 21,			// "cancel last move" would be "move to original position" 
	// upgrading
	UpgradeBuilding = 22,
	CancelUpgrading = 25,
	UpgradingFinished = 27,

	//////////////////////////////////////-
	// UI events
	
	UI_ButtonPressed = 100,  	
};


// 场景定义

BtConstStr BTSCN_start     = "StartScene";
BtConstStr BTSCN_bubble    = "BubbleScene";
BtConstStr BTSCN_town      = "TownScene";
BtConstStr BTSCN_world     = "WorldScene"; 


// 建筑定义

BT_Hall		= "hall";
BT_Deco		= "deco";
BT_House	= "house";
BT_Shop		= "shop";
BT_Workshop	= "workshop";
