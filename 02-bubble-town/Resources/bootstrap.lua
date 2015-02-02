
t_buildings = {}
t_buildings["hall"]         = { arrawScale = 0.4, sizeInTile = { x = 3, y = 3 } }
t_buildings["deco"]         = { arrawScale = 0.2, sizeInTile = { x = 1, y = 1 } }
t_buildings["house"]        = { arrawScale = 0.3, sizeInTile = { x = 2, y = 2 } }
t_buildings["shop"]         = { arrawScale = 0.2, sizeInTile = { x = 1, y = 1 } }
t_buildings["workshop"]     = { arrawScale = 0.2, sizeInTile = { x = 1, y = 1 } }

function get_building_image(name) 		return "elements/"..name..".png" end
function get_building_arrowScale(name) 	return t_buildings[name].arrawScale end
function get_building_width(name) 		return t_buildings[name].sizeInTile.x end
function get_building_height(name) 		return t_buildings[name].sizeInTile.y end

