
JSON = require "JSON" 

level = {}

function level.prepare(player, levelID)
	local levelConfig = {
		level_id = levelID,
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
