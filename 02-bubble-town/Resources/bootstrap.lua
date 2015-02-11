
-- 数据表
dofile "lua/tab_buildings.lua"

-- 主逻辑入口
game = dofile "lua/game.lua"

-- 注册一系列系统主入口点
hostcall_init = game.init
hostcall_tick = game.tick
hostcall_destroy = game.destroy

hostcall_post = game.simulation.inbox.post
hostcall_flush = game.simulation.process_messages

