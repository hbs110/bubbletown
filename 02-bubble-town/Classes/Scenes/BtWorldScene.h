/*!
 * \file BtWorldScene.h
 * \date 2014/12/25 17:36
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtWorldScene 
*/

#pragma once

#include "Core/BtGuiUtil.h"

class BtWorldScene : public cocos2d::Layer
{
public:
    BtWorldScene() {}
    virtual ~BtWorldScene() {}
    CREATE_FUNC(BtWorldScene);

    virtual bool init();  

    void onMenuItem(cocos2d::Ref* sender);
};

