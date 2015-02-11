

handlers_ui = {}

function handlers_ui.is_ui_msg(msg)
    return msg.id == BtMsgID.UI_ButtonPressed
end

function handlers_ui.execute(msg)
	local k = msg.info.."|"..msg.args[1].."|"..msg.id
	local handler = handlers_ui[k]
	if handler ~= nil then
		handler(msg)
	end
end

function on_bubble_loot_pressed(msg)
	util.goto_scene(BTSCN_town)
end

function reg_ui_handler(sceneName, ctrlName, msgID, handlerFunc)
	local k = sceneName.."|"..ctrlName.."|"..msgID
	handlers_ui[k]	= handlerFunc
end

-- register handlers
reg_ui_handler(BTSCN_bubble, "Loot", BtMsgID.UI_ButtonPressed, on_bubble_loot_pressed)

return handlers_ui

