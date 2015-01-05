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

cocos2d::Scene* BtTownScene::scene()
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();

    // 'layer' is an autorelease object
    BtTownScene *layer = BtTownScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene                                           
    return scene;
}

// on "init" you need to initialize your instance
bool BtTownScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = cocos2d::MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(BtTownScene::menuCloseCallback,this));

    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = cocos2d::LabelTTF::create("Town Scene", "Arial", TITLE_FONT_SIZE);

    // position the label on the center of the screen
    label->setPosition(origin.x + visibleSize.width/2,
        origin.y + visibleSize.height - label->getContentSize().height);

    // add the label as a child to this layer
    this->addChild(label, 1);


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
    mb.SetHandler(std::bind(&BtTownScene::OnMenuItem, this, std::placeholders::_1));
    cocos2d::Menu* menuBuild = mb.Build();
    if (menuBuild)
    {
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
        CC_CALLBACK_1(BtTownScene::OnMenuItem,this));

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


void BtTownScene::menuCloseCallback(Ref* sender)
{
    BtMsgGotoScene_Emit(BTSCN_Start);
}

void BtTownScene::OnMenuItem(Ref* sender)
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

