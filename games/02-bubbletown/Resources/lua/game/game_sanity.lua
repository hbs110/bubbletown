
require "g_util"

game_sanity = {}

function game_sanity.check_env() 
	local paths = g_split(package.path, ';')
	print("package paths:")
	for _, s in ipairs(paths) do
	    print("  "..s)
	end
end

function game_sanity.check_post_init()
	if g_post == nil then
		error("'g_post' hasn't been set.")
	end
end

return game_sanity
