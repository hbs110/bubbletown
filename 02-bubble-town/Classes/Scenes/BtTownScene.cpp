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

enum {
    kTagTileMap = 1,
};

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

    //auto layer = map->getLayer("trees3");
    //int tileID = layer->getTileGIDAt(cocos2d::Vec2(21, 28));
    //cocos2d::Sprite* sprite = layer->getTileAt(cocos2d::Vec2(21, 28));

    auto listener = cocos2d::EventListenerTouchAllAtOnce::create();
    listener->onTouchesMoved = CC_CALLBACK_2(BtTownScene::onTouchesMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    BtTextMenuBuilder mb;
    mb.AddItem("build tree");
    mb.AddItem("build grass");
    mb.SetItemAlign(BtTextMenuBuilder::Left);
    mb.SetHandler(std::bind(&BtTownScene::onMenuItem, this, std::placeholders::_1));
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
        BtMsgGotoScene_Emit(BTSCN_World);
        return;
    }

    auto mi = dynamic_cast<cocos2d::MenuItemLabel*>(sender);
    if (!mi)
        return;

    auto label = dynamic_cast<cocos2d::LabelProtocol*>(mi->getLabel());
    if (!label)
        return;

    CCLOG("TownScene::OnMenuItem -> %s", label->getString().c_str());
    
    if (label->getString() == "build tree")
    {
        auto layer = m_tileMap->getLayer("trees3");
        int tileID = layer->getTileGIDAt(cocos2d::Vec2(21, 28));
        layer->setTileGID(tileID, cocos2d::Vec2(20, 28));
        cocos2d::Sprite* sprite = layer->getTileAt(cocos2d::Vec2(20, 28));
    }
   
}

