

handlers = {}

function OnMsg_GotoScene(msg) 
	util.goto_scene(msg.info)
end

-- register handlers
handlers[BtMsgID.GotoScene] = OnMsg_GotoScene 
return handlers

