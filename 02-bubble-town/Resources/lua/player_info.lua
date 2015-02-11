
player_info = {}

player_info.level = 1
player_info.exp = 0
player_info.coins = 0
player_info.diamonds = 0
player_info.energy = 5


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
	player_info.heros["name"] = { color=red, level=1, exp=0 }
]] 

player_info.heros = {} 



--[[ level stats
sample: 
	player_info.level_stats["level_1"] = { stars=1, time_spent=12.0, highest_score=10000 }
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


