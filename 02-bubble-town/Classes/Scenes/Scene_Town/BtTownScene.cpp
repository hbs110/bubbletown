/*!
 * \file TownScene.cpp
 * \date 2014/12/22 17:08
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TownScene
*/

#include "stdafx.h"
#include "BtTownScene.h"

#include "AppMacros.h"

#include "Core/BtCoreDef.h"
#include "Core/BtGuiUtil.h"
#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"

#include "BtTownBuilding.h"

#include "Services/BtLuaService.h"

#include "Core/tinyformat/tinyformat.h"

enum {
    kTagTileMap = 1,
};

BtTownScene::BtTownScene() 
    : m_operationState(eOperationState::Idle)
    , m_holdBuildingTimer(0.0f)
    , m_selectedBuilding(nullptr)
{
}

// on "init" you need to initialize your instance
bool BtTownScene::do_init()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    if (!m_ui.init(m_uiRoot))
        return false;

    m_ui.setBuildingPlacingBegan(std::bind(&BtTownScene::onPlacingBuildingBegan, this, std::placeholders::_1));

    if (!m_tiledMap.Load("scn_town02/map_tiles.tmx"))
        return false;
    m_sceneRoot->addChild(m_tiledMap.GetTileMapRoot(), 1);

    auto listener = cocos2d::EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(BtTownScene::onTouchesBegan, this);
    listener->onTouchesEnded = CC_CALLBACK_2(BtTownScene::onTouchesEnded, this);
    listener->onTouchesMoved = CC_CALLBACK_2(BtTownScene::onTouchesMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    if (!m_widgets.init(m_tiledMap.GetSpriteRoot()))
        return false;

    return true;
}

void BtTownScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    if (isPlacingBuilding())
        return;

    auto touch = touches[0];
    cocos2d::Vec2 tileCoord;
    cocos2d::Vec2 tileCenter;
    if (!m_tiledMap.getTouchedTileInfo(touch, &tileCoord, &tileCenter))
        return;

    auto touchedPos = m_tiledMap.GetSpriteRoot()->convertTouchToNodeSpace(touch);

    BtTownBuilding* selected = nullptr;
    for (auto building : m_buildings)
    {
        cocos2d::Rect bbox = building->getBoundingBox();
        if (bbox.containsPoint(touchedPos))
        {
            selected = building;
            break;
        }
    }

    selectBuilding(selected);
}

void BtTownScene::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    if (isPlacingBuilding())
    {
        // support one touch only (ignore additional touches)
        auto touch = touches[0];
        if (touch->getStartLocation() == touch->getLocation())
        {
            cocos2d::Vec2 tileCoord;
            cocos2d::Vec2 tileCenter;
            if (m_tiledMap.getTouchedTileInfo(touch, &tileCoord, &tileCenter) &&
                isAvailForBuilding(tileCoord, m_placingBuildingName))
            {
                btlua_ref image = BT_CALL_LUA("get_building_image", m_placingBuildingName);
                if (image.isString())
                {
                    BtTownBuilding* building = BtTownBuilding::create(m_placingBuildingName, image.tostring(), 2);
                    building->initDeco();

                    cocos2d::Vec2 buildingPos;
                    getBuildingPos(tileCoord, m_placingBuildingName, &buildingPos);
                    building->setPosition(buildingPos);

                    m_buildings.push_back(building);
                    m_tiledMap.GetSpriteRoot()->addChild(building);

                    BT_POST_LUA(BtMsgID::ConstructBuilding, m_placingBuildingName);
                }
            }
        }

        m_placingBuildingName.clear();;
        m_tiledMap.endSocketShimmering();
    }

    if (m_selectedBuilding)
    {
        auto touch = touches[0];
        cocos2d::Vec2 tileCoord;
        cocos2d::Vec2 tileCenter;
        if (m_tiledMap.getTouchedTileInfo(touch, &tileCoord, &tileCenter))
        {
            if (!isAvailForBuilding(tileCoord, m_selectedBuilding->getName()))
            {
                m_selectedBuilding->setPosition(m_selectedBuildingOriginalCoord);
                updateArrows(true);
            }
        }
    }
}

void BtTownScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
    if (m_selectedBuilding)
    {
        auto layer = m_tiledMap.GetTileMapLayer(BtLayer_Background);
        if (layer)
        {
            cocos2d::Vec2 tileCoord;
            cocos2d::Vec2 tileCenter;
            if (m_tiledMap.getTouchedTileInfo(touches[0], &tileCoord, &tileCenter))
            {
                cocos2d::Vec2 buildingPos;
                getBuildingPos(tileCoord, m_selectedBuilding->getName(), &buildingPos);
                m_selectedBuilding->setPosition(buildingPos);
                updateArrows(isAvailForBuilding(tileCoord, m_selectedBuilding->getName()));
            }
        }
    } 
    else
    {
        m_tiledMap.onTouchesMoved(touches, event);
    }
}

void BtTownScene::updateArrows(bool avail)
{
    if (m_selectedBuilding)
    {
        cocos2d::Vec2 tileCoordBuilding;
        if (m_tiledMap.getTileCoord(m_selectedBuilding->getPosition(), &tileCoordBuilding))
        {
            cocos2d::Rect tileRect;
            getBuildingRect(tileCoordBuilding, m_selectedBuilding->getName(), &tileRect);

            float arrowScale = 1.0f;
            btlua_ref scale = BT_CALL_LUA("get_building_arrowScale", m_selectedBuilding->getName());
            if (scale.isNumber())
                arrowScale = (float) scale;

            m_widgets.showArrowsAt(&m_tiledMap, tileRect, arrowScale);
            m_widgets.markArrowsAvail(avail);
        }
    }
    else
    {
        m_widgets.hideArrows();
    }
}

bool BtTownScene::isAvailForBuilding(const cocos2d::Vec2& tileCoord, const std::string& buildingName)
{
    cocos2d::Rect tileRect;
    getBuildingRect(tileCoord, buildingName, &tileRect);
    return m_tiledMap.isInSocketArea(tileRect);
}

void BtTownScene::getBuildingRect(const cocos2d::Vec2& tileCoord, const std::string& buildingName, cocos2d::Rect* tileRect)
{
    if (!tileRect)
        return;

    float width = 1.0f;
    btlua_ref w = BT_CALL_LUA("get_building_width", buildingName);
    if (w.isNumber())
        width = (float) w;

    float height = 1.0f;
    btlua_ref h = BT_CALL_LUA("get_building_height", buildingName);
    if (h.isNumber())
        height = (float) h;

    tileRect->origin.x = tileCoord.x - (float) (((int) width - 1) / 2);
    tileRect->origin.y = tileCoord.y - (float) (((int) height - 1) / 2);
    tileRect->size.width = width - 1;
    tileRect->size.height = height - 1;

}

void BtTownScene::getBuildingPos(const cocos2d::Vec2& tileCoord, const std::string& buildingName, cocos2d::Vec2* buildingPos)
{
    cocos2d::Rect tileRect;
    getBuildingRect(tileCoord, buildingName, &tileRect);
    cocos2d::Vec2 buildingTile = cocos2d::Vec2(tileRect.getMidX(), tileRect.getMidY());
    m_tiledMap.getTilePosition(buildingTile, buildingPos);
}

void BtTownScene::selectBuilding(BtTownBuilding* building)
{
    if (building)
    {
        if (building != m_selectedBuilding)
        {
            if (m_selectedBuilding)
                m_selectedBuilding->deselect();

            m_selectedBuilding = building;
            m_selectedBuilding->select();
            m_selectedBuildingOriginalCoord = m_selectedBuilding->getPosition();
        }
    }
    else
    {
        if (m_selectedBuilding)
        {
            m_selectedBuilding->deselect();
            m_selectedBuilding = nullptr;
        }
    }

    m_ui.setUpgradeVisible(building != nullptr);
    updateArrows(true);
}

void BtTownScene::onPlacingBuildingBegan(const std::string& buildingName)
{
    m_placingBuildingName = buildingName;
    m_tiledMap.beginSocketShimmering();
}


