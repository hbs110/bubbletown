
-- this library is found here
-- 		http://regex.info/blog/lua/json
JSON = require "JSON" 

core = {}

PLAYER_PROFILE_DIR = "users/"

function core.checkpoint(name)
	print(string.format("[CP] '%s' reached at '%.3f'.", name, bt_native.get_current_time()))
end	

function core.log_err(info)
	print(string.format("[ERROR] '%s' at '%.3f'.", info, bt_native.get_current_time()))
end	

function core.build_profile_path(profileName)
	return PLAYER_PROFILE_DIR..profileName..".json"
end

function core.load_json(file)
	local f, err = io.open(file, "r")
	if f == nil then
	 	core.log_err(string.format("Opening file '%s' failed. (%s)", file, err))
		return nil
	end

	local content = f:read("*all")
	if not content then
	 	core.log_err(string.format("Reading file '%s' content failed.", file))
		f:close()
		return nil
	end
	f:close()

	local succ, ret = pcall(function () return JSON:decode(content) end)
	if not succ then 
	 	core.log_err(string.format("Decoding Json '%s' failed. (%s) (%s)", file, ret, content))
	 	return nil
	end

	return ret
end

function core.save_json(file)
	local succ, ret = pcall(function () return JSON:encode_pretty(player.info) end)
	if not succ then 
	 	core.log_err(string.format("Encoding Json failed. (%s)", ret))
	 	return false
	end

	local f, err = io.open(file, "w")
	if not f then
	 	core.log_err(string.format("Opening file '%s' failed. (%s)", file, err))
		return false
	end

    succ, ret = f:write(ret)
	if not succ then 
	 	core.log_err(string.format("Writing Json '%s' failed. (%s)", file, ret))
		f:close()
	 	return false
	end

	f:close()
	return true
end

return core