
package.path=package.path
	..';lua/?.lua'
	..';lua/JSON/?.lua'
	..';lua/shared/?.lua'

-- 数据表
require "tab_buildings"

-- 主逻辑入口
game = require "game"

-- 注册一系列系统主入口点
hostcall_init = game.init
hostcall_tick = game.tick
hostcall_destroy = game.destroy

hostcall_post = game.simulation.inbox.post
hostcall_flush = game.simulation.process_messages

