/*!
 * \file AppStartScene.h
 * \date 2015/01/21 9:56
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of AppStartScene 
*/

#pragma once

#include "Core/BtMsgDef.h"
#include "Scenes/BtBaseScene.h"

class AppStartScene : public cocos2d::Layer
{
public:
    virtual ~AppStartScene() {}
    
    BT_DEF_SCENE(AppStartScene, BTSCN_start);

    virtual bool init();  
};
