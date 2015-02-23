
require "def" 

JSON = require "JSON" 
util = require "util"

level_handlers = {}
level_handlers.player = nil
level_handlers.handlerSet = {}


function level_handlers.init(player)
	level_handlers.player = player

	level_handlers.handlerSet[BtMsgID.GotoScene] 		= function (msg) util.goto_scene(msg.info) end
	level_handlers.handlerSet[BtMsgID.StartNextLevel] 	= level_handlers.onStartNextLevel
	level_handlers.handlerSet[BtMsgID.RestartLevel]		= level_handlers.onRestartLevel
	level_handlers.handlerSet[BtMsgID.LevelCompleted]	= level_handlers.onLevelCompleted
	level_handlers.handlerSet[BtMsgID.LevelEntered]		= level_handlers.onLevelEntered
	level_handlers.handlerSet[BtMsgID.LevelLeft]		= level_handlers.onLevelLeft
end


function gotoLevel(player, levelID)
	if level_handlers.player == nil then
		return
	end

	local levelConfig = {
		level_id = levelID,
		level_stats = player.info.level_stats[levelID],		-- this might be nil
		items = player.info.items,
		heroes = player.info.heroes,
	} 

	local succ, ret = pcall(function () return JSON:encode_pretty(levelConfig) end)
	if not succ then 
	 	core.log_err(string.format("Encoding Json failed while preparing level. (%s)", ret))
	 	return
	end

	util.goto_scene(BTSCN_bubble, ret) 
end

function level_handlers.onStartNextLevel(msg)
	gotoLevel(level_handlers.player, level_handlers.player.getNextLevel())
end

function level_handlers.onRestartLevel(msg)
	gotoLevel(level_handlers.player, level_handlers.player.current_level)
end

function level_handlers.onLevelCompleted(msg)
	print("rewards: "..msg.info)
end

function level_handlers.onLevelEntered(msg)
	level_handlers.player.setCurrentLevel(msg.args[1])
end

function level_handlers.onLevelLeft(msg)
	level_handlers.player.setCurrentLevel(nil)
end

return level_handlers

