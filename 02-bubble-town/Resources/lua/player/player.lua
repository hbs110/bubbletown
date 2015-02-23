
player = {}
player.status = require "player_status"

function player.save()
	return player.status.save()
end

function player.load(profileName)
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
end

function player.refresh_ui()
	player.status.refresh_ui()
end

return player

