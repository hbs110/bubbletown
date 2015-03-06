
require "g_util"

ts_player = {}
ts_player.status = require "status"

local function test_exp_and_level() 
	-- checking initial values
	local lv = ts_player.status.getAt(BTPL_Level)
	local exp = ts_player.status.getAt(BTPL_Exp)
	assert(lv == 1, "incorrect initial value of player 'exp'.")
	assert(exp == 0, "incorrect initial value of player 'exp'.")

	-- simulating 'Level Up' event
	ts_player.status.addExp(50)
	lv = ts_player.status.getAt(BTPL_Level)
	assert(lv == 2, "exp is given but no level up triggered.")
end

function ts_player.run()
	test_exp_and_level()
end

return ts_player
