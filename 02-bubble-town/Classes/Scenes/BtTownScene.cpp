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

#include "Scenes/BtSceneUtil.h"
#include "MsgHandling/BtMsgDef.h"

enum {
    kTagTileMap = 1,
};

BtConstStr MI_Build = "Build";
BtConstStr MI_Clear = "Clear";

BtConstStr s_buildings[] = 
{
    "deco",
    "hall",
    "house",
    "shop",
    "workshop",
};

const int BT_TilemapFileMaxZ = 10;
const int BT_TilemapLogicalZ = BT_TilemapFileMaxZ + 10;


cocos2d::Vec2 BtGetTouchedTile(cocos2d::Touch* touch, cocos2d::experimental::TMXLayer* layer)
{
    if (!touch || !layer)
        return BT_INVALID_VEC2;

    cocos2d::Mat4 n2t = layer->tileToNodeTransform().getInversed();

    auto lLoc = layer->convertTouchToNodeSpace(touch);
    auto tLoc = PointApplyTransform(lLoc, n2t);
    tLoc = cocos2d::Vec2(BtRound(tLoc.x), BtRound(tLoc.y));
    //CCLOG("lLoc -> %.2f, %.2f", lLoc.x, lLoc.y);
    //CCLOG("tLoc -> %.2f, %.2f", tLoc.x, tLoc.y);
    return tLoc;
}

cocos2d::Sprite* BtCreateIsoSprite(const std::string& imagePath, int borderPixels = 0)
{
    cocos2d::Sprite* sprite = cocos2d::Sprite::create(imagePath);
    auto sf = sprite->getSpriteFrame();

    if (borderPixels > 0)
    {
        cocos2d::SpriteFrame *frame = cocos2d::SpriteFrame::createWithTexture(sf->getTexture(), sf->getRect());
        auto rect = sf->getRectInPixels();
        rect.origin.x = borderPixels;
        rect.origin.y = borderPixels;
        rect.size.width -= borderPixels * 2;
        rect.size.height -= borderPixels * 2;
        frame->setRectInPixels(rect);
        sprite->setDisplayFrame(frame);
    }

    cocos2d::Size size = sprite->getContentSize();
    sprite->setAnchorPoint(cocos2d::Vec2(0.5f, size.width * 0.25f / size.height));
    return sprite;
}

// on "init" you need to initialize your instance
bool BtTownScene::init()
{
    if ( !Layer::init() )
        return false;

    auto root = BtCreateDefaultUIElements(std::bind(&BtStdHandler_BackToMainMenu, std::placeholders::_1), BTSCN_Town);
    if (root)
        addChild(root, 1);

    auto map = tileMap_t::create("scn_town02/map_tiles.tmx");
    addChild(map, 0, kTagTileMap);
    m_tileMap = map;

    auto s = map->getContentSize();
    CCLOG("ContentSize: %f, %f", s.width,s.height);
    map->setPosition(cocos2d::Vec2(0, 0));

    auto listener = cocos2d::EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(BtTownScene::onTouchesEnded, this);
    listener->onTouchesMoved = CC_CALLBACK_2(BtTownScene::onTouchesMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    BtTextMenuBuilder mb;

    for (auto building : s_buildings)
    {
        mb.AddItem(std::string(MI_Build) + " " + building, std::bind(&BtTownScene::onMenu_Build, this, std::placeholders::_1, building));
    }
    mb.AddItem(MI_Clear, std::bind(&BtTownScene::onMenu_Clear, this, std::placeholders::_1));

    mb.SetItemAlign(BtTextMenuBuilder::Left);
    cocos2d::Menu* menuBuild = mb.Build();
    if (menuBuild)
    {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        cocos2d::Vec2 menuPos;
        menuPos.x = origin.x + 50;
        menuPos.y = origin.y + visibleSize.height - 100;
        menuBuild->setPosition(menuPos);

        addChild(menuBuild, 1);
    }

    // add a "close" icon to exit the progress. it's an autorelease object
    auto worldButton = cocos2d::MenuItemImage::create(
        "treasure-map-icon.png",
        "treasure-map-icon.png",
        CC_CALLBACK_1(BtTownScene::onMenuItem,this));

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    worldButton->setPosition(origin.x + visibleSize.width - worldButton->getContentSize().width / 2, worldButton->getContentSize().height / 2);

    // create menu, it's an autorelease object
    auto worldMenu = cocos2d::Menu::create(worldButton, nullptr);
    worldMenu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(worldMenu, 1);

    int maxZ = 0;
    for (auto child : m_tileMap->getChildren())
    {
        maxZ = std::max(maxZ, child->getLocalZOrder());
    }

    if (maxZ >= BT_TilemapFileMaxZ)
        return false;

    m_root = cocos2d::Node::create();
    m_tileMap->addChild(m_root, BT_TilemapLogicalZ);

    scheduleUpdate();

    return true;
}

void BtTownScene::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    if (!m_isPicking)
        return;

    // support one touch only (ignore additional touches)
    auto touch = touches[0];
    if (touch->getStartLocation() != touch->getLocation())
        return; // return if onTouchesMoved() is called

    auto layer = m_tileMap->getLayer("layer_background");
    auto tile = BtGetTouchedTile(touch, layer);
    if (tile == BT_INVALID_VEC2)
        return;

    CCLOG("tile -> %.2f, %.2f", tile.x, tile.y);
    auto pos = layer->getPositionAt(tile);

    m_townCenter = BtCreateIsoSprite(std::string("elements/") + m_currentBuildingName + ".png", 2);
    m_townCenter->setPosition(pos);

    m_townCenterLabel = cocos2d::LabelTTF::create("0%", "Arial", TITLE_FONT_SIZE);
    m_townCenterLabel->setPosition(m_townCenter->getContentSize().width / 2, 0.0f);
    m_townCenter->addChild(m_townCenterLabel, 1);
    m_townCenter->setOpacity(0);
    m_countdown = 2.0f;

    m_root->addChild(m_townCenter);
}

void BtTownScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
    auto touch = touches[0];

    auto diff = touch->getDelta();
    auto node = getChildByTag(kTagTileMap);
    auto currentPos = node->getPosition();
    node->setPosition(currentPos + diff);
}

void BtTownScene::onMenuItem(Ref* sender)
{
    auto image = dynamic_cast<cocos2d::MenuItemImage*>(sender);
    if (image)
    {
        BtEmitMessage(BTMSG_GotoScene, BTSCN_World);
        return;
    }

    auto mi = dynamic_cast<cocos2d::MenuItemLabel*>(sender);
    if (!mi)
        return;

    auto label = dynamic_cast<cocos2d::LabelProtocol*>(mi->getLabel());
    if (!label)
        return;

    CCLOG("TownScene::OnMenuItem -> %s", label->getString().c_str());
    
    if (label->getString() == MI_Clear)
    {
        auto layer = m_tileMap->getLayer("trees3");
        int tileID = layer->getTileGIDAt(cocos2d::Vec2(21, 28));
        layer->setTileGID(tileID, cocos2d::Vec2(20, 28));
        cocos2d::Sprite* sprite = layer->getTileAt(cocos2d::Vec2(20, 28));
        int z = layer->getLocalZOrder();
        CCLOG("local z -> %d", z);
        //m_tileMap->reorderChild(_tamara, newZ);    
    }
   
}

void BtTownScene::onMenu_Clear(Ref* sender)
{
    m_isPicking = false;
    CCLOG("m_isPicking -> %d", m_isPicking);
}

void BtTownScene::onMenu_Build(Ref* sender, const std::string& buildingName)
{
    CCLOG("Build touched.");
    m_currentBuildingName = buildingName;
    m_isPicking = true;
}

void BtTownScene::update(float delta)
{
    Layer::update(delta);

    if (m_townCenter && m_countdown > 0.0f)
    {
        m_countdown = std::max(0.0f, m_countdown - delta);

        float percent = (2.0f - m_countdown) / 2.0f;
        m_townCenter->setOpacity((int)(255.0f * percent));

        if (m_townCenterLabel)
        {
            m_townCenterLabel->setString(std::to_string((int)(100.0f * percent)) + "%");

            if (BtIsZero(m_countdown))
            {
                m_townCenter->removeChild(m_townCenterLabel, true);
                m_townCenterLabel = nullptr;
            }
        }
    }
}

