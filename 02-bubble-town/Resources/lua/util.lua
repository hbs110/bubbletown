
util = {}

function util.goto_scene(sceneName) 
	print(string.format("goto_scene('%s')", sceneName))
	bt_native.goto_scene(sceneName)
end

return util

-- -- start scene ui handlers
-- bt_native.set_scene_named_handler("scn_start", "Bubble", function () goto_scene("scn_bubble") end )
-- bt_native.set_scene_named_handler("scn_start", "Town", function () goto_scene("scn_town") end )
