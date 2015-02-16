
require "def"

core = require "core"
util = require "util"

game_sanity = require "game_sanity"

-- install components
game = {}
game.simulation = require "simulation"
game.player = require "player"

game.simulation.handlers.player = game.player

-- test vars
TEST_PLAYER_PROFILE = "test"


function game.init() 
	core.checkpoint("game.init() started.")

	game_sanity.check_env()

	util.goto_scene(BTSCN_start)

	if not game.player.load(TEST_PLAYER_PROFILE) then
		core.log_err("Player profile loading failed, possibly broken.")
		return false
	end

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

