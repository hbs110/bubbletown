
package.path=package.path
	..';lua/external/JSON/?.lua'
	..';lua/core/?.lua'
	..';lua/core/g_tab/?.lua'
	..';lua/player/?.lua'
	..';lua/simulation/?.lua'
	..';lua/game/?.lua'
	..';lua/?.lua'

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

-- UI 刷新
hostcall_refresh_ui = game.player.refresh_ui

