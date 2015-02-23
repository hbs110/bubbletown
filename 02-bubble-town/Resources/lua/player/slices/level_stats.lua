
require "g_constants"
require "g_util"

--[[ level stats
sample: 
	_data[102] = { stars=1, time_spent=12.0, highest_score=10000 }
]] 

level_stats = {}  

local _data = {}

function level_stats.load(tbl)
	for k,v in pairs(tbl) do
		local levelID = tonumber(k)
		if levelID == nil then
			return false
		end
		_data[levelID] = v
	end
	--g_print_table_pretty(_data)
	return true
end

function level_stats.save()
	--g_print_table_pretty(_data)
	local stringIndexedTable = {}
	for k,v in pairs(_data) do
		stringIndexedTable[tostring(k)] = v
	end
	--g_print_table_pretty(stringIndexedTable)
	return stringIndexedTable
end

function level_stats.getFarestLevelID()
	local farest = BT_Invalid
	for k,v in pairs(_data) do
		if k > farest then
			farest = k
		end
	end
	return farest
end

function level_stats.getAt(levelID)
	return _data[levelID]
end

function level_stats.refresh(levelID, levelData)
	local saved = _data[levelID]
	if saved == nil then
		print(string.format("player completes level '%d' for the first time.", levelID))
		_data[levelID] = { stars=levelData.stars, highest_score=levelData.score, time_spent=levelData.time_spent }
	else
		if levelData.stars > saved.stars then
			print(string.format("player refreshes star count: new='%d', old='%d'.", levelData.stars, saved.stars))
			saved.stars = levelData.stars
		end
		if levelData.score > saved.highest_score then
			print(string.format("player refreshes score: new='%d', old='%d'.", levelData.score, saved.highest_score))
			saved.highest_score = levelData.score
		end
		if levelData.time_spent < saved.time_spent then
			print(string.format("player refreshes time_spent: new='%.2f', old='%.2f'.", levelData.time_spent, saved.time_spent))
			saved.time_spent = levelData.time_spent
		end
	end
end

return level_stats