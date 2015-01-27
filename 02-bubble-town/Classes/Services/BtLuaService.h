/*!
 * \file BtLuaService.h
 * \date 2015/01/26 18:37
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtLuaService 
*/

#pragma once

#include "Core/BtCoreDef.h"

class BtLua;

class BtLuaService 
{
    BT_SINGLETON_DEF(BtLuaService);

public: 
    bool Init();
    void Destroy();

private:
    BtLua* m_lua;

    void OnError(const std::string& errMsg);
};

