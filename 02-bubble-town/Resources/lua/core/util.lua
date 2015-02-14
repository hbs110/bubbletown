
util = {}

function util.goto_scene(sceneName, sceneConfig) 
	local config = sceneConfig
	if config == nil then
		config = ""
	end

	print(string.format("goto_scene('%s', '...')", sceneName))
	bt_native.goto_scene(sceneName, config)
end

function util.split(s, p)
    local rt= {}
    string.gsub(s, '[^'..p..']+', function(w) table.insert(rt, w) end )
    return rt
end

return util
