/*!
 * \file BtLuaService.cpp
 * \date 2015/01/26 18:37
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtLuaService
*/

#include "stdafx.h"
#include "BtLuaService.h"

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
    BtLua_SetErrorOutput(std::bind(&BtLuaService::OnError, this, std::placeholders::_1));

    L = BtLua_Init();
    if (!L)
        return false;

    if (!BtLua_ExecFile(L, "bootstrap.lua"))
        return false;

    std::string value;
    if (BtLua_GetGlobalString(L, "foo", &value))
        CCLOG("foo: %s", value.c_str());

    return true;
}

void BtLuaService::Destroy()
{
    BtLua_Destroy(L);
}

void BtLuaService::OnError(const std::string& errMsg)
{
    CCLOG(errMsg.c_str());
}

btlua_handle BtLuaService::GetHandle()
{
    return L;
}

