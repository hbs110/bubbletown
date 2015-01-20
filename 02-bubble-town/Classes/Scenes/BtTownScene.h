/*!
 * \file TownScene.h
 * \date 2014/12/22 17:07
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TownScene 
*/

#pragma once

typedef cocos2d::experimental::TMXTiledMap  tileMap_t;

class BtTownScene : public cocos2d::Layer
{
public:
    BtTownScene() : m_tileMap(nullptr), m_isPicking(false) {}
    virtual ~BtTownScene() {}
    CREATE_FUNC(BtTownScene);

    virtual bool init();  

    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

    void onMenuItem(Ref* sender);
    void onMenu_Pick(Ref* sender);
    void onMenu_Build(Ref* sender);

    tileMap_t* m_tileMap;

    bool m_isPicking;

    cocos2d::Node* m_root;
};

