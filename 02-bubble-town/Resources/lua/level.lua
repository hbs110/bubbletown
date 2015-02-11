
JSON = dofile "lua/JSON/JSON.lua" 

level = {}

function level.prepare(player)
	local levelConfig = {
		level_id = player.get_next_level(),
		items = player.info.items,
		heros = player.info.heros,
	} 

	local succ, ret = pcall(function () return JSON:encode_pretty(levelConfig) end)
	if not succ then 
	 	core.log_err(string.format("Encoding Json failed while preparing level. (%s)", ret))
	 	return ""
	end

	return ret
end

return level
