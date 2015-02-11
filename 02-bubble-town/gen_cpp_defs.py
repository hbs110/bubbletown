
# -*- coding: utf-8 -*-

import shutil
import os
import fnmatch

LuaSrcFile = "Resources/lua/shared/def_msgs.lua"
CppDestFile = "Classes/Core/BtMsgDef_AUTOGEN.h"

EnumName_MsgId = 'BtMsgID'


CppPragmaText = "#pragma once\n"

CppWarningText = \
u"""

/*
    !!! 此文件由脚本生成，请勿直接修改 !!!

    请修改文件 {}
    并运行 {} 重新生成此头文件
*/

""".format(LuaSrcFile, __file__).encode('utf-8')


def convert(src, dest):
    
    dest.write(CppPragmaText)
    dest.write(CppWarningText)

    for line in src:
        line = line.replace('--', '//')

        if line.startswith(EnumName_MsgId):
            line = "enum class " + EnumName_MsgId + "\n"

        dest.write(line)


def main():
    with open(LuaSrcFile, "r") as src:
        with open(CppDestFile, "w") as dest:
            convert(src, dest)


if __name__ == '__main__':
    main()
