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

#include "Core/BtCoreDef.h"
#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"

#include "Services/BtLuaService.h"

BtConstStr Bt_CloseButton = "close";
BtConstStr Bt_SwitchButton = "switch";

bool BtWorldScene::do_init()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto titleLabel = cocos2d::LabelTTF::create("WorldScene", "Arial", TITLE_FONT_SIZE);
    titleLabel->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height - titleLabel->getContentSize().height);
    m_uiRoot->addChild(titleLabel, 1);

    auto callback = [](cocos2d::Ref*) { BT_POST_LUA_WITH_FLUSH(BtMsgID::GotoScene, BTSCN_town); };
    auto worldButton = cocos2d::MenuItemImage::create(
        "world_button.png",
        "world_button.png",
        std::bind(callback, std::placeholders::_1));
    worldButton->setName(Bt_SwitchButton);
    worldButton->setPosition(origin.x + visibleSize.width - worldButton->getContentSize().width / 2, origin.y + worldButton->getContentSize().height / 2);

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::create(worldButton, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);


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
