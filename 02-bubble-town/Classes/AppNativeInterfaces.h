/*!
 * \file AppNativeInterfaces.h
 * \date 2015/02/10 11:00
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of AppNativeInterfaces 
*/

#pragma once

class AppNativeInterfaces 
{
public: 
    static double GetCurrentGameTime();
    static void EmitMsg_GotoScene(const std::string& sceneName);
    static bool CopyFile(const std::string& src, const std::string& dest);
};
