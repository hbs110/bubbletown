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
#include "Core/BtCoreUtil.h"

#include "BtLuaLoader.h"

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
    L = BtLua_Init();
    if (!L)
        return false;

    // builtin functions & callbacks
    RegisterFunction("print", &BtLuaService::NativePrint);
    BtLua_SetErrorOutput(std::bind(&BtLuaService::OnError, this, std::placeholders::_1));

    BtLuaAddSearchPath(L, "lua");
    BtSetLuaLoader(L, BtLuaLoader);

    // bootstrapping
    if (!RunScriptFile("bootstrap"))
        return false;

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
    BT_ERROR("lua_err: %s", errMsg);
}

void BtLuaService::NativePrint(const std::string& msg)
{
    BT_LOG("lua: %s", msg.c_str());
}

bool BtLuaService::RunScriptFile(const char* filename)
{
    return BtLua_ExecString(L, tfm::format("require \"%s\"", filename));
}

