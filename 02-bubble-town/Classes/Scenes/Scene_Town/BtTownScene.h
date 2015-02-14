/*!
 * \file TownScene.h
 * \date 2014/12/22 17:07
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TownScene 
*/

#pragma once

#include "Core/BtMsgDef.h"
#include "Scenes/BtBaseScene.h"

#include "BtTiledMap.h"
#include "BtTownSceneWidgets.h"
#include "BtTownSceneUI.h"

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
    BT_DEF_SCENE(BtTownScene, BTSCN_town);

protected:
    virtual bool do_init();

    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

    void getBuildingRect(const cocos2d::Vec2& tileCoord, const std::string& buildingName, cocos2d::Rect* tileRect);
    void getBuildingPos(const cocos2d::Vec2& tileCoord, const std::string& buildingName, cocos2d::Vec2* buildingPos);
    bool isAvailForBuilding(const cocos2d::Vec2& tileCoord, const std::string& buildingName);
    void updateArrows(bool avail);

    // entities
    BtTiledMap m_tiledMap;
    std::vector<BtTownBuilding*> m_buildings;

    BtTownBuilding* m_selectedBuilding;
    cocos2d::Vec2 m_selectedBuildingOriginalCoord;

    // not used yet
    eOperationState m_operationState;
    float m_holdBuildingTimer;

    BtTownSceneWidgets m_widgets;
    BtTownSceneUI m_ui;

    std::string m_placingBuildingName;
    void onPlacingBuildingBegan(const std::string& buildingName);
    bool isPlacingBuilding() const { return m_placingBuildingName.size(); }

    void selectBuilding(BtTownBuilding* building);
};

