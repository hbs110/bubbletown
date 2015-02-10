
dofile "lua/core.lua"
dofile "lua/scenes.lua"

core = dofile "lua/core.lua"

game = {}

game.inbox = dofile "lua/game_inbox.lua"
game.player = dofile "lua/player.lua"

TEST_PLAYER_PROFILE = "test"

function game.init() 
	core.checkpoint("game.init() started.")

	if not game.player.load(TEST_PLAYER_PROFILE) then
		core.log_err("Player profile loading failed, possibly broken.")
		return false
	end

	goto_scene("scn_start")
	core.checkpoint("game.init() done.")
	return true
end

function game.tick(deltaSeconds) 
	-- core.checkpoint(string.format("game.tick(%.3f).", deltaSeconds))
end

function game.destroy() 
	core.checkpoint("game.destroy() started.")

	--[[ the saving procedure should be much more reliable than it looks now, including:
		- error-handling while saving
		- broken content
		
		however, we *should* save the profile onto server later, would revisit here then. 
	--]]

	if game.player.save() then
		print("player profile saved.")
	end

	core.checkpoint("game.destroy() done.")
end

return game

