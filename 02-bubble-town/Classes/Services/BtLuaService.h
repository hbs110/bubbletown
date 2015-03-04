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

// Lua 环境中为 native 接口准备的名字空间
BtConstStr BtNativeNamespace = "bt_native";
// 全局 Lua 句柄
#define BT_L    BtLuaService::Get()->GetHandle()          

// 同步调用 lua 函数，指定函数名和任意个参数
#define BT_CALL_LUA(func_name, ...)             \
    BTLUA_CALL_FUNCTION(BT_L, func_name, ##__VA_ARGS__)

// 向 lua 发一个异步消息，指定消息 id 和 info，其他参数可选且个数不限，将在下一次 tick 时处理
//    * 适用于一些时效性不高的消息，如获取了某个物品，完成了某个建筑的建造，等等
#define BT_POST_LUA(msg_id, msg_info, ...)      \
    BTLUA_CALL_FUNCTION(BT_L, "hostcall_post", (int)msg_id, msg_info, ##__VA_ARGS__)

// 向 lua 发一个异步消息，但随后立刻处理
//    * 适用于需要立刻响应的消息，如界面响应，相当于走消息处理流程的同步调用
#define BT_POST_LUA_AND_FLUSH(msg_id, msg_info, ...)      \
    BTLUA_CALL_FUNCTION(BT_L, "hostcall_post", (int)msg_id, msg_info, ##__VA_ARGS__); \
    BTLUA_CALL_FUNCTION(BT_L, "hostcall_flush")

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
    // callbacks of internal events
    void OnError(const std::string& errMsg);

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
