
import shutil
import os

BtLua_Major = 0
BtLua_Minor = 1
BtLua_Patch = 0

BtLuaDistDir = "BtLua-v{}.{}.{}".format(BtLua_Major, BtLua_Minor, BtLua_Patch) 

BtLua_LibFiles = [ 
    "lua-5.2.3-static.lib",
    "btlua-static.lib", 
    "lua-5.2.3-static-debug.lib", 
    "btlua-static-debug.lib"
]

def main():
    # remove existing dist directory
    if os.path.exists(BtLuaDistDir):
        shutil.rmtree(BtLuaDistDir)

    # copy headers
    shutil.copytree("BtLua/include", os.path.join(BtLuaDistDir, "include"))

    # copy lib (currently only the static release version is used)
    os.makedirs(os.path.join(BtLuaDistDir, "lib"))
    for f in BtLua_LibFiles:
        shutil.copyfile(os.path.join("BtLua/build/vs2012/bin", f), os.path.join(BtLuaDistDir, "lib", f))

if __name__ == '__main__':
    main()
