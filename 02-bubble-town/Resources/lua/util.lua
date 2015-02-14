
util = {}

function util.goto_scene(sceneName, sceneConfig) 
	local config = sceneConfig
	if config == nil then
		config = ""
	end

	print(string.format("goto_scene('%s', '%s')", sceneName, config))
	bt_native.goto_scene(sceneName, config)
end

return util
