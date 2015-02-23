
JSON = require "JSON" 

require "tab_levels" 
require "g_constants"

default_player_info = require "player_info"

player = {}

player.info = default_player_info
player.profile_name = DEFAULT_PLAYER_PROFILE
player.current_level = nil

function player.save()
	if player.profile_name == DEFAULT_PLAYER_PROFILE then
		print("profile not been correctly initialized, saving request ignored.")
		return true
	end

	local profilePath = g_build_profile_path(player.profile_name)
	if not g_save_json(profilePath) then
		return false
	end

	print(string.format("profile saved into '%s'.", profilePath))
	return true
end

function player.load(profileName)
	local data = g_load_json(g_build_profile_path(profileName))
	if data == nil then 
		return false
	end

	-- merge the loaded profile into player.info
	for k,v in pairs(data) do
		player.info[k] = v 
	end

	player.profile_name = profileName
	g_checkpoint(string.format("player profile ('%s') loaded.", player.profile_name))
	return true
end

function player.getNextLevel()
	local curLevel = BT_Invalid
	if player.current_level ~= nil then
		curLevel = player.current_level
	else
		for k,v in pairs(player.info.level_stats) do
			if k > curLevel then
				curLevel = k
			end
		end
	end

	for _,v in ipairs(t_level_id_list) do
		if v > curLevel then
			return v
		end
	end
	return BT_Invalid
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

