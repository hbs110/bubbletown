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

    void onMenu_Build(Ref* sender, const std::string& buildingName);

    void getBuildingRect(const cocos2d::Vec2& tileCoord, const std::string& buildingName, cocos2d::Rect* tileRect);
    void getBuildingPos(const cocos2d::Vec2& tileCoord, const std::string& buildingName, cocos2d::Vec2* buildingPos);
    bool isAvailForBuilding(const cocos2d::Vec2& tileCoord, const std::string& buildingName);
    void updateArrows(BtTownBuilding* building, bool avail);

    // entities
    BtTiledMap m_tiledMap;
    std::vector<BtTownBuilding*> m_buildings;

    BtTownBuilding* m_selectedBuilding;
    cocos2d::Vec2 m_selectedBuildingOriginalCoord;

    // placing building
    bool m_isPlacingBuilding;
    std::string m_placingBuildingName;

    // not used yet
    eOperationState m_operationState;
    float m_holdBuildingTimer;

    BtTownSceneWidgets m_widgets;
};

