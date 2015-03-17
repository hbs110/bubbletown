/*!
 * \file BtLuaBuiltinInterfaces.h
 * \date 2015/03/06 13:46
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtLuaBuiltinInterfaces 
*/

#pragma once

class BtLuaBuiltinInterfaces 
{
public:
    static void Print(const std::string& msg);

    static std::string GetAppWritablePath();

    static std::string LoadStringFromFile(const std::string& file);
    static std::string LoadStringFromFileWP(const std::string& file);
    static bool SaveStringIntoFileWP(const std::string& file, const std::string& content);
};

