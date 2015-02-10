
default_player_info = dofile "lua/player_info.lua"
core = dofile "lua/core.lua"

-- this library is found here
-- 		http://regex.info/blog/lua/json
JSON = (loadfile "lua/JSON/JSON.lua")() -- one-time load of the routines

player = {}

player.info = default_player_info
player.profile_name = "__unnamed__"

function player.save()
	if not core.save_json(core.build_profile_path(player.profile_name)) then
		return false
	end

	return true
end

function player.load(profileName)
	local data = core.load_json(core.build_profile_path(profileName))
	if data == nil then 
		return false
	end

	player.info = data
	player.profile_name = profileName
	return true
end

return player

