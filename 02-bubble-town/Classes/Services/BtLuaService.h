/*!
 * \file BtLuaService.h
 * \date 2015/01/26 18:37
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtLuaService 
*/

#pragma once

#include "Core/BtCoreDef.h"

#include "btlua.h"

#define BT_CALL_LUA(func_name, ...)      BTLUA_CALL_FUNCTION(BtLuaService::Get()->GetHandle(), func_name, __VA_ARGS__)

class BtLuaService 
{
    BT_SINGLETON_DEF(BtLuaService);

public: 
    bool Init();
    void Destroy();

    btlua_handle GetHandle();  // main Lua handle accessing

private:
    // callbacks of various internal events
    void OnError(const std::string& errMsg);
    void OnPrint(const std::string& msg);

    // methods exposed to Lua 
    static void NativePrint(const std::string& msg);
};

