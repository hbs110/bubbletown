
dofile "lua/scenes.lua"

game = {}
game.inbox = dofile "lua/game_inbox.lua"

function game.init() 
	print("game.init().")

	goto_scene("scn_start");
	return true
end

function game.tick(deltaSeconds) 
	print(string.format("game.tick(%.3f).", deltaSeconds))
end

function game.destroy() 
	print("game.destroy().")
end

return game

