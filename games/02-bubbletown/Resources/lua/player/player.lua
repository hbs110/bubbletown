
require "tab_levels"

player = {}
player.status = require "status"
player.level_stats = require "level_stats"

local _archiving = require "archiving"
_archiving.register("level_stats", player.level_stats.save, player.level_stats.load)
_archiving.register("status", player.status.save, player.status.load)

local _currentLevel = nil

function player.save()
	return _archiving.savefile()
end

function player.load(profileName)
	return _archiving.loadfile(profileName)
end

function player.getNextLevel()
	local curLevel = _currentLevel or player.level_stats.getFarestLevelID()
	if curLevel == nil then
		return level_get_first()
	end
	return level_get_next(curLevel) 
end

function player.getCurrentLevel()
	return _currentLevel
end

function player.setCurrentLevel(currentLevel)
	_currentLevel = currentLevel
	if _currentLevel ~= nil then
		print(string.format("player is now in level '%d'.", _currentLevel))
	else
		print("player is now out of level.")
	end
end

function player.onLevelCompleted(level_rewards, level_stats)
	print(string.format("player completes level '%d'.", _currentLevel))
	print(string.format("player gets rewards: coins=%d, exp=%d, heroes=%d.", level_rewards.coins, level_rewards.exp, level_rewards.heroes))
	print(string.format("level_stats: stars=%d, score=%d, time_spent=%.2f.", level_stats.stars, level_stats.score, level_stats.time_spent))

	player.status.addCoins(level_rewards.coins)
	player.status.addExp(level_rewards.exp)

	player.level_stats.refresh(_currentLevel, level_stats)
end

function player.refresh_ui()
	bt_native.set_player_property(BTPL_Level, player.status.getAt(BTPL_Level), false)
	bt_native.set_player_property(BTPL_Exp, player.status.getAt(BTPL_Exp), false)
	bt_native.set_player_property(BTPL_Coins, player.status.getAt(BTPL_Coins), false)
	bt_native.set_player_property(BTPL_Diamonds, player.status.getAt(BTPL_Diamonds), false)
	bt_native.set_player_property(BTPL_Energy, player.status.getAt(BTPL_Energy), true)
end

return player

