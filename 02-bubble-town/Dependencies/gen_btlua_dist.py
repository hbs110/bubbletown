
import shutil
import os

BtLua_Major = 0
BtLua_Minor = 1
BtLua_Patch = 0

BtLuaDistDir = "BtLua-v{}.{}.{}".format(BtLua_Major, BtLua_Minor, BtLua_Patch) 

btlua_libfile = "btlua-static.lib"

def main():
    # remove existing dist directory
    if os.path.exists(BtLuaDistDir):
        shutil.rmtree(BtLuaDistDir)

    # copy headers
    shutil.copytree("BtLua/include", os.path.join(BtLuaDistDir, "include"))

    # copy lib (currently only the static release version is used)
    os.makedirs(os.path.join(BtLuaDistDir, "lib"))
    source_file = os.path.join("BtLua/build/vs2012/bin", btlua_libfile)
    target_file = os.path.join(BtLuaDistDir, "lib", btlua_libfile)
    shutil.copyfile(source_file, target_file)

if __name__ == '__main__':
    main()
