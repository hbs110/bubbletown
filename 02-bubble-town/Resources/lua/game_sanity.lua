
require "util"

game_sanity = {}

function game_sanity.check_env() 
	local paths = util.split(package.path, ';')
	print("package paths:")
	for _, s in ipairs(paths) do
	    print("  "..s)
	end
end

return game_sanity
