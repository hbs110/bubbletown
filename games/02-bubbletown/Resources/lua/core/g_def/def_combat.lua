
function get_combat_res_path(filename) 	return "combat/"..filename end
function get_combat_background() 		return get_combat_res_path("background.jpg") end
function get_combat_vs() 				return get_combat_res_path("vs.png") end
function get_combat_card_path(num) 		return get_combat_res_path(string.format("placeholder_%d.jpg", num)) end
