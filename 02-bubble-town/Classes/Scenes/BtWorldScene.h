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

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // a selector callback
    void menuCloseCallback(Ref* sender);

    // implement the "static node()" method manually
    CREATE_FUNC(BtWorldScene);

    // from BtGuiListener
    virtual void OnMenuItem(cocos2d::Ref* sender);
};

