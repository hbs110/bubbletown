
require "def"
require "g_constants"

core = require "core"
util = require "util"

game_sanity = require "game_sanity"
level_handlers = require "level_handlers"
ui_handlers = require "ui_handlers"

game = {}

-- installed components
game.simulation = require "simulation"
game.player = require "player"


function game.init() 
	core.checkpoint("game.init() started.")

	game_sanity.check_env()

	-- init components
	level_handlers.init(game.player)
	ui_handlers.init(game.simulation)

	-- registering handlers
	game.simulation.register_handlers(level_handlers.handlerSet)
	for _,v in ipairs(ui_handlers.handlerSet) do
		game.simulation.register_ui_handler(v.scene, v.ctrl, v.msgID, v.handler)
	end

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

