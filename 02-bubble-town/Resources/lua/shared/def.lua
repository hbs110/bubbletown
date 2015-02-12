
-- 消息定义

BtMsgID = 
{
	-- Game events
	GotoScene = 0,  			-- 跳转到场景
	RestartLevel = 1,			-- 重玩本关
	StartNextLevel = 2,			-- 开始下一关
	LevelEntered = 3,
	LevelLeft = 4,
	LevelRewards = 5,

	-- UI events
	UI_ButtonPressed = 100,  	
};


-- 场景定义

BTSCN_start     = "StartScene";
BTSCN_bubble    = "BubbleScene";
BTSCN_town      = "TownScene";
BTSCN_world     = "WorldScene"; 


