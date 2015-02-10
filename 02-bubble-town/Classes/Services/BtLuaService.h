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

#define BT_L                          BtLuaService::Get()->GetHandle()          

#define BT_CALL_LUA(func_name, ...)     BTLUA_CALL_FUNCTION(BT_L, func_name, __VA_ARGS__)

namespace
{
    BtConstStr BtNativeNamespace = "bt_native";
}

class BtLuaService 
{
    BT_SINGLETON_DEF(BtLuaService);

public: 
    bool Init();
    void Destroy();

    btlua_handle GetHandle();  // main Lua handle accessing

    template <typename T>
    void RegisterVariable(const char* name, T* var);
    template <typename FUNC>
    void RegisterFunction(const char* name, FUNC const fp);

private:
    // callbacks of various internal events
    void OnError(const std::string& errMsg);
    void OnPrint(const std::string& msg);

    // methods exposed to Lua 
    static void NativePrint(const std::string& msg);
};


/* ----- Implementation Details ----- */

template <typename T>
void BtLuaService::RegisterVariable(const char* name, T* var)
{
    btlua_handle h = GetHandle();
    if (!h)
        return;

    luabridge::getGlobalNamespace(h)
        .beginNamespace(BtNativeNamespace)
        .addVariable(name, var)
        .endNamespace();
}


template <typename FUNC>
void BtLuaService::RegisterFunction(const char* name, FUNC const fp)
{
    btlua_handle h = GetHandle();
    if (!h)
        return;

    luabridge::getGlobalNamespace(h)
        .beginNamespace(BtNativeNamespace)
        .addFunction(name, fp)
        .endNamespace();
}
