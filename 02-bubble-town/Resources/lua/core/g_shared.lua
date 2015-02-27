
-- messages

BtMsgID = 
{
    ----------------------------------------
    -- Game events

    GotoScene = 0,              

    ----------------------------------------
    -- Level events

    RestartLevel = 1,           
    StartNextLevel = 2,         
    LevelEntered = 3,
    LevelLeft = 4,
    LevelCompleted = 5,

    ----------------------------------------
    -- Town Building

    -- construction 
    ConstructBuilding = 20,
    CancelConstructing = 23,
    ConstructingFinished = 26,
    -- moving 
    MoveBuilding = 21,          -- "cancel last move" would be "move to original position" 
    -- upgrading
    UpgradeBuilding = 22,
    CancelUpgrading = 25,
    UpgradingFinished = 27,

    ----------------------------------------
    -- UI events
    
    UI_ButtonPressed = 100,     
};


-- scenes

BTSCN_start     = "StartScene";
BTSCN_bubble    = "BubbleScene";
BTSCN_town      = "TownScene";
BTSCN_world     = "WorldScene"; 


-- buildings

BT_Hall     = "hall";
BT_Deco     = "deco";
BT_House    = "house";
BT_Shop     = "shop";
BT_Workshop = "workshop";


-- player properties

BTPL_Level    = "level";
BTPL_Exp      = "exp";
BTPL_Coins    = "coins";
BTPL_Diamonds = "diamonds";
BTPL_Energy   = "energy";


-- Probability Level  
-- (defines a word-set to describe the happening probability of a given event) 
-- The number would be the percentage count. for example, 'BtProb.High = 30' means for the target event
-- (probability marked as 'High'), there would be a 30% chance to make it happen in the given context.
BtProb = 
{
    None = 0, 
    Low = 5, 
    Medium = 15, 
    High = 30, 
    Must = 100, 
};


-- invalid index 
--  mainly used for:
--      1. invalid return value checking
--      2. default element in a table for demonstration
BT_Invalid = -1;
