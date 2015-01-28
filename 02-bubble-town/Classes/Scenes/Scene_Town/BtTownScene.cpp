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

#include "Scenes/BtSceneDef.h"

#include "BtTownBuilding.h"

#include "Services/BtLuaService.h"

#include "Core/tinyformat-2.0.1/tinyformat.h"

enum {
    kTagTileMap = 1,
};

BtConstStr MI_Build = "build";

BtConstStr s_buildings[] = 
{
    "deco",
    "hall",
    "house",
    "shop",
    "workshop",
};

BtTownScene::BtTownScene() 
    : m_operationState(eOperationState::Idle)
    , m_holdBuildingTimer(0.0f)
    , m_selectedBuilding(nullptr)
{
    memset(m_arrows, 0, sizeof(cocos2d::Sprite*) * BT_ARRAY_SIZE(m_arrows));
}

// on "init" you need to initialize your instance
bool BtTownScene::do_init()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto titleLabel = cocos2d::LabelTTF::create(BTSCN_Town, "Arial", TITLE_FONT_SIZE);
    titleLabel->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height - titleLabel->getContentSize().height);
    m_uiRoot->addChild(titleLabel, 1);

    if (!m_tiledMap.Load("scn_town02/map_tiles.tmx"))
        return false;
    m_sceneRoot->addChild(m_tiledMap.GetTileMapRoot(), 1);

    auto listener = cocos2d::EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(BtTownScene::onTouchesBegan, this);
    listener->onTouchesEnded = CC_CALLBACK_2(BtTownScene::onTouchesEnded, this);
    listener->onTouchesMoved = CC_CALLBACK_2(BtTownScene::onTouchesMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    BtTextMenuBuilder mb;
    for (auto building : s_buildings)
    {
        mb.AddItem(std::string(MI_Build) + " " + building, std::bind(&BtTownScene::onMenu_Build, this, std::placeholders::_1, building));
    }

    mb.SetItemAlign(BtTextMenuBuilder::Left);
    cocos2d::Menu* menuBuild = mb.Build();
    if (menuBuild)
    {
        cocos2d::Vec2 menuPos;
        menuPos.x = origin.x + 50;
        menuPos.y = origin.y + visibleSize.height - 100;
        menuBuild->setPosition(menuPos);

        addChild(menuBuild, 1);
    }

    cocos2d::ui::Button* btWorld = cocos2d::ui::Button::create("treasure-map-icon.png", "treasure-map-icon.png");
    btWorld->setPosition(cocos2d::Vec2(origin.x + visibleSize.width - btWorld->getContentSize().width / 2, btWorld->getContentSize().height / 2));
    BtSetButtonHandler(btWorld, [] () { BtEmitMessage(BTMSG_GotoScene, BTSCN_World); });
    m_uiRoot->addChild(btWorld, 1);


    m_arrows[0] = cocos2d::Sprite::create("elements/fx/arrow_downleft.png");
    m_arrows[1] = cocos2d::Sprite::create("elements/fx/arrow_downright.png");
    m_arrows[2] = cocos2d::Sprite::create("elements/fx/arrow_upleft.png");
    m_arrows[3] = cocos2d::Sprite::create("elements/fx/arrow_upright.png");
    for (auto arrow : m_arrows)
    {
        arrow->setVisible(false);

        m_tiledMap.GetSpriteRoot()->addChild(arrow, 2);
    }

    return true;
}

void BtTownScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    if (m_isPlacingBuilding)
        return;

    auto touch = touches[0];
    cocos2d::Vec2 tileCoord;
    cocos2d::Vec2 tileCenter;
    if (!m_tiledMap.getTouchedTileInfo(touch, &tileCoord, &tileCenter))
        return;

    auto touchedPos = m_tiledMap.GetSpriteRoot()->convertTouchToNodeSpace(touch);

    bool selected = false;
    for (auto building : m_buildings)
    {
        cocos2d::Rect bbox = building->getBoundingBox();
        if (bbox.containsPoint(touchedPos))
        {
            if (building != m_selectedBuilding)
            {
                if (m_selectedBuilding)
                    m_selectedBuilding->deselect();

                m_selectedBuilding = building;
                m_selectedBuilding->select();
            }

            cocos2d::Vec2 tileCoordBuilding;
            if (m_tiledMap.getTileCoord(building->getPosition(), &tileCoordBuilding))
            {
                float arrowDist = 1.0f;
                btlua_ref dist = BT_CALL_LUA("get_building_arrowDist", building->getName());
                if (dist.isNumber())
                    arrowDist = (float) dist;

                cocos2d::Vec2 tilePos;
                if (m_tiledMap.getTilePosition(cocos2d::Vec2(tileCoordBuilding.x, tileCoordBuilding.y + arrowDist), &tilePos))
                    m_arrows[0]->setPosition(tilePos);
                if (m_tiledMap.getTilePosition(cocos2d::Vec2(tileCoordBuilding.x + arrowDist, tileCoordBuilding.y), &tilePos))
                    m_arrows[1]->setPosition(tilePos);
                if (m_tiledMap.getTilePosition(cocos2d::Vec2(tileCoordBuilding.x - arrowDist, tileCoordBuilding.y), &tilePos))
                    m_arrows[2]->setPosition(tilePos);
                if (m_tiledMap.getTilePosition(cocos2d::Vec2(tileCoordBuilding.x, tileCoordBuilding.y - arrowDist), &tilePos))
                    m_arrows[3]->setPosition(tilePos);

                float arrowScale = 1.0f;
                btlua_ref scale = BT_CALL_LUA("get_building_arrowScale", building->getName());
                if (scale.isNumber())
                    arrowScale = (float) scale;

                for (auto arrow : m_arrows)
                {
                    cocos2d::Size size = m_tiledMap.GetTileSize();
                    arrow->setScale(size.width / arrow->getContentSize().width * arrowScale, size.height / arrow->getContentSize().height * arrowScale);
                    arrow->setVisible(true);
                }
            }

            selected = true;
            break;
        }
    }

    if (!selected && m_selectedBuilding)
    {
        for (auto arrow : m_arrows)
            arrow->setVisible(false);

        m_selectedBuilding->deselect();
        m_selectedBuilding = nullptr;
    }
}

void BtTownScene::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    if (!m_isPlacingBuilding || m_placingBuildingName.empty())
        return;

    // support one touch only (ignore additional touches)
    auto touch = touches[0];
    if (touch->getStartLocation() != touch->getLocation())
        return; // return if onTouchesMoved() is called

    cocos2d::Vec2 tileCoord;
    cocos2d::Vec2 tileCenter;
    if (!m_tiledMap.getTouchedTileInfo(touch, &tileCoord, &tileCenter))
        return;

    btlua_ref image = BT_CALL_LUA("get_building_image", m_placingBuildingName);
    if (!image.isString())
        return;

    BtTownBuilding* building = BtTownBuilding::create(m_placingBuildingName, image.tostring(), 2);
    building->initDeco();
    building->setPosition(tileCenter);
    m_buildings.push_back(building);
    m_tiledMap.GetSpriteRoot()->addChild(building);

    m_isPlacingBuilding = false;
    m_placingBuildingName = "";
}

void BtTownScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
    m_tiledMap.onTouchesMoved(touches, event);
}

void BtTownScene::onMenu_Build(Ref* sender, const std::string& buildingName)
{
    m_placingBuildingName = buildingName;
    m_isPlacingBuilding = true;
}

