
default_player_info = require "player_info"
core = require "core"

JSON = require "JSON" 

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

function player.refresh_ui()
	bt_native.set_player_property(BTPL_Level, player.info[BTPL_Level], false)
	bt_native.set_player_property(BTPL_Exp, player.info[BTPL_Exp], false)
	bt_native.set_player_property(BTPL_Coins, player.info[BTPL_Coins], false)
	bt_native.set_player_property(BTPL_Diamonds, player.info[BTPL_Diamonds], false)
	bt_native.set_player_property(BTPL_Energy, player.info[BTPL_Energy], true)
end

return player

