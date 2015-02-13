
-- 消息定义

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
	LevelRewards = 5,

	----------------------------------------
	-- Town Building

	-- construction 
	ConstructBuilding = 20,
	CancelConstructing = 23,
	ConstructingFinished = 26,
	-- moving 
	MoveBuilding = 21,			-- "cancel last move" would be "move to original position" 
	-- upgrading
	UpgradeBuilding = 22,
	CancelUpgrading = 25,
	UpgradingFinished = 27,

	----------------------------------------
	-- UI events
	
	UI_ButtonPressed = 100,  	
};


-- 场景定义

BTSCN_start     = "StartScene";
BTSCN_bubble    = "BubbleScene";
BTSCN_town      = "TownScene";
BTSCN_world     = "WorldScene"; 


-- 建筑定义

BT_Hall		= "hall";
BT_Deco		= "deco";
BT_House	= "house";
BT_Shop		= "shop";
BT_Workshop	= "workshop";
