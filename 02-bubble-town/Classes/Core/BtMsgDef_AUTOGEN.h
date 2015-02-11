/*
    !!! 此文件由脚本生成，请勿直接修改 !!!

    如需修改，请修改文件 Resources/lua/shared/def.lua ，并运行 gen_cpp_defs.py 重新生成此头文件
*/

#pragma once

#include "BtCoreDef.h"


// 消息定义

enum class BtMsgID
{
	GotoScene = 0,  	// 跳转到场景
	UI_ButtonPressed,  	// 按钮被按下
};


// 场景定义

BtConstStr BTSCN_start     = "StartScene";
BtConstStr BTSCN_bubble    = "BubbleScene";
BtConstStr BTSCN_town      = "TownScene";
BtConstStr BTSCN_world     = "WorldScene"; 
