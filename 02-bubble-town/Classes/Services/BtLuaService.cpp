/*!
 * \file BtLuaService.cpp
 * \date 2015/01/26 18:37
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtLuaService
*/

#include "stdafx.h"
#include "BtLuaService.h"

#include "Core/tinyformat/tinyformat.h"

#include <btlua.h>

#ifdef _DEBUG
#   define LIB_NAME_POSTFIX  "-debug"
#else
#   define LIB_NAME_POSTFIX  ""
#endif

#pragma comment(lib, "lua-5.2.3-static"LIB_NAME_POSTFIX".lib")
#pragma comment(lib, "btlua-static"LIB_NAME_POSTFIX".lib")

namespace
{
    // wouldn't pollute the global namespace
    btlua_handle L;

    BtConstStr Namespace_Default = "bubbletown";

    BtConstStr Func_Print = "print";
}

BT_SINGLETON_IMPL(BtLuaService);

BtLuaService::BtLuaService() 
{

}

BtLuaService::~BtLuaService()
{
    Destroy();
}

bool BtLuaService::Init()
{
    L = BtLua_Init();
    if (!L)
        return false;

    BtLua_SetErrorOutput(std::bind(&BtLuaService::OnError, this, std::placeholders::_1));

    if (!BtLua_ExecFile(L, "bootstrap.lua"))
        return false;

    luabridge::getGlobalNamespace(L)
        .beginNamespace(Namespace_Default)
        .addFunction(Func_Print, BtLuaService::NativePrint)
        .endNamespace();

    // replacing system functions with customized ones 
    BtLua_ExecString(L, tinyformat::format("_G.%s = %s.%s", Func_Print, Namespace_Default, Func_Print));

    return true;
}

void BtLuaService::Destroy()
{
    BtLua_Destroy(L);
}

btlua_handle BtLuaService::GetHandle()
{
    return L;
}

void BtLuaService::OnError(const std::string& errMsg)
{
    CCLOG("lua_err: %s", errMsg.c_str());
}

void BtLuaService::OnPrint(const std::string& msg)
{
    CCLOG("lua: %s", msg.c_str());
}

void BtLuaService::NativePrint(const std::string& msg)
{
    Get()->OnPrint(msg);
}

