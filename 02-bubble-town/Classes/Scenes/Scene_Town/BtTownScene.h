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
#include "BtTownSceneWidgets.h"

class BtTownBuilding;

enum class eOperationState
{
    Idle,
    MovingScene,
    MovingBuilding,
};

class BtTownScene : public BtBaseScene
{
public:
    BtTownScene();
    virtual ~BtTownScene() {}
    CREATE_FUNC(BtTownScene);

protected:
    virtual bool do_init();

    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

    void onMenu_Build(Ref* sender, const std::string& buildingName);

    void updateArrowPositions(BtTownBuilding* building);

    // entities
    BtTiledMap m_tiledMap;
    std::vector<BtTownBuilding*> m_buildings;
    BtTownBuilding* m_selectedBuilding;

    // placing building
    bool m_isPlacingBuilding;
    std::string m_placingBuildingName;

    // not used yet
    eOperationState m_operationState;
    float m_holdBuildingTimer;

    BtTownSceneWidgets m_widgets;
};

