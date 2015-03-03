/*!
 * \file BtBubbleScene.cpp
 * \date 2015/01/21 9:26
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtBubbleScene
*/

#include "stdafx.h"
#include "BtBubbleScene.h"

#include "Core/BtCoreUtil.h"
#include "Core/BtGuiUtil.h"
#include "Core/BtMsgDef.h"

#include "Services/BtLuaService.h"

#include "Core/tinyformat/tinyformat.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

static std::string _json_doc_to_string(const rapidjson::Document& doc)
{
    using namespace rapidjson;
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

// for unexpected error when entering bubble scene: 
//      when something is unexpectedly, seriously and horribly wrong, 
//      we would try to recover from the situation and return to the town scene immediately
static auto fnBackToTown = []() { BT_POST_LUA_AND_FLUSH(BtMsgID::GotoScene, BTSCN_town); };
#define BT_BubbleSceneValidate(expr, errMsg)  \
    BT_EXPECT_RET(expr, errMsg, fnBackToTown())   // fnBackToTown() is called immediately so BT_EXPECT_RET() still returns nothing

BtBubbleScene::BtBubbleScene() 
    : m_btLoot(nullptr)
    , m_btRestart(nullptr)
    , m_btNext(nullptr)
    , m_btBackToTown(nullptr)
    , m_labelPlaying(nullptr)
    , m_labelEnd(nullptr)
    , m_curPlayingLevel(BT_INVALID_ID)
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
            bt->setName(text);
            bt->setPosition(position);
            if (handler)
            {
                BtSetButtonHandler(bt, handler);
            }
            else
            {
                registerLuaHandler(bt);
            }
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
    m_btBackToTown = makeTest("back", cocos2d::Vec2(center.x - 30.0f, center.y - 50), std::bind(&BtBubbleScene::onButton_BackToTown, this));
    m_btRestart = makeTest("restart", cocos2d::Vec2(center.x, center.y - 50), std::bind(&BtBubbleScene::onButton_Restart, this));
    m_btNext = makeTest("next", cocos2d::Vec2(center.x + 30.0f, center.y - 50), std::bind(&BtBubbleScene::onButton_Next, this));

    m_labelPlaying  = createLabel("Playing Screen");
    m_labelEnd      = createLabel("End Screen");

    return true;
}

void BtBubbleScene::do_enter()
{
    showEndScreen(false);

    // read 'battleConfig' below for current level
    std::string battleConfig = m_preEnterConfig;
    BT_BubbleSceneValidate(battleConfig.size() > 0, "scene config is empty.");
    // it would be cleared as soon as do_enter() ends

    // parsing the json and extracting the level id
    rapidjson::Document doc;
    doc.Parse<0>(battleConfig.c_str());
    BT_BubbleSceneValidate(doc.IsObject(), "invalid scene config json: (not a 'json_object').");
    BT_BubbleSceneValidate(doc["level_id"].IsInt(), "invalid scene config json: (invalid 'level_id': not a 'json_integer').");
    m_curPlayingLevel = doc["level_id"].GetInt();

    // notify the game
    BT_POST_LUA_AND_FLUSH(BtMsgID::LevelEntered, "", m_curPlayingLevel); // report the level id, so that the game knows we are playing a level now
}

void BtBubbleScene::do_exit()
{
    m_curPlayingLevel = BT_INVALID_ID;
    // notify the game that we are leaving
    BT_POST_LUA_AND_FLUSH(BtMsgID::LevelLeft, "");
}

void BtBubbleScene::onButton_Loot()
{
    auto getter = [](const char* func, int levelID) {
        btlua_ref ret = BT_CALL_LUA(func, levelID);
        return !ret.isNil() ? ret.cast<int>() : 0;
    };
    // get level rewards from lua
    int coins = getter("level_get_reward_coins", m_curPlayingLevel);
    int exp = getter("level_get_reward_exp", m_curPlayingLevel);
    int heroes = getter("level_get_reward_heroes", m_curPlayingLevel);

    // send rewards and stats back to game
    rapidjson::Document doc;
    doc.SetObject();
    {
        rapidjson::Value rewards(rapidjson::kObjectType);
        rewards.AddMember("coins", coins, doc.GetAllocator());
        rewards.AddMember("exp", exp, doc.GetAllocator());
        rewards.AddMember("heroes", heroes, doc.GetAllocator());
        doc.AddMember("level_rewards", rewards, doc.GetAllocator());

        rapidjson::Value stats(rapidjson::kObjectType); // faked data
        stats.AddMember("stars", 1, doc.GetAllocator());
        stats.AddMember("score", 1000 + rand() % 500, doc.GetAllocator());
        stats.AddMember("time_spent", 12.5f + (float) (rand() % 20), doc.GetAllocator());
        doc.AddMember("level_stats", stats, doc.GetAllocator());
    }
    BT_POST_LUA_AND_FLUSH(BtMsgID::LevelCompleted, _json_doc_to_string(doc));

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

void BtBubbleScene::onButton_BackToTown()
{
    BT_POST_LUA_AND_FLUSH(BtMsgID::GotoScene, BTSCN_town);
}

void BtBubbleScene::showEndScreen(bool show)
{
    m_labelPlaying->setVisible(!show);
    m_btLoot->setVisible(!show);

    m_labelEnd->setVisible(show);
    m_btRestart->setVisible(show);
    m_btNext->setVisible(show);
    m_btBackToTown->setVisible(show);
}

