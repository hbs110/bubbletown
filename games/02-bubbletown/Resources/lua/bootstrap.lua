
-- injected print
_G.print = bt_native.print

package.path = package.path..require "packages"

-- debugging output
-- print(tostring(package.path))
-- print(tostring(package.searchers))

-- 数据表
require "tab_exp"
require "tab_buildings"

-- 快速框架
require "def_combat"

-- 主逻辑入口
game = require "game"

-- 注册一系列系统主入口点
hostcall_init = game.init
hostcall_tick = game.tick
hostcall_destroy = game.destroy

-- 消息处理
g_post = game.simulation.inbox.post
hostcall_post = g_post	-- two-phases assignment, 'g_post' is called by lua and 'hostcall_post' is called by cpp
hostcall_flush = game.simulation.process_messages

-- UI 刷新
hostcall_refresh_ui = game.player.refresh_ui

