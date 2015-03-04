-- this library is found here
--      http://regex.info/blog/lua/json
JSON = require "JSON" 

require "g_constants"

g_post = nil    -- should be assembled

function g_checkpoint(name)
    print(string.format("[CP] '%s' reached at '%.3f'.", name, bt_native.get_current_time()))
end 

function g_log(fmt, ...)
    print(string.format(fmt, ...))
end 

function g_log_err(info)
    print(string.format("[ERROR] '%s' at '%.3f'.", info, bt_native.get_current_time()))
end 

function g_log_dbg(info)
    print(string.format("[DEBUG] '%s' at '%.3f'.", info, bt_native.get_current_time()))
end 

function g_build_profile_path(profileName)
    return PLAYER_PROFILE_DIR..profileName..".json"
end

function g_load_json_from_string(str)
    local succ, ret = pcall(function () return JSON:decode(str) end)
    if not succ then 
        g_log_err(string.format("Decoding Json string failed. (%s)", ret))
        return nil
    end

    return ret
end

function g_load_json(file)
    local f, err = io.open(file, "r")
    if f == nil then
        g_log_err(string.format("Opening file '%s' failed. (%s)", file, err))
        return nil
    end

    local content = f:read("*all")
    if not content then
        g_log_err(string.format("Reading file '%s' content failed.", file))
        f:close()
        return nil
    end
    f:close()

    return g_load_json_from_string(content)
end

function g_save_json(file, tbl)
    local succ, ret = pcall(function () return JSON:encode_pretty(tbl) end)
    if not succ then 
        g_log_err(string.format("Encoding Json failed. (%s)", ret))
        return false
    end

    local f, err = io.open(file, "w")
    if not f then
        g_log_err(string.format("Opening file '%s' failed. (%s)", file, err))
        return false
    end

    succ, ret = f:write(ret)
    if not succ then 
        g_log_err(string.format("Writing Json '%s' failed. (%s)", file, ret))
        f:close()
        return false
    end

    f:close()
    return true
end

function g_goto_scene(sceneName, sceneConfig) 
    local config = sceneConfig
    if config == nil then
        config = ""
    end

    print(string.format("goto_scene('%s', '...')", sceneName))
    bt_native.goto_scene(sceneName, config)
end

function g_split(s, p)
    local rt= {}
    string.gsub(s, '[^'..p..']+', function(w) table.insert(rt, w) end )
    return rt
end

function g_table_to_string( tbl )
    local function table_print (tt, indent, done)
      done = done or {}
      indent = indent or 0
      if type(tt) == "table" then
        local sb = {}
        for key, value in pairs (tt) do
          table.insert(sb, string.rep (" ", indent)) -- indent it
          if type (value) == "table" and not done [value] then
            done [value] = true
            table.insert(sb, tostring (key).." = {\n");
            table.insert(sb, table_print (value, indent + 2, done))
            table.insert(sb, string.rep (" ", indent)) -- indent it
            table.insert(sb, "}\n");
          elseif "number" == type(key) then
            table.insert(sb, string.format("%s = \"%s\"\n", tostring (key), tostring(value)))
          else
            table.insert(sb, string.format("%s = \"%s\"\n", tostring (key), tostring(value)))
           end
        end
        return table.concat(sb)
      else
        return tt .. "\n"
      end
    end

    if  "nil"       == type( tbl ) then
        return tostring(nil)
    elseif  "table" == type( tbl ) then
        return table_print(tbl)
    elseif  "string" == type( tbl ) then
        return tbl
    else
        return tostring(tbl)
    end
end

function g_print_table_pretty(tbl)
    print(tostring(tbl).." printed below:\n"..
        "----- table begin -----\n"..
        g_table_to_string(tbl)..
        "-----  table end  -----\n")
end
