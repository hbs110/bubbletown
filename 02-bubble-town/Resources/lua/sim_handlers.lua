
level = dofile "lua/level.lua"

handlers = {}

handlers.player = nil

handlers[BtMsgID.GotoScene] = function (msg) util.goto_scene(msg.info) end

handlers[BtMsgID.StartNextLevel] = 
	function (msg)
		if handlers.player == nil then
			return
		end

		local levelCfg = level.prepare(handlers.player)
		util.goto_scene(BTSCN_bubble, levelCfg) 
	end

return handlers

