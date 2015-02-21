
# -*- coding: utf-8 -*-

import shutil
import os
import fnmatch

LuaSrcFile = "Resources/lua/shared/def.lua"
CppDestFile = "Classes/Core/BtMsgDef_AUTOGEN.h"


Enums = ['BtMsgID', 'BtProb']
Integers = ['BT_Invalid']
StringPrefixes = ['BTSCN_', 'BT_', 'BTPL_']


CppHeadingText = \
u"""/*
    !!! GENERATED FROM LUA !!!

    For editing, 
        1. modify the file '{}'
        2. run '{}' to regenerate this file
*/

#pragma once

#include "BtCoreDef.h"

""".format(LuaSrcFile, __file__).encode('utf-8')


def convert(src, dest):
    
    dest.write(CppHeadingText)

    for line in src:
        line = line.replace('--', '//')

        for integer in Integers:
            if line.startswith(integer):
                line = "const int " + line
                break

        for enum in Enums:
            if line.startswith(enum):
                line = "enum class " + enum + "\n"
                break

        for prefix in StringPrefixes:
            if line.startswith(prefix):
                line = "BtConstStr " + line
                break

        dest.write(line)


def main():
    with open(LuaSrcFile, "r") as src:
        with open(CppDestFile, "w") as dest:
            convert(src, dest)


if __name__ == '__main__':
    main()
