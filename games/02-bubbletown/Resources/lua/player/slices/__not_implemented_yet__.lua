require "g_shared"

not_implemented_yet = {}

--[[ buildings
sample: 
	{ id=1, name="name", level=1, pos={x=-1,y=-1}, heros={"foo", "bar"} }
]] 

not_implemented_yet.buildings = {} 



--[[ items
sample: 
	not_implemented_yet.items[id] = { count=1 }
]] 

not_implemented_yet.items = {} 



--[[ heros
sample: 
	not_implemented_yet.heroes["name"] = { color=red, level=1, exp=0 }
]] 

not_implemented_yet.heroes = {} 


--[[ in progress events
sample: 
	{ type=build, 	start_time="yyyy-mm-dd:HH-MM-SS", building_id=1 }
	{ type=upgrade, start_time="yyyy-mm-dd:HH-MM-SS", building_id=1 }
	{ type=make, 	start_time="yyyy-mm-dd:HH-MM-SS", heros="foo" }
]] 

not_implemented_yet.in_progress_events = {}  

return not_implemented_yet


