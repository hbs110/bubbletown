
require "g_shared"
require "g_constants"
require "g_util"

require "tab_exp"

game_sanity = require "game_sanity"
level_handlers = require "level_handlers"
ui_handlers = require "ui_handlers"

game = {}

-- installed components
game.simulation = require "simulation"
game.player = require "player"

function game.init() 
	g_checkpoint("game.init() started.")

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
		g_log_err("Player profile loading failed, start a new profile.")
	end

	game_sanity.check_post_init()
	
	g_goto_scene(BTSCN_start)

	-- testing/debugging code
	-- g_log_dbg("max_level: "..get_max_level())
	-- g_log_dbg("level 1 exp: "..get_level_exp(1))
	-- local content = bt_native.load_string_from_file("users/default.json");
	-- g_log_dbg("default.json size: "..#content)

	g_checkpoint("game.init() done.")
	return true
end

function game.tick(deltaSeconds) 
	-- g_checkpoint(string.format("game.tick(%.3f).", deltaSeconds))
	game.simulation.tick(deltaSeconds)
end

function game.destroy() 
	g_checkpoint("game.destroy() started.")

	--[[ the saving procedure should be much more reliable than it looks now, including:
		- error-handling while saving
		- broken content
		
		however, we *should* save the profile onto server later, would revisit here then. 
	--]]

	if not game.player.save() then
		g_log_err("player profile saving failed.")
	end

	g_checkpoint("game.destroy() done.")
end

return game

