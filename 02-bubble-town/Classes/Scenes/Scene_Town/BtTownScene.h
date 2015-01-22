/*!
 * \file TownScene.h
 * \date 2014/12/22 17:07
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TownScene 
*/

#pragma once

#include "Scenes/BtBaseScene.h"

#include "BtTiledMap.h"

class BtTownBuilding;

class BtTownScene : public BtBaseScene
{
public:
    BtTownScene() : m_isPicking(false) {}
    virtual ~BtTownScene() {}
    CREATE_FUNC(BtTownScene);

protected:
    virtual bool do_init();

    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

    void onMenu_Clear(Ref* sender);
    void onMenu_Build(Ref* sender, const std::string& buildingName);

    BtTiledMap m_tiledMap;

    bool m_isPicking;
    std::string m_currentBuildingName;

    std::vector<BtTownBuilding*> m_buildings;
};

