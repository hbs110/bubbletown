
JSON = require "JSON" 

require "tab_levels" 
require "g_constants"

status = {}

status.info = require "player_profile"
status.profile_name = DEFAULT_PLAYER_PROFILE
status.current_level = nil

function status.save()
	if status.profile_name == DEFAULT_PLAYER_PROFILE then
		print("profile not been correctly initialized, saving request ignored.")
		return true
	end

	local profilePath = g_build_profile_path(status.profile_name)
	if not g_save_json(profilePath, status.info) then
		return false
	end

	print(string.format("profile saved into '%s'.", profilePath))
	return true
end

function status.load(profileName)
	local data = g_load_json(g_build_profile_path(profileName))
	if data == nil then 
		return false
	end

	-- merge the loaded profile into status.info
	for k,v in pairs(data) do
		status.info[k] = v 
	end

	-- print("status.info.[post loading]")
	-- g_print_table_pretty(status.info)

	status.profile_name = profileName
	g_checkpoint(string.format("player profile ('%s') loaded.", status.profile_name))
	return true
end

function status.getNextLevel()
	local curLevel = BT_Invalid
	if status.current_level ~= nil then
		curLevel = status.current_level
	else
		for k,v in pairs(status.info.level_stats) do
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

function status.getCurrentLevel()
	return status.current_level
end

function status.setCurrentLevel(currentLevel)
	status.current_level = currentLevel
	if status.current_level ~= nil then
		print(string.format("player is now in level '%d'.", status.current_level))
	else
		print("player is now out of level.")
	end
end

function status.onLevelCompleted(level_rewards, level_stats)
	print(string.format("player completes level '%d'.", status.current_level))
	print(string.format("player gets rewards: coins=%d, exp=%d, heroes=%d.", level_rewards.coins, level_rewards.exp, level_rewards.heroes))
	print(string.format("level_stats: stars=%d, score=%d, time_spent=%.2f.", level_stats.stars, level_stats.score, level_stats.time_spent))

	-- apply rewards
	status.info[BTPL_Coins] = status.info[BTPL_Coins] + level_rewards.coins
	status.info[BTPL_Exp] = status.info[BTPL_Exp] + level_rewards.exp
	print(string.format("player info (coins='%d', exp='%d')", status.info[BTPL_Coins], status.info[BTPL_Exp]))

	if status.info[BTPL_Exp] > 20 then
		status.info[BTPL_Level] = status.info[BTPL_Level] + 1
		print(string.format("player level up! (now '%d')", status.info[BTPL_Level]))

		status.info[BTPL_Exp] = status.info[BTPL_Exp] - 20
		print(string.format("player exp changed to '%d'.", status.info[BTPL_Exp]))
	end

	-- refreshing level stats
	local saved_stats = status.info.level_stats[tostring(status.current_level)]
	if saved_stats == nil then
		print(string.format("player completes level '%d' for the first time.", status.current_level))
		status.info.level_stats[tostring(status.current_level)] = { stars=level_stats.stars, highest_score=level_stats.score, time_spent=level_stats.time_spent }
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
		status.info.level_stats[tostring(status.current_level)] = saved_stats
	end

	status.refresh_ui()
end

function status.refresh_ui()
	bt_native.set_player_property(BTPL_Level, status.info[BTPL_Level], false)
	bt_native.set_player_property(BTPL_Exp, status.info[BTPL_Exp], false)
	bt_native.set_player_property(BTPL_Coins, status.info[BTPL_Coins], false)
	bt_native.set_player_property(BTPL_Diamonds, status.info[BTPL_Diamonds], false)
	bt_native.set_player_property(BTPL_Energy, status.info[BTPL_Energy], true)
end

return status

