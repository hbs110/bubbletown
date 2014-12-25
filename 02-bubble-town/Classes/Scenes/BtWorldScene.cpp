/*!
 * \file BtWorldScene.cpp
 * \date 2014/12/25 17:36
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtWorldScene
*/

#include "stdafx.h"
#include "BtWorldScene.h"

#include "AppMacros.h"

#include "BtSceneUtil.h"

cocos2d::Scene* BtWorldScene::scene()
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();

    // 'layer' is an autorelease object
    BtWorldScene *layer = BtWorldScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene                                           
    return scene;
}


bool BtWorldScene::init()
{
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
        CC_CALLBACK_1(BtWorldScene::menuCloseCallback,this));

    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);


    // add a "close" icon to exit the progress. it's an autorelease object
    auto worldButton = cocos2d::MenuItemImage::create(
        "world_button.png",
        "world_button.png",
        CC_CALLBACK_1(BtWorldScene::OnMenuItem,this));

    worldButton->setPosition(origin.x + visibleSize.width - worldButton->getContentSize().width / 2, origin.y + worldButton->getContentSize().height / 2);

    // create menu, it's an autorelease object
    auto worldMenu = cocos2d::Menu::create(worldButton, nullptr);
    worldMenu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(worldMenu, 1);


    auto label = cocos2d::LabelTTF::create("World Scene", "Arial", TITLE_FONT_SIZE);

    // position the label on the center of the screen
    label->setPosition(origin.x + visibleSize.width/2,
        origin.y + visibleSize.height - label->getContentSize().height);

    // add the label as a child to this layer
    this->addChild(label, 1);

    auto sprite = cocos2d::Sprite::create("world_bg.jpg");

    float ratio = visibleSize.height / visibleSize.width;
    cocos2d::Vec2 conSize = sprite->getContentSize();
    if (conSize.y / conSize.x < ratio)
    {
        conSize.x = conSize.y / ratio;
    } 
    else
    {
        conSize.y = conSize.x * ratio;
    }

    float scale = visibleSize.height / conSize.y;
    sprite->setScale(scale);    

    // position the sprite on the center of the screen
    sprite->setPosition(cocos2d::Vec2(visibleSize / 2) + origin);

    // add the sprite as a child to this layer
    this->addChild(sprite, -1);


    return true;
}

void BtWorldScene::menuCloseCallback(Ref* sender)
{
    BtMsgGotoScene_Emit(BTSCN_Start);
}

void BtWorldScene::OnMenuItem(cocos2d::Ref* sender)
{
    BtMsgGotoScene_Emit(BTSCN_Town);
}

