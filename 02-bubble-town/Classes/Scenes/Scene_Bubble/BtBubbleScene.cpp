/*!
 * \file BtBubbleScene.cpp
 * \date 2015/01/21 9:26
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtBubbleScene
*/

#include "stdafx.h"
#include "BtBubbleScene.h"

#include "Core/BtGuiUtil.h"

#include "Core/BtMsgDef.h"

#include "Services/BtLuaService.h"

bool BtBubbleScene::do_init()
{
    cocos2d::ui::Button* btLoot = cocos2d::ui::Button::create("done.png", "done.png");
    btLoot->setPosition(cocos2d::Director::getInstance()->getVisibleOrigin() + cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize()) / 2);
    m_uiRoot->addChild(btLoot, 1);
    BtSetButtonHandler(btLoot, std::bind(&BtBubbleScene::onLootPressed, this));
    return true;
}

void BtBubbleScene::do_enter()
{
    // read 'battleConfig' below for current level
    std::string battleConfig = m_preEnterConfig;
    CCLOG(battleConfig.c_str());
    // it would be cleared as soon as do_enter() ends
}

void BtBubbleScene::onLootPressed()
{
    
}

