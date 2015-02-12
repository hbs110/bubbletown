/*
    !!! 此文件由脚本生成，请勿直接修改 !!!

    如需修改，请修改文件 Resources/lua/shared/def.lua ，并运行 gen_cpp_defs.py 重新生成此头文件
*/

#pragma once

#include "BtCoreDef.h"


// 消息定义

enum class BtMsgID
{
	// Game events
	GotoScene = 0,  			// 跳转到场景
	RestartLevel = 1,			// 重玩本关
	StartNextLevel = 2,			// 开始下一关
	LevelEntered = 3,
	LevelLeft = 4,
	LevelRewards = 5,

	// UI events
	UI_ButtonPressed = 100,  	
};


// 场景定义

BtConstStr BTSCN_start     = "StartScene";
BtConstStr BTSCN_bubble    = "BubbleScene";
BtConstStr BTSCN_town      = "TownScene";
BtConstStr BTSCN_world     = "WorldScene"; 


