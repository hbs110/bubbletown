/*!
 * \file TownScene.h
 * \date 2014/12/22 17:07
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TownScene 
*/

#pragma once

#include "cocos2d.h"

#include "Core/BtGuiUtil.h"

typedef cocos2d::experimental::TMXTiledMap  tileMap_t;

class BtTownScene : public cocos2d::Layer, public BtGuiListener
{
public:
    BtTownScene() : m_tileMap(nullptr) {}
    virtual ~BtTownScene() {}

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();

    // a selector callback
    void menuCloseCallback(Ref* sender);

    // implement the "static node()" method manually
    CREATE_FUNC(BtTownScene);

    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

    // from BtGuiListener
    virtual void OnMenuItem(Ref* sender);

    tileMap_t* m_tileMap;
};

