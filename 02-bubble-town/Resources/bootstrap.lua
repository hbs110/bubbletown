
t_buildings = {}
t_buildings["hall"]         = { arrawScale = 0.4, arrawDist = 2 }
t_buildings["deco"]         = { arrawScale = 0.4, arrawDist = 2 }
t_buildings["house"]        = { arrawScale = 0.4, arrawDist = 2 }
t_buildings["shop"]         = { arrawScale = 0.4, arrawDist = 2 }
t_buildings["workshop"]     = { arrawScale = 0.4, arrawDist = 2 }

function get_building_image(name)
    if t_buildings[name] == nil then
        return nil
    else
        return "elements/"..name..".png"
    end
end

function get_building_arrowScale(name) return t_buildings[name].arrawScale end
function get_building_arrowDist(name) return t_buildings[name].arrawDist end
