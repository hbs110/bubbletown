
package.path=package.path
	..';lua/?.lua'
	..';lua/JSON/?.lua'
	..';lua/shared/?.lua'
	..';lua/core/?.lua'
	..';lua/tab/?.lua'
	..';lua/player/?.lua'
	..';lua/simulation/?.lua'

-- 数据表
require "tab_buildings"

-- 主逻辑入口
game = require "game"

-- 注册一系列系统主入口点
hostcall_init = game.init
hostcall_tick = game.tick
hostcall_destroy = game.destroy

-- 消息处理
hostcall_post = game.simulation.inbox.post
hostcall_flush = game.simulation.process_messages

-- 同步调用的 lua 函数
function get_building_image(name) 		return "elements/"..name..".png" end
function get_building_arrowScale(name) 	return t_buildings[name].arrawScale end
function get_building_width(name) 		return t_buildings[name].sizeInTile.x end
function get_building_height(name) 		return t_buildings[name].sizeInTile.y end
