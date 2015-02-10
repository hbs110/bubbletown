
t_scenes = {
	scn_start 	=	{ name = "StartScene" },
	scn_bubble	=	{ name = "BubbleScene" },
	scn_town	=	{ name = "TownScene" },
	scn_world	=	{ name = "WorldScene" },
}

function get_scene_name(id) 
	local scn = t_scenes[id]
	if scn ~= nil then 
		return scn.name 
	else
		return nil
	end
end

function goto_scene(id) 
	print(string.format("goto_scene('%s')", id))
	bt_native.goto_scene(get_scene_name(id))
end

-- -- start scene ui handlers
-- bt_native.set_scene_named_handler("scn_start", "Bubble", function () goto_scene("scn_bubble") end )
-- bt_native.set_scene_named_handler("scn_start", "Town", function () goto_scene("scn_town") end )
