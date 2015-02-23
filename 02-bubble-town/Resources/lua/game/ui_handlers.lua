
require "def"

util = require "util"

ui_handlers = {}
ui_handlers.simulation = nil

function ui_handlers.init(simulation)
	ui_handlers.simulation = simulation
end

local function on_bubble_restart(msg)
	simulation.inbox.post(BtMsgID.RestartLevel)
end

local function on_bubble_next(msg)
	simulation.inbox.post(BtMsgID.StartNextLevel)
end

ui_handlers.handlerSet = {
	{ scene=BTSCN_bubble, ctrl="restart", msgID=BtMsgID.UI_ButtonPressed, handler=on_bubble_restart },
	{ scene=BTSCN_bubble, ctrl="next", msgID=BtMsgID.UI_ButtonPressed, handler=on_bubble_next },
}

return ui_handlers

