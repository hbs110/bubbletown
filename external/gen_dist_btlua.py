
import shutil
import os
import fnmatch

_join = os.path.join

BtLuaDistDir = "BtLua-dist"
BtLua_Toolset = "vs2013"
BtSrcDir_Lua = "BtLua/dep/lua-5.2.3-" + BtLua_Toolset + "-shared"
BtSrcDir_LuaBridge = "BtLua/dep/LuaBridge/Source/LuaBridge"

BtLua_LibFiles = [ 
    _join("BtLua", "build", BtLua_Toolset, "bin", "btlua-shared.lib"),
    _join("BtLua", "build", BtLua_Toolset, "bin", "btlua-shared-debug.lib"),
]

BtLua_DLLFiles = [ 
    _join("BtLua", "build", BtLua_Toolset, "bin", "btlua-shared.dll"),
    _join("BtLua", "build", BtLua_Toolset, "bin", "btlua-shared-debug.dll"),
]

def main():
    print("erasing existing dist directory...")
    if os.path.exists(BtLuaDistDir):
        shutil.rmtree(BtLuaDistDir)

    print("copying BtLua headers...")
    BtLuaDistInclude = _join(BtLuaDistDir, "include")
    shutil.copytree("BtLua/include", BtLuaDistInclude)

    print("copying lua headers and binaries...")
    shutil.copytree(_join(BtSrcDir_Lua, "include"), _join(BtLuaDistInclude, "lua"))
    shutil.copytree(_join(BtSrcDir_Lua, "lib"), _join(BtLuaDistDir, "lib", BtLua_Toolset))
    shutil.copytree(_join(BtSrcDir_Lua, "bin"), _join(BtLuaDistDir, "bin"))

    print("copying luabridge headers...")
    shutil.copytree(BtSrcDir_LuaBridge, _join(BtLuaDistInclude, "LuaBridge"))

    print("copying BtLua lib files...")
    for f in BtLua_LibFiles:
        shutil.copyfile(f, _join(BtLuaDistDir,  "lib", BtLua_Toolset, os.path.basename(f)))

    print("copying BtLua binaries...")
    for f in BtLua_DLLFiles:
        shutil.copyfile(f, _join(BtLuaDistDir,  "bin", os.path.basename(f)))
    
    print("done successfully!")

if __name__ == '__main__':
    main()
