


simulation = {}

simulation.inbox    = dofile "lua/sim_inbox.lua"
simulation.handlers = dofile "lua/sim_handlers.lua"

function simulation.process_messages()
	if simulation.inbox == nil or simulation.inbox.is_empty() then
		return
	end

	repeat
		local msg = simulation.inbox.fetch()
		if msg ~= nil then
			-- print("msg processed. id: "..msg.id.." info: "..msg.info.." arg_count: "..#msg.args)
			local handler = simulation.handlers[msg.id]
			if handler ~= nil then
				handler(msg)
			end
		end
	until msg == nil
end

function simulation.tick(deltaSeconds)
	simulation.process_messages()
end

return simulation

