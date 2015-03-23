/*!
 * \file BtBubbleTestUI.cpp
 * \date 2015/03/23 9:28
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtBubbleTestUI
*/

#include "stdafx.h"
#include "BtBubbleTestUI.h"

#include "Core/BtCoreUtil.h"
#include "Core/BtGuiUtil.h"
#include "Core/BtMsgDef.h"

#include "Services/BtLuaService.h"

#include <tinyformat.h>

BtBubbleTestUI::BtBubbleTestUI()
    : m_btLoot(nullptr)
    , m_btRestart(nullptr)
    , m_btNext(nullptr)
    , m_btBackToTown(nullptr)
    , m_labelPlaying(nullptr)
    , m_labelEnd(nullptr)
{

}

bool BtBubbleTestUI::init(cocos2d::Layer* uiRoot)
{
    cocos2d::Vec2 center = cocos2d::Director::getInstance()->getVisibleOrigin() + cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize()) / 2;

    auto makeTest = [&](const std::string& text, const cocos2d::Vec2& position) -> cocos2d::ui::Button* {
        std::string res = tfm::format("ui_test/%s.png", text);
        auto bt = cocos2d::ui::Button::create(res, res);
        if (bt)
        {
            bt->setName(text);
            bt->setPosition(position);
        }
        return bt;
    };

    auto createLabel = [&](const std::string& text) -> cocos2d::ui::Text*{
        auto label = cocos2d::ui::Text::create(text, "Arial", 16);
        label->setPosition(cocos2d::Vec2(center.x, center.y - 50));
        return label;
    };

    BT_EXPECT_RET(uiRoot, "bad parameter", false);

    m_btLoot = makeTest("loot", center);
    m_btBackToTown = makeTest("back", cocos2d::Vec2(center.x - 30.0f, center.y - 100));
    m_btRestart = makeTest("restart", cocos2d::Vec2(center.x, center.y - 100));
    m_btNext = makeTest("next", cocos2d::Vec2(center.x + 30.0f, center.y - 100));
    
    uiRoot->addChild(m_btLoot, 1);
    uiRoot->addChild(m_btBackToTown, 1);
    uiRoot->addChild(m_btRestart, 1);
    uiRoot->addChild(m_btNext, 1);

    m_labelPlaying = createLabel("Playing Screen");
    m_labelEnd = createLabel("End Screen");

    uiRoot->addChild(m_labelPlaying, 1);
    uiRoot->addChild(m_labelEnd, 1);

    return true;
}

void BtBubbleTestUI::showEndScreen(bool show)
{
    m_labelPlaying->setVisible(!show);
    m_btLoot->setVisible(!show);

    m_labelEnd->setVisible(show);
    m_btRestart->setVisible(show);
    m_btNext->setVisible(show);
    m_btBackToTown->setVisible(show);
}

