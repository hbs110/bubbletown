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

#include "Core/tinyformat/tinyformat.h"

cocos2d::ui::Button* makeTest(const char* text, buttonHandler_t handler)
{
    std::string res = tfm::format("ui_test/%s.png", text);
    auto bt = cocos2d::ui::Button::create(res, res);
    if (!bt)
        return nullptr;

    BtSetButtonHandler(bt, handler);
    return bt;
}

BtBubbleScene::BtBubbleScene() 
    : m_btLoot(nullptr)
    , m_btRestart(nullptr)
    , m_btNext(nullptr)
{

}

bool BtBubbleScene::do_init()
{
    // loot button
    m_btLoot = makeTest("loot", std::bind(&BtBubbleScene::onButton_Loot, this));
    m_btLoot->setPosition(cocos2d::Director::getInstance()->getVisibleOrigin() + cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize()) / 2);
    m_uiRoot->addChild(m_btLoot, 1);

    // restart button
    m_btRestart = makeTest("restart", std::bind(&BtBubbleScene::onButton_Restart, this));
    m_btRestart->setPosition(cocos2d::Vec2(m_btLoot->getPositionX() - 20.0f, m_btLoot->getPositionY() - 50));
    m_uiRoot->addChild(m_btRestart, 1);

    // next button
    m_btNext = makeTest("next", std::bind(&BtBubbleScene::onButton_Next, this));
    m_btNext->setPosition(cocos2d::Vec2(m_btLoot->getPositionX() + 20.0f, m_btLoot->getPositionY() - 50));
    m_uiRoot->addChild(m_btNext, 1);

    return true;
}

void BtBubbleScene::do_enter()
{
    // read 'battleConfig' below for current level
    std::string battleConfig = m_preEnterConfig;
    CCLOG(battleConfig.c_str());
    // it would be cleared as soon as do_enter() ends

    showEndScreen(false);

    // report the level id (should be available now)
    // the game knows we are playing a level now
    int currentLevel = 1; 
    BT_POST_LUA_AND_FLUSH(BtMsgID::LevelEntered, "", currentLevel);
}

void BtBubbleScene::do_exit()
{
    // notify the game that we are leaving
    BT_POST_LUA_AND_FLUSH(BtMsgID::LevelLeft, "");
}

void BtBubbleScene::onButton_Loot()
{
    // sending test rewards data
    std::string rewards = "{  \"coins\": 10, \"exp\": 5 }";
    BT_POST_LUA_AND_FLUSH(BtMsgID::LevelRewards, rewards);

    showEndScreen(true);
}

void BtBubbleScene::onButton_Restart()
{
    BT_POST_LUA_AND_FLUSH(BtMsgID::RestartLevel, "");
}

void BtBubbleScene::onButton_Next()
{
    BT_POST_LUA_AND_FLUSH(BtMsgID::StartNextLevel, "");
}

void BtBubbleScene::showEndScreen(bool show)
{
    m_btLoot->setVisible(!show);
    m_btRestart->setVisible(show);
    m_btNext->setVisible(show);
}

