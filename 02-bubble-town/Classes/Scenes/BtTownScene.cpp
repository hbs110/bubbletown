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

BtConstStr MI_Pick = "Pick";
BtConstStr MI_Build = "Build";


template <typename T>
T BtRound(T value)
{
    T base = floor(value);
    if (value < base + T(0.5f))
    {
        return base;
    } 
    else
    {
        return base + T(1.0f);
    }
}

cocos2d::Vec2 BtGetTouchedTile(cocos2d::Touch* touch, cocos2d::experimental::TMXLayer* layer)
{
    if (!touch || !layer)
        return BT_INVALID_VEC2;

    cocos2d::Mat4 n2t = layer->tileToNodeTransform().getInversed();

    auto lLoc = layer->convertTouchToNodeSpace(touch);
    auto tLoc = PointApplyTransform(lLoc, n2t);
    tLoc = cocos2d::Vec2(BtRound(tLoc.x), BtRound(tLoc.y) + 1);
    //CCLOG("lLoc -> %.2f, %.2f", lLoc.x, lLoc.y);
    //CCLOG("tLoc -> %.2f, %.2f", tLoc.x, tLoc.y);
    return tLoc;
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
    mb.AddItem(MI_Pick, std::bind(&BtTownScene::onMenu_Pick, this, std::placeholders::_1));
    mb.AddItem(MI_Build, std::bind(&BtTownScene::onMenu_Build, this, std::placeholders::_1));
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
        "world_button.png",
        "world_button.png",
        CC_CALLBACK_1(BtTownScene::onMenuItem,this));

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    worldButton->setPosition(origin.x + visibleSize.width - worldButton->getContentSize().width / 2, worldButton->getContentSize().height / 2);

    // create menu, it's an autorelease object
    auto worldMenu = cocos2d::Menu::create(worldButton, nullptr);
    worldMenu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(worldMenu, 1);

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
    
    if (label->getString() == MI_Pick)
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

void BtTownScene::onMenu_Pick(Ref* sender)
{
    m_isPicking = !m_isPicking;
    CCLOG("m_isPicking -> %d", m_isPicking);
}

void BtTownScene::onMenu_Build(Ref* sender)
{
    CCLOG("Build touched.");
}

