/*!
 * \file TownScene.cpp
 * \date 2014/12/22 17:08
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TownScene
*/

#include "stdafx.h"
#include "TownScene.h"

#include "AppMacros.h"
#include "AppStartScene.h"

#include "Core/BtCoreDef.h"
#include "Core/BtGuiUtil.h"

enum {
    kTagTileMap = 1,
};

Scene* TownScene::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    TownScene *layer = TownScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene                                           
    return scene;
}

// on "init" you need to initialize your instance
bool TownScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(TownScene::menuCloseCallback,this));

    closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = LabelTTF::create("Town Scene", "Arial", TITLE_FONT_SIZE);

    // position the label on the center of the screen
    label->setPosition(origin.x + visibleSize.width/2,
        origin.y + visibleSize.height - label->getContentSize().height);

    // add the label as a child to this layer
    this->addChild(label, 1);


    auto map = tileMap_t::create("scn_town/town.tmx");
    addChild(map, 0, kTagTileMap);
    m_tileMap = map;

    auto s = map->getContentSize();
    CCLOG("ContentSize: %f, %f", s.width,s.height);
    map->setPosition(Vec2(-s.width/2,0));

    //const Vector<TMXObjectGroup*> objectGroups = map->getObjectGroups();
    //CCLOG("Object Group Size: %d", objectGroups.size());

    auto layer = map->getLayer("trees3");
    int tileID = layer->getTileGIDAt(Vec2(21, 28));
    Sprite* sprite = layer->getTileAt(Vec2(21, 28));

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesMoved = CC_CALLBACK_2(TownScene::onTouchesMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    const char* itemTexts[] = {
        "build tree",
        "build grass",
    }; 

    cocos2d::Menu* menuBuild = BtGuiUtil::CreateMenu(itemTexts, BT_ARRAY_SIZE(itemTexts), this);

    Vec2 menuPos;
    menuPos.x = origin.x + 50;
    menuPos.y = origin.y + visibleSize.height - 100;
    menuBuild->setPosition(menuPos);

    addChild(menuBuild, 1);

    return true;
}

void TownScene::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    auto touch = touches[0];

    auto diff = touch->getDelta();
    auto node = getChildByTag(kTagTileMap);
    auto currentPos = node->getPosition();
    node->setPosition(currentPos + diff);
}


void TownScene::menuCloseCallback(Ref* sender)
{
    auto director = Director::getInstance();
    director->replaceScene(AppStartScene::scene());
}

void TownScene::OnMenuItem(Ref* sender)
{
    auto mi = dynamic_cast<MenuItemLabel*>(sender);
    if (!mi)
        return;

    auto label = dynamic_cast<LabelProtocol*>(mi->getLabel());
    if (!label)
        return;

    CCLOG("TownScene::OnMenuItem -> %s", label->getString().c_str());
    
    if (label->getString() == "build tree")
    {
        auto layer = m_tileMap->getLayer("trees3");
        int tileID = layer->getTileGIDAt(Vec2(21, 28));
        layer->setTileGID(tileID, Vec2(20, 28));
        Sprite* sprite = layer->getTileAt(Vec2(20, 28));
    }
    
}

