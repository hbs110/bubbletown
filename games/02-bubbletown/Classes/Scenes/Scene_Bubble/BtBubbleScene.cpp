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

#include <tinyformat.h>

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "BtBubbleTestUI.h"
#include "BtBubbleArena.h"

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
    : m_curPlayingLevel(BT_INVALID_ID)
    , m_testUI(nullptr)
    , m_arena(nullptr)
{

}

bool BtBubbleScene::do_init()
{
    m_testUI = new BtBubbleTestUI;
    if (!m_testUI->init(m_uiRoot))
        return false;

    BtSetButtonHandler(m_testUI->m_btLoot, std::bind(&BtBubbleScene::onButton_Loot, this));
    BtSetButtonHandler(m_testUI->m_btBackToTown, std::bind(&BtBubbleScene::onButton_BackToTown, this));
    BtSetButtonHandler(m_testUI->m_btRestart, std::bind(&BtBubbleScene::onButton_Restart, this));
    BtSetButtonHandler(m_testUI->m_btNext, std::bind(&BtBubbleScene::onButton_Next, this));

    m_arena = new BtBubbleArena;
    if (!m_arena->init())
        return false;

    addChild(m_arena->getCombatLayer(), 2);

    return true;
}

void BtBubbleScene::do_enter()
{
    m_testUI->showEndScreen(false);

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

    m_testUI->showEndScreen(true);
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

