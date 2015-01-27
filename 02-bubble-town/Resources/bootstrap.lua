
foo = "bar"

t_buildings = {}
t_buildings["hall"]         = { arrawScale = 0.4, arrawDist = 2 }
t_buildings["deco"]         = { arrawScale = 0.4, arrawDist = 2 }
t_buildings["house"]        = { arrawScale = 0.4, arrawDist = 2 }
t_buildings["shop"]         = { arrawScale = 0.4, arrawDist = 2 }
t_buildings["workshop"]     = { arrawScale = 0.4, arrawDist = 2 }

-- function get_building_names()
--     local names = {}
--     for i, v in ipairs(t) do 
--         table.insert(names, i)
--     end
--     return names
-- end

-- function get_building_image(name)
--     print get_building_names()

--     local imageEntry = t_buildings[name]
--     if imageEntry == nil then
--         return nil
--     end

--     return "elements/"..name..".png"
-- end

