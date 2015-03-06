/*!
 * \file BtLuaLoader.cpp
 * \date 2015/03/06 11:17
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtLuaLoader
*/

#include "stdafx.h"
#include "BtLuaLoader.h"

#include "btlua.h"

#include "Core/BtCoreUtil.h"

extern "C" 
{
    int BtLuaLoader(lua_State *L)
    {
        static const std::string BYTECODE_FILE_EXT = ".luac";
        static const std::string NOT_BYTECODE_FILE_EXT = ".lua";

        std::string filename(luaL_checkstring(L, 1));
        size_t pos = filename.rfind(BYTECODE_FILE_EXT);
        if (pos != std::string::npos)
        {
            filename = filename.substr(0, pos);
        }
        else
        {
            pos = filename.rfind(NOT_BYTECODE_FILE_EXT);
            if (pos == filename.length() - NOT_BYTECODE_FILE_EXT.length())
            {
                filename = filename.substr(0, pos);
            }
        }

        pos = filename.find_first_of(".");
        while (pos != std::string::npos)
        {
            filename.replace(pos, 1, "/");
            pos = filename.find_first_of(".");
        }

        //BT_LOG("1) after filename manipulation: %s", filename.c_str());

        // search file in package.path
        unsigned char* chunk = nullptr;
        ssize_t chunkSize = 0;
        std::string chunkName;
        cocos2d::FileUtils* utils = cocos2d::FileUtils::getInstance();

        lua_getglobal(L, "package");
        lua_getfield(L, -1, "path");
        std::string searchpath(lua_tostring(L, -1));
        lua_pop(L, 1);
        size_t begin = 0;
        size_t next = searchpath.find_first_of(";", 0);

        do
        {
            if (next == std::string::npos)
                next = searchpath.length();
            std::string prefix = searchpath.substr(begin, next);
            if (prefix[0] == '.' && prefix[1] == '/')
            {
                prefix = prefix.substr(2);
            }

            pos = prefix.find("?.lua");
            chunkName = prefix.substr(0, pos) + filename + BYTECODE_FILE_EXT;
            if (utils->isFileExist(chunkName))
            {
                chunk = utils->getFileData(chunkName.c_str(), "rb", &chunkSize);
                break;
            }
            else
            {
                chunkName = prefix.substr(0, pos) + filename + NOT_BYTECODE_FILE_EXT;
                //BT_LOG("2) trying: %s", chunkName.c_str());
                if (utils->isFileExist(chunkName))
                {
                    chunk = utils->getFileData(chunkName.c_str(), "rb", &chunkSize);
                    break;
                }
            }

            begin = next + 1;
            next = searchpath.find_first_of(";", begin);
        } while (begin < (int) searchpath.length());

        if (chunk)
        {
            int r = luaL_loadbuffer(L, (char*) chunk, (int) chunkSize, chunkName.c_str());

            //BT_LOG("file data loaded: %s", chunkName.c_str());

#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG > 0
            if (r)
            {
                switch (r)
                {
                case LUA_ERRSYNTAX:
                    BT_LOG("[LUA ERROR] load \"%s\", error: syntax error during pre-compilation.", chunkName.c_str());
                    break;

                case LUA_ERRMEM:
                    BT_LOG("[LUA ERROR] load \"%s\", error: memory allocation error.", chunkName.c_str());
                    break;

                case LUA_ERRFILE:
                    BT_LOG("[LUA ERROR] load \"%s\", error: cannot open/read file.", chunkName.c_str());
                    break;

                default:
                    BT_LOG("[LUA ERROR] load \"%s\", error: unknown.", chunkName.c_str());
                }
            }
#endif
            free(chunk);
        }
        else
        {
            BT_LOG("can not get file data of %s", chunkName.c_str());
            return 0;
        }

        return 1;
    }
}

void BtLuaAddSearchPath(lua_State * _state, const char* path)
{
    lua_getglobal(_state, "package");                                  /* L: package */
    lua_getfield(_state, -1, "path");                /* get package.path, L: package path */
    const char* cur_path = lua_tostring(_state, -1);
    lua_pushfstring(_state, "%s;%s/?.lua", cur_path, path);            /* L: package path newpath */
    lua_setfield(_state, -3, "path");          /* package.path = newpath, L: package path */
    lua_pop(_state, 2);                                                /* L: - */
}

void BtSetLuaLoader(lua_State * _state, lua_CFunction func)
{
    if (!func) return;

    // stack content after the invoking of the function
    // get loader table
    lua_getglobal(_state, "package");                                  /* L: package */
    lua_getfield(_state, -1, "searchers");                               /* L: package, loaders */

    // insert loader into index 2
    lua_pushcfunction(_state, func);                                   /* L: package, loaders, func */
    for (int i = (int) (lua_rawlen(_state, -2) + 1); i > 2; --i)
    {
        lua_rawgeti(_state, -2, i - 1);                                /* L: package, loaders, func, function */
        // we call lua_rawgeti, so the loader table now is at -3
        lua_rawseti(_state, -3, i);                                    /* L: package, loaders, func */
    }
    lua_rawseti(_state, -2, 2);                                        /* L: package, loaders */

    // set loaders into package
    lua_setfield(_state, -2, "searchers");                               /* L: package */

    lua_pop(_state, 1);
}
