
-- 数据表
dofile "lua/tab_buildings.lua"

-- 主逻辑入口
game = dofile "lua/game.lua"

-- 注册一系列系统主入口点。注意，这些 hostcall 开头的函数由 Host (C++) 决定调用时机
hostcall_init = game.init
hostcall_tick = game.tick
hostcall_destroy = game.destroy



