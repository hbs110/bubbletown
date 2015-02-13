
# -*- coding: utf-8 -*-

import shutil
import os
import fnmatch

LuaSrcFile = "Resources/lua/shared/def.lua"
CppDestFile = "Classes/Core/BtMsgDef_AUTOGEN.h"

EnumName_MsgId = 'BtMsgID'
SceneNamePrefix = 'BTSCN_'
BuildingNamePrefix = 'BT_'

CppHeadingText = \
u"""/*
    !!! 此文件由脚本生成，请勿直接修改 !!!

    如需修改，请修改文件 {} ，并运行 {} 重新生成此头文件
*/

#pragma once

#include "BtCoreDef.h"

""".format(LuaSrcFile, __file__).encode('utf-8')


def convert(src, dest):
    
    dest.write(CppHeadingText)

    for line in src:
        line = line.replace('--', '//')

        if line.startswith(EnumName_MsgId):
            line = "enum class " + EnumName_MsgId + "\n"

        if line.startswith(SceneNamePrefix) or line.startswith(BuildingNamePrefix):
            line = "BtConstStr " + line

        dest.write(line)


def main():
    with open(LuaSrcFile, "r") as src:
        with open(CppDestFile, "w") as dest:
            convert(src, dest)


if __name__ == '__main__':
    main()
