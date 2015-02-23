
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

	-- print("player.info.[post loading]")
	-- g_print_table_pretty(player.info)

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
			if tonumber(k) > curLevel then
				curLevel = tonumber(k)
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

function player.onLevelCompleted(level_rewards, level_stats)
	print(string.format("player completes level '%d'.", player.current_level))
	print(string.format("player gets rewards: coins=%d, exp=%d, heroes=%d.", level_rewards.coins, level_rewards.exp, level_rewards.heroes))
	print(string.format("level_stats: stars=%d, score=%d, time_spent=%.2f.", level_stats.stars, level_stats.score, level_stats.time_spent))

	-- apply rewards
	player.info[BTPL_Coins] = player.info[BTPL_Coins] + level_rewards.coins
	player.info[BTPL_Exp] = player.info[BTPL_Exp] + level_rewards.exp
	print(string.format("player info (coins='%d', exp='%d')", player.info[BTPL_Coins], player.info[BTPL_Exp]))

	if player.info[BTPL_Exp] > 20 then
		player.info[BTPL_Level] = player.info[BTPL_Level] + 1
		print(string.format("player level up! (now '%d')", player.info[BTPL_Level]))

		player.info[BTPL_Exp] = player.info[BTPL_Exp] - 20
		print(string.format("player exp changed to '%d'.", player.info[BTPL_Exp]))
	end

	-- refreshing level stats
	local saved_stats = player.info.level_stats[tostring(player.current_level)]
	if saved_stats == nil then
		print(string.format("player completes level '%d' for the first time.", player.current_level))
		player.info.level_stats[tostring(player.current_level)] = { stars=level_stats.stars, highest_score=level_stats.score, time_spent=level_stats.time_spent }
	else
		if level_stats.stars > saved_stats.stars then
			print(string.format("player refreshes star count: new='%d', old='%d'.", level_stats.stars, saved_stats.stars))
			saved_stats.stars = level_stats.stars
		end
		if level_stats.score > saved_stats.highest_score then
			print(string.format("player refreshes score: new='%d', old='%d'.", level_stats.score, saved_stats.highest_score))
			saved_stats.highest_score = level_stats.score
		end
		if level_stats.time_spent < saved_stats.time_spent then
			print(string.format("player refreshes time_spent: new='%.2f', old='%.2f'.", level_stats.time_spent, saved_stats.time_spent))
			saved_stats.stars = level_stats.stars
		end
		player.info.level_stats[tostring(player.current_level)] = saved_stats
	end

	player.refresh_ui()
end

function player.refresh_ui()
	bt_native.set_player_property(BTPL_Level, player.info[BTPL_Level], false)
	bt_native.set_player_property(BTPL_Exp, player.info[BTPL_Exp], false)
	bt_native.set_player_property(BTPL_Coins, player.info[BTPL_Coins], false)
	bt_native.set_player_property(BTPL_Diamonds, player.info[BTPL_Diamonds], false)
	bt_native.set_player_property(BTPL_Energy, player.info[BTPL_Energy], true)
end

return player

