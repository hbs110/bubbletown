
dofile "lua/shared/def.lua"

core = dofile "lua/core.lua"
util = dofile "lua/util.lua"


-- install components
game = {}
game.simulation = dofile "lua/simulation.lua"
game.player = dofile "lua/player.lua"

game.simulation.handlers.player = game.player

-- test vars
TEST_PLAYER_PROFILE = "test"


function game.init() 
	core.checkpoint("game.init() started.")

	if not game.player.load(TEST_PLAYER_PROFILE) then
		core.log_err("Player profile loading failed, possibly broken.")
		return false
	end

	util.goto_scene(BTSCN_start)
	core.checkpoint("game.init() done.")
	return true
end

function game.tick(deltaSeconds) 
	-- core.checkpoint(string.format("game.tick(%.3f).", deltaSeconds))
	game.simulation.tick(deltaSeconds)
end

function game.destroy() 
	core.checkpoint("game.destroy() started.")

	--[[ the saving procedure should be much more reliable than it looks now, including:
		- error-handling while saving
		- broken content
		
		however, we *should* save the profile onto server later, would revisit here then. 
	--]]

	if not game.player.save() then
		print("player profile saving failed.")
	end

	core.checkpoint("game.destroy() done.")
end

return game

