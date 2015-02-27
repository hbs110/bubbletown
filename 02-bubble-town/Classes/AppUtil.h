/*!
 * \file AppUtil.h
 * \date 2015/02/10 11:00
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of AppUtil 
*/

#pragma once

class AppDelegate;
class BtTownScene;

class AppUtil 
{
public: 
    static AppDelegate* GetApp();

    static BtTownScene* GetTownSceneIfIn();
};

