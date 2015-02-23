require "g_def_shared"

player_info = {}

player_info[BTPL_Level] = 1
player_info[BTPL_Exp] = 0
player_info[BTPL_Coins] = 0
player_info[BTPL_Diamonds] = 0
player_info[BTPL_Energy] = 5


--[[ buildings
sample: 
	{ id=1, name="name", level=1, pos={x=-1,y=-1}, heros={"foo", "bar"} }
]] 

player_info.buildings = {} 



--[[ items
sample: 
	player_info.items[id] = { count=1 }
]] 

player_info.items = {} 



--[[ heros
sample: 
	player_info.heroes["name"] = { color=red, level=1, exp=0 }
]] 

player_info.heroes = {} 



--[[ level stats
sample: 
	player_info.level_stats[101] = { stars=1, time_spent=12.0, highest_score=10000 }
]] 

player_info.level_stats = {}  



--[[ in progress events
sample: 
	{ type=build, 	start_time="yyyy-mm-dd:HH-MM-SS", building_id=1 }
	{ type=upgrade, start_time="yyyy-mm-dd:HH-MM-SS", building_id=1 }
	{ type=make, 	start_time="yyyy-mm-dd:HH-MM-SS", heros="foo" }
]] 

player_info.in_progress_events = {}  

return player_info


