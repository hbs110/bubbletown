
require "def"


--[[
    关卡的数据结构注意以下几点：

    1. 注意维持简单性，尽量避免分表和二次索引，便于日后可能的自动化生成
    2. 注意保持描述性，确保人能很快看懂并容易理解

    说明：
    - 注意关卡 ID 是手动填写的。自动生成的话，会有在维护过程中可能发生变化的隐患。
    - 教学关卡每关奖励一个英雄，后面每个阶段奖励一个英雄
]]

t_level_main_story = 
{
    -- tutorial levels
    { id = 101, name = "_", group = "tutorial",     tag = "", rewards = { coins = 100, exp = 10, hero = 1 } },
    { id = 102, name = "_", group = "tutorial",     tag = "", rewards = { coins = 100, exp = 10, hero = 1 } },
    { id = 103, name = "_", group = "tutorial",     tag = "", rewards = { coins = 100, exp = 10, hero = 1 } },

    -- bring-it-on
    { id = 201, name = "_", group = "bring-it-on",  tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 202, name = "_", group = "bring-it-on",  tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 203, name = "_", group = "bring-it-on",  tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 204, name = "_", group = "bring-it-on",  tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 205, name = "_", group = "bring-it-on",  tag = "", rewards = { coins = 100, exp = 10, hero = 1 } },

    -- hardcore
    { id = 301, name = "_", group = "hardcore",     tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 302, name = "_", group = "hardcore",     tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 303, name = "_", group = "hardcore",     tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 304, name = "_", group = "hardcore",     tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 305, name = "_", group = "hardcore",     tag = "", rewards = { coins = 100, exp = 10, hero = 1 } },

    -- nightmare
    { id = 401, name = "_", group = "nightmare",    tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 402, name = "_", group = "nightmare",    tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 403, name = "_", group = "nightmare",    tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 404, name = "_", group = "nightmare",    tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 405, name = "_", group = "nightmare",    tag = "", rewards = { coins = 100, exp = 10, hero = 1 } },

    -- epic
    { id = 501, name = "_", group = "epic",         tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 502, name = "_", group = "epic",         tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 503, name = "_", group = "epic",         tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 504, name = "_", group = "epic",         tag = "", rewards = { coins = 100, exp = 10 } },
    { id = 505, name = "_", group = "epic",         tag = "", rewards = { coins = 100, exp = 10, hero = 1 } },
}

--[[
    这里是所有的副本定义
    'locked_by_level' 是前置关卡的 id，也就是说必须打过了某个关卡才能开启此副本
]]
t_level_instances = 
{
    { id = 10001, name = "inst_a", rewards = { coins = 100, exp = 10 }, locked_by_level = 203 },
    { id = 10002, name = "inst_b", rewards = { coins = 100, exp = 10 }, locked_by_level = 303 },
    { id = 10003, name = "inst_c", rewards = { coins = 100, exp = 10 }, locked_by_level = 403 },
}


