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

    return true;
}

void BtTownScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    auto touch = touches[0];
    auto touchedPos = m_tiledMap.GetSpriteRoot()->convertTouchToNodeSpace(touch);

    for (auto building : m_buildings)
    {
        cocos2d::Rect bbox = building->getBoundingBox();
        if (bbox.containsPoint(touchedPos))
        {
            building->playAnim_Selected();
            break;
        }
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

    BtTownBuilding* building = BtTownBuilding::create(m_placingBuildingName, std::string("elements/") + m_placingBuildingName + ".png", 2);
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
