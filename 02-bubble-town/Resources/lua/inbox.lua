
inbox = {}

inbox.msgs = {}

-- I don't know how to call var_args lua functions from C++ directly in luabridge
-- up to 9 parameters looks enough for cross language invoking
-- please also note the hidden parameter 'arg' is deprecated in Lua 5.2, use {...} instead.
function inbox.post(msgID, msgInfo, p1, p2, p3, p4, p5, p6, p7, p8, p9)
	local arg={p1, p2, p3, p4, p5, p6, p7, p8, p9}
	-- print("inbox.post() called. id: "..msgID.." info: "..msgInfo.." arg_count: "..#arg)
	table.insert(inbox.msgs, {id=msgID, info=msgInfo, args=arg}) 
end

function inbox.is_empty()
	return next(inbox.msgs) == nil
end

-- fetch the first msg for later processing if there is one
function inbox.fetch()
	if inbox.is_empty() then
		return nil
	end

	return table.remove(inbox.msgs, 1)
end

function inbox.dbg_print()
	for i,v in ipairs(inbox.msgs) do
		print("inbox.post() called. id: "..v.id.." info: "..v.info.." arg_count: "..#v.args)
	end
end

return inbox

