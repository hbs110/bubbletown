
simulation = {}

simulation.inbox    = require "sim_inbox"

local handlers = {}
local ui_handlers = {}

function simulation.register_handlers(handlerSet)
	for k,v in pairs(handlerSet) do
		handlers[k] = v
	end
end

function simulation.register_ui_handler(sceneName, ctrlName, msgID, handlerFunc)
	local k = sceneName.."|"..ctrlName.."|"..msgID
	ui_handlers[k] = handlerFunc
end

local function is_ui_msg(msg)
    return msg.id == BtMsgID.UI_ButtonPressed
end

local function execute_ui_msg(msg)
	local k = msg.info.."|"..msg.args[1].."|"..msg.id
	local handler = ui_handlers[k]
	if handler ~= nil then
		handler(msg)
	end
end

function simulation.process_messages()
	if simulation.inbox == nil or simulation.inbox.is_empty() then
		return
	end

	while true do
		local msg = simulation.inbox.fetch()
		if msg == nil then
            break
        end

		--print("msg processed. id: "..msg.id.." info: "..msg.info.." arg_count: "..#msg.args)

        if is_ui_msg(msg) then
            execute_ui_msg(msg)
        else
			local handler = handlers[msg.id]
			if handler ~= nil then
				handler(msg)
			end
        end
	end
end

function simulation.tick(deltaSeconds)
	simulation.process_messages()
end

return simulation

