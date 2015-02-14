
level = require "level"

handlers = {}

handlers.player = nil

function onStartNextLevel(msg)
	if handlers.player == nil then
		return
	end

	local levelCfg = level.prepare(handlers.player, handlers.player.getNextLevel())
	util.goto_scene(BTSCN_bubble, levelCfg) 
end

function onRestartLevel(msg)
	if handlers.player == nil then
		return
	end

	local levelCfg = level.prepare(handlers.player, handlers.player.current_level)
	util.goto_scene(BTSCN_bubble, levelCfg) 
end

function onLevelRewards(msg)
	if handlers.player == nil then
		return
	end

	print("rewards: "..msg.info)
end


handlers[BtMsgID.GotoScene] 		= function (msg) util.goto_scene(msg.info) end
handlers[BtMsgID.StartNextLevel] 	= onStartNextLevel
handlers[BtMsgID.RestartLevel] 		= onRestartLevel
handlers[BtMsgID.LevelRewards] 		= onLevelRewards
handlers[BtMsgID.LevelEntered] 		= function (msg) handlers.player.setCurrentLevel(msg.args[1]) end
handlers[BtMsgID.LevelLeft] 		= function (msg) handlers.player.setCurrentLevel(nil) end

return handlers

