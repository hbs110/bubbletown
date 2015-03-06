/*!
 * \file BtLuaBuiltinInterfaces.cpp
 * \date 2015/03/06 13:46
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtLuaBuiltinInterfaces
*/

#include "stdafx.h"
#include "BtLuaBuiltinInterfaces.h"

#include "Core/BtCoreUtil.h"

void BtLuaBuiltinInterfaces::Print(const std::string& msg)
{
    BT_LOG("lua: %s", msg.c_str());
}

std::string BtLuaBuiltinInterfaces::GetAppWritablePath()
{
    return cocos2d::FileUtils::getInstance()->getWritablePath();
}

std::string BtLuaBuiltinInterfaces::LoadStringFromFile(const std::string& file)
{
    return cocos2d::FileUtils::getInstance()->getStringFromFile(file);
}

std::string BtLuaBuiltinInterfaces::LoadStringFromFileWP(const std::string& file)
{
    std::string fullpath = cocos2d::FileUtils::getInstance()->getWritablePath() + file;
    return cocos2d::FileUtils::getInstance()->getStringFromFile(fullpath);
}

bool BtLuaBuiltinInterfaces::SaveStringIntoFileWP(const std::string& file, const std::string& content)
{
    std::string fullpath = cocos2d::FileUtils::getInstance()->getWritablePath() + file;
    FILE* f = fopen(fullpath.c_str(), "wt");
    if (!f)
        return false;

    size_t written = fwrite(content.c_str(), 1, content.size(), f);
    if (written != content.size())
    {
        fclose(f);
        return false;
    }

    fclose(f);
    return true;
}


