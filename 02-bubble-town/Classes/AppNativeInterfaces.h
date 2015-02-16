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
    static void GotoScene(const std::string& sceneName, const std::string& sceneConfig);

    static void SetPlayerProperty(const std::string& playerProperty, int value, bool flush);
};

void BtRegisterNativeInerfaces();

