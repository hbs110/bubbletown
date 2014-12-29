/*!
 * \file TexturePoolTestScene.cpp
 * \date 2014/12/29 17:39
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TexturePoolTestScene
*/

#include "stdafx.h"
#include "TexturePoolTestScene.h"

#include "Scenes/BtSceneUtil.h"

bool TexturePoolTestScene::init()
{
    if ( !Layer::init() )
        return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = cocos2d::MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(TexturePoolTestScene::menuCloseCallback,this));

    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);


    auto sprite = cocos2d::Sprite::create("__test_texture_pool__/bang.png");
    sprite->setPosition(cocos2d::Vec2(200, 200));
    addChild(sprite, -1);

    return true;
}

cocos2d::Scene* TexturePoolTestScene::scene()
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();

    // 'layer' is an autorelease object
    cocos2d::Layer *layer = TexturePoolTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene                                           
    return scene;
}

void TexturePoolTestScene::menuCloseCallback(Ref* sender)
{
    BtMsgGotoScene_Emit(BTSCN_Start);
}
