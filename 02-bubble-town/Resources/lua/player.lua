
default_player_info = dofile "lua/player_info.lua"
core = dofile "lua/core.lua"

-- this library is found here
-- 		http://regex.info/blog/lua/json
JSON = (loadfile "lua/JSON/JSON.lua")() -- one-time load of the routines

player = {}

player.info = default_player_info
player.profile_name = "__unnamed__"
player.current_level = nil

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

function player.getNextLevel()
	if player.current_level ~= nil then
		return player.current_level + 1
	else
		return 1
	end
end

function player.setCurrentLevel(currentLevel)
	player.current_level = currentLevel
	if player.current_level ~= nil then
		print(string.format("player is now in level '%d'.", player.current_level))
	else
		print("player is now out of level.")
	end
end

return player

