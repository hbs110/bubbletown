/*!
 * \file BtLuaLoader.h
 * \date 2015/03/06 11:17
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtLuaLoader 
*/

#pragma once

// it's a lua callback, which has to be guaranteed as C-style invoking
extern "C"
{
    int BtLuaLoader(lua_State *L);
}

// lua package handling
//      - manipulating package.path & package.searchers
void BtLuaAddSearchPath(lua_State * _state, const char* path);
void BtSetLuaLoader(lua_State * _state, lua_CFunction func);
