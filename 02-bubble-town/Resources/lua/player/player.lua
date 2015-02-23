
player = {}
player.status = require "player_status"
player.level_stats = require "level_stats"

player.archiving = require "archiving"
player.archiving.register("level_stats", player.level_stats.save, player.level_stats.load)

function player.save()
	archiving.savefile(TEST_PLAYER_PROFILE_V2)

	return player.status.save()
end

function player.load(profileName)
	archiving.loadfile(TEST_PLAYER_PROFILE_V2)
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
	player.status.onLevelCompleted(level_rewards, level_stats)

	player.level_stats.refresh(player.status.getCurrentLevel(), level_stats)
end

function player.refresh_ui()
	player.status.refresh_ui()
end

return player

