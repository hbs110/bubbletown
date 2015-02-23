
JSON = require "JSON" 

require "g_shared" 
require "g_util"

level_handlers = {}
level_handlers.player = nil

function level_handlers.init(player)
	level_handlers.player = player
end

function level_handlers.onLevelCompleted(msg)
	packed = g_load_json_from_string(msg.info)
	--g_print_table_pretty(packed)
	if packed == nil or packed.level_rewards == nil or packed.level_stats == nil then
		error("bad json received, ignored.")
	end

	level_handlers.player.onLevelCompleted(packed.level_rewards, packed.level_stats)
end

function level_handlers.onLevelEntered(msg)
	level_handlers.player.setCurrentLevel(msg.args[1])
end

function level_handlers.onLevelLeft(msg)
	level_handlers.player.setCurrentLevel(nil)
end

local function gotoLevel(player, levelID)
	if level_handlers.player == nil then
		return
	end

	local levelConfig = {
		level_id = levelID,
		level_stats = level_handlers.player.info.level_stats[levelID],		-- this might be nil
		items = level_handlers.player.info.items,
		heroes = level_handlers.player.info.heroes,
	} 

	local succ, ret = pcall(function () return JSON:encode_pretty(levelConfig) end)
	if not succ then 
	 	g_log_err(string.format("Encoding Json failed while preparing level. (%s)", ret))
	 	return
	end

	g_goto_scene(BTSCN_bubble, ret) 
end

level_handlers.handlerSet = {}
level_handlers.handlerSet[BtMsgID.GotoScene] 		= function (msg) g_goto_scene(msg.info) end
level_handlers.handlerSet[BtMsgID.StartNextLevel] 	= function (msg) gotoLevel(level_handlers.player, level_handlers.player.getNextLevel()) end
level_handlers.handlerSet[BtMsgID.RestartLevel]		= function (msg) gotoLevel(level_handlers.player, level_handlers.player.current_level) end
level_handlers.handlerSet[BtMsgID.LevelCompleted]	= level_handlers.onLevelCompleted
level_handlers.handlerSet[BtMsgID.LevelEntered]		= level_handlers.onLevelEntered
level_handlers.handlerSet[BtMsgID.LevelLeft]		= level_handlers.onLevelLeft

return level_handlers

