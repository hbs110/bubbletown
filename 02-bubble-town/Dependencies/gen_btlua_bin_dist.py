
import shutil
import os
import fnmatch

BtLuaDistDir = "BtLua-dist"
BtLuaDistInclude = os.path.join(BtLuaDistDir, "include")

BtSrcDir_Lua = 'BtLua/dep/lua/src'
BtSrcDir_LuaBridge = "BtLua/dep/LuaBridge/Source/LuaBridge"

BtLua_LibFiles = [ 
    "lua-5.2.3-static.lib",
    "lua-5.2.3-static-debug.lib", 
    "btlua-static.lib", 
    "btlua-static-debug.lib"
]

BtLua_Toolset = "vs2013"

def main():
    # remove existing dist directory
    if os.path.exists(BtLuaDistDir):
        shutil.rmtree(BtLuaDistDir)

    # copy headers
    shutil.copytree("BtLua/include", BtLuaDistInclude)
    shutil.copytree(BtSrcDir_LuaBridge, os.path.join(BtLuaDistInclude, "LuaBridge"))

    # copy lua headers
    target_dir = os.path.join(BtLuaDistInclude, "lua")
    os.makedirs(target_dir)
    for f in os.listdir(BtSrcDir_Lua):
        if fnmatch.fnmatch(f, '*.h') or fnmatch.fnmatch(f, '*.hpp'):
            shutil.copyfile(os.path.join(BtSrcDir_Lua, f), os.path.join(target_dir, f))

    # copy lib (currently only the static release version is used)
    os.makedirs(os.path.join(BtLuaDistDir, "lib", BtLua_Toolset))
    for f in BtLua_LibFiles:
        shutil.copyfile(os.path.join("BtLua", "build", BtLua_Toolset, "bin", f), os.path.join(BtLuaDistDir, "lib", BtLua_Toolset, f))

if __name__ == '__main__':
    main()
