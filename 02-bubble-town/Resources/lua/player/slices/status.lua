
require "g_shared"

status = {}

local _data = {}
_data[BTPL_Level] = 1
_data[BTPL_Exp] = 0
_data[BTPL_Coins] = 0
_data[BTPL_Diamonds] = 0
_data[BTPL_Energy] = 5

function status.getAt(propName)
	return _data[propName]
end

function status.addCoins(count)
	_data[BTPL_Coins] = _data[BTPL_Coins] + count
end

function status.subCoins(count)
	_data[BTPL_Coins] = _data[BTPL_Coins] - count
end

function status.addExp(count)
	_data[BTPL_Exp] = _data[BTPL_Exp] + count

	if _data[BTPL_Exp] > 20 then
		status.levelUp()

		_data[BTPL_Exp] = _data[BTPL_Exp] - 20
		print(string.format("player exp changed to '%d'.", _data[BTPL_Exp]))
	end
end

function status.levelUp()
	_data[BTPL_Level] = _data[BTPL_Level] + 1
	print(string.format("player level up! (now '%d')", _data[BTPL_Level]))
end

function status.load(tbl)
	for k,v in pairs(tbl) do
		_data[k] = v
	end
	return true
end

function status.save()
	return _data
end

return status