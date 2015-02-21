/*
    !!! GENERATED FROM LUA !!!

    For editing, 
        1. modify the file 'Resources/lua/shared/def.lua'
        2. run 'gen_cpp_defs.py' to regenerate this file
*/

#pragma once

#include "BtCoreDef.h"


// messages

enum class BtMsgID
{
    ////////////////////////////////////////
    // Game events

    GotoScene = 0,              

    ////////////////////////////////////////
    // Level events

    RestartLevel = 1,           
    StartNextLevel = 2,         
    LevelEntered = 3,
    LevelLeft = 4,
    LevelRewards = 5,

    ////////////////////////////////////////
    // Town Building

    // construction 
    ConstructBuilding = 20,
    CancelConstructing = 23,
    ConstructingFinished = 26,
    // moving 
    MoveBuilding = 21,          // "cancel last move" would be "move to original position" 
    // upgrading
    UpgradeBuilding = 22,
    CancelUpgrading = 25,
    UpgradingFinished = 27,

    ////////////////////////////////////////
    // UI events
    
    UI_ButtonPressed = 100,     
};


// scenes

BtConstStr BTSCN_start     = "StartScene";
BtConstStr BTSCN_bubble    = "BubbleScene";
BtConstStr BTSCN_town      = "TownScene";
BtConstStr BTSCN_world     = "WorldScene"; 


// buildings

BtConstStr BT_Hall     = "hall";
BtConstStr BT_Deco     = "deco";
BtConstStr BT_House    = "house";
BtConstStr BT_Shop     = "shop";
BtConstStr BT_Workshop = "workshop";


// player properties

BtConstStr BTPL_Level    = "level";
BtConstStr BTPL_Exp      = "exp";
BtConstStr BTPL_Coins    = "coins";
BtConstStr BTPL_Diamonds = "diamonds";
BtConstStr BTPL_Energy   = "energy";


// Probability Level  
// (defines a word-set to describe the happening probability of a given event) 
// The number would be the percentage count. for example, 'BtProb.High = 30' means for the target event
// (probability marked as 'High'), there would be a 30% chance to make it happen in the given context.
enum class BtProb
{
    None = 0, 
    Low = 5, 
    Medium = 15, 
    High = 30, 
    Must = 100, 
};


// invalid index 
//  mainly used for:
//      1. invalid return value checking
//      2. default element in a table for demonstration
const int BT_Invalid = -1;
