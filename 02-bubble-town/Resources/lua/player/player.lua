
player = {}
player.status = require "player_status"
player.statusV2 = require "statusV2"
player.level_stats = require "level_stats"

local _archiving = require "archiving"
_archiving.register("level_stats", player.level_stats.save, player.level_stats.load)
_archiving.register("status", player.statusV2.save, player.statusV2.load)

function player.save()
	_archiving.savefile(TEST_PLAYER_PROFILE_V2)

	return player.status.save()
end

function player.load(profileName)
	_archiving.loadfile(TEST_PLAYER_PROFILE_V2)
	return player.status.load(profileName)
end

function player.getNextLevel()
	return player.status.getNextLevel()
end

function player.getCurrentLevel()
	return player.status.getCurrentLevel()
end

function player.setCurrentLevel(currentLevel)
	player.status.setCurrentLevel(currentLevel)
end

function player.onLevelCompleted(level_rewards, level_stats)
	print(string.format("player completes level '%d'.", player.status.current_level))
	print(string.format("player gets rewards: coins=%d, exp=%d, heroes=%d.", level_rewards.coins, level_rewards.exp, level_rewards.heroes))
	print(string.format("level_stats: stars=%d, score=%d, time_spent=%.2f.", level_stats.stars, level_stats.score, level_stats.time_spent))

	player.status.onLevelCompleted(level_rewards, level_stats)

	player.statusV2.addCoins(level_rewards.coins)
	player.statusV2.addExp(level_rewards.exp)

	player.level_stats.refresh(player.status.getCurrentLevel(), level_stats)
end

function player.refresh_ui()
	player.status.refresh_ui()
end

return player

