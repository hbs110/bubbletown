
simulation = {}

simulation.inbox    = require "sim_inbox"
simulation.handlers_ui = require "sim_handlers_ui"
simulation.handlers = {}

function simulation.register_handlers(handlerSet)
	for k,v in pairs(handlerSet) do
		simulation.handlers[k] = v
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

        if simulation.handlers_ui.is_ui_msg(msg) then
            simulation.handlers_ui.execute(msg)
        else
			local handler = simulation.handlers[msg.id]
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

