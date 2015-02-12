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

BtBubbleScene::BtBubbleScene() 
    : m_btLoot(nullptr)
    , m_btRestart(nullptr)
    , m_btNext(nullptr)
    , m_labelPlaying(nullptr)
    , m_labelEnd(nullptr)
{

}

bool BtBubbleScene::do_init()
{
    cocos2d::Vec2 center = cocos2d::Director::getInstance()->getVisibleOrigin() + cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize()) / 2;

    auto makeTest = [&](const std::string& text, const cocos2d::Vec2& position, buttonHandler_t handler) -> cocos2d::ui::Button* {
        std::string res = tfm::format("ui_test/%s.png", text);
        auto bt = cocos2d::ui::Button::create(res, res);
        if (bt)
        {
            bt->setPosition(position);
            BtSetButtonHandler(bt, handler);
            m_uiRoot->addChild(bt, 1);
        }
        return bt;
    };

    auto createLabel = [&](const std::string& text) -> cocos2d::ui::Text*{
        auto label = cocos2d::ui::Text::create(text, "Arial", 16);
        label->setPosition(cocos2d::Vec2(center.x, center.y + 50));
        m_uiRoot->addChild(label, 1);
        return label;
    };

    m_btLoot    = makeTest("loot", center, std::bind(&BtBubbleScene::onButton_Loot, this));
    m_btRestart = makeTest("restart", cocos2d::Vec2(center.x - 20.0f, center.y - 50), std::bind(&BtBubbleScene::onButton_Restart, this));
    m_btNext    = makeTest("next", cocos2d::Vec2(center.x + 20.0f, center.y - 50), std::bind(&BtBubbleScene::onButton_Next, this));
    
    m_labelPlaying  = createLabel("Playing Screen");
    m_labelEnd      = createLabel("End Screen");

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
    m_labelPlaying->setVisible(!show);
    m_btLoot->setVisible(!show);

    m_labelEnd->setVisible(show);
    m_btRestart->setVisible(show);
    m_btNext->setVisible(show);
}

