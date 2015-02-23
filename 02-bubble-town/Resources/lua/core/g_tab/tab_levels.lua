
require "g_shared"

BTL_Storytag = "story"
BTL_InstTag = "inst"
BTL_InstBase = 10000

--[[
    关卡的数据结构注意以下几点：

    1. 注意维持简单性，尽量避免分表和二次索引，便于日后可能的自动化生成
    2. 注意保持描述性，确保人能很快看懂并容易理解

    说明：
    - 注意关卡 ID 是手动填写的。自动生成的话，会有在维护过程中可能发生变化的隐患。
    - 教学关卡每关奖励一个英雄，后面每个阶段奖励一个英雄
]]

t_levels = {}

-- tutorial levels
t_levels[101] = { name = "_", group = "tutorial",     tag = BTL_StoryTag, rewards = { coins = 100, exp = 10, heroes = 1 } }
t_levels[102] = { name = "_", group = "tutorial",     tag = BTL_StoryTag, rewards = { coins = 100, exp = 10, heroes = 1 } }
t_levels[103] = { name = "_", group = "tutorial",     tag = BTL_StoryTag, rewards = { coins = 100, exp = 10, heroes = 1 } }

-- bring-it-on
t_levels[201] = { name = "_", group = "bring-it-on",  tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[202] = { name = "_", group = "bring-it-on",  tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[203] = { name = "_", group = "bring-it-on",  tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[204] = { name = "_", group = "bring-it-on",  tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[205] = { name = "_", group = "bring-it-on",  tag = BTL_StoryTag, rewards = { coins = 100, exp = 10, heroes = 1 } }

-- hardcore
t_levels[301] = { name = "_", group = "hardcore",     tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[302] = { name = "_", group = "hardcore",     tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[303] = { name = "_", group = "hardcore",     tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[304] = { name = "_", group = "hardcore",     tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[305] = { name = "_", group = "hardcore",     tag = BTL_StoryTag, rewards = { coins = 100, exp = 10, heroes = 1 } }

-- nightmare
t_levels[401] = { name = "_", group = "nightmare",    tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[402] = { name = "_", group = "nightmare",    tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[403] = { name = "_", group = "nightmare",    tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[404] = { name = "_", group = "nightmare",    tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[405] = { name = "_", group = "nightmare",    tag = BTL_StoryTag, rewards = { coins = 100, exp = 10, heroes = 1 } }

-- epic
t_levels[501] = { name = "_", group = "epic",         tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[502] = { name = "_", group = "epic",         tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[503] = { name = "_", group = "epic",         tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[504] = { name = "_", group = "epic",         tag = BTL_StoryTag, rewards = { coins = 100, exp = 10 } }
t_levels[505] = { name = "_", group = "epic",         tag = BTL_StoryTag, rewards = { coins = 100, exp = 10, heroes = 1 } }

--[[
    这里是所有的副本定义
    'locked_by_level' 是前置关卡的 id，也就是说必须打过了某个关卡才能开启此副本
]]
t_levels[BTL_InstBase + 1] = { name = "inst_a",       tag = BTL_InstTag, rewards = { coins = 100, exp = 10 }, locked_by_level = 203 }
t_levels[BTL_InstBase + 2] = { name = "inst_b",       tag = BTL_InstTag, rewards = { coins = 100, exp = 10 }, locked_by_level = 303 }
t_levels[BTL_InstBase + 3] = { name = "inst_c",       tag = BTL_InstTag, rewards = { coins = 100, exp = 10 }, locked_by_level = 403 }

-- id list
t_level_id_list = {}
for k,_ in pairs(t_levels) do
    table.insert(t_level_id_list, tonumber(k))
end
table.sort(t_level_id_list)


function level_is_instance(levelID) return string.find(t_levels[levelID].tag, BTL_InstTag) end

-- rewards
function level_get_reward_coins(levelID)
    if t_levels[levelID] ~= nil then return t_levels[levelID].rewards.coins else return nil end
end
function level_get_reward_exp(levelID)
    if t_levels[levelID] ~= nil then return t_levels[levelID].rewards.exp else return nil end
end
function level_get_reward_hero(levelID)
    if t_levels[levelID] ~= nil then return t_levels[levelID].rewards.heroes else return nil end
end

