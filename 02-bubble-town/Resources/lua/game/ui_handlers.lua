
require "g_shared"

ui_handlers = {}
ui_handlers.simulation = nil

function ui_handlers.init(simulation)
	ui_handlers.simulation = simulation
end

local function on_enter_town(msg)
	simulation.inbox.post(BtMsgID.GotoScene, BTSCN_town)
end

local function on_start_next_level(msg)
	simulation.inbox.post(BtMsgID.StartNextLevel)
end

local function on_restart_level(msg)
	simulation.inbox.post(BtMsgID.RestartLevel)
end

ui_handlers.handlerSet = {

	{ scene=BTSCN_start, ctrl="bt_enter", msgID=BtMsgID.UI_ButtonPressed, handler=on_enter_town },
	{ scene=BTSCN_start, ctrl="bt_next", msgID=BtMsgID.UI_ButtonPressed, handler=on_start_next_level },

	{ scene=BTSCN_bubble, ctrl="restart", msgID=BtMsgID.UI_ButtonPressed, handler=on_restart_level },
	{ scene=BTSCN_bubble, ctrl="next", msgID=BtMsgID.UI_ButtonPressed, handler=on_start_next_level },
}

return ui_handlers

