
default_player_info = dofile "lua/player_info.lua"
core = dofile "lua/core.lua"

-- this library is found here
-- 		http://regex.info/blog/lua/json
JSON = (loadfile "lua/JSON/JSON.lua")() -- one-time load of the routines

player = {}

player.info = default_player_info
player.profile_name = "__unnamed__"

function player.save()
	local profilePath = core.build_profile_path(player.profile_name)
	if not core.save_json(profilePath) then
		return false
	end

	print(string.format("profile saved into '%s'.", profilePath))
	return true
end

function player.load(profileName)
	local data = core.load_json(core.build_profile_path(profileName))
	if data == nil then 
		return false
	end

	-- merge the loaded profile into player.info
	for k,v in pairs(data) do
		player.info[k] = v 
	end

	player.profile_name = profileName
	return true
end

function player.get_next_level()
	return 1
end

return player

