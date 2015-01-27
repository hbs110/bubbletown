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
#include <btlua_call.h>

#ifdef _DEBUG
#   define LIB_NAME_POSTFIX  "-debug"
#else
#   define LIB_NAME_POSTFIX  ""
#endif

#pragma comment(lib, "lua-5.2.3-static"LIB_NAME_POSTFIX".lib")
#pragma comment(lib, "btlua-static"LIB_NAME_POSTFIX".lib")

BT_SINGLETON_IMPL(BtLuaService);

BtLuaService::BtLuaService() 
    : m_lua(nullptr)
{

}

BtLuaService::~BtLuaService()
{
    Destroy();
}

bool BtLuaService::Init()
{
    BtLuaAddErrorOutput(std::bind(&BtLuaService::OnError, this, std::placeholders::_1));

    m_lua = new BtLua;
    if (!m_lua->Init("bootstrap.lua"))
        return false;

    std::string value;
    if (m_lua->GetGlobalString("foo", &value))
        CCLOG("foo: %s", value.c_str());

    luabridge::LuaRef ref = m_lua->GetGlobalCallable("get_building_image");
    luabridge::LuaRef ret = BtLuaCall(m_lua->GetHandle(), std::bind(ref, "hall"));
    if (ret.isString())
    {
        CCLOG("ret: %s", ret.tostring().c_str());
    }
    return true;
}

void BtLuaService::Destroy()
{
    BtDeletePointer(m_lua);
}

void BtLuaService::OnError(const std::string& errMsg)
{
    CCLOG(errMsg.c_str());
}

