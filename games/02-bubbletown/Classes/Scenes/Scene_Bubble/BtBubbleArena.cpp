/*!
 * \file BtBubbleArena.cpp
 * \date 2015/03/23 9:21
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtBubbleArena
*/

#include "stdafx.h"
#include "BtBubbleArena.h"

#include "Services/BtLuaService.h"

#include "Core/BtCoreDef.h"
#include "Core/BtGuiUtil.h"

const float d_combat_frame_ratio = 0.625f;	// 16:10
const float d_combat_calibration_y = 0.4f;
const float d_combat_squad_front_x = 0.3f;
const float d_combat_squad_middle_x = 0.2f;
const float d_combat_squad_back_x = 0.1f;



cocos2d::Sprite* BtCreateSprite(const char* luaFuncForPath)
{
    btlua_ref ret = BT_CALL_LUA(luaFuncForPath);
    if (!ret.isString())
        return nullptr;

    cocos2d::Sprite* sp = cocos2d::Sprite::create(ret.tostring());
    return sp;
}

BtBubbleArena::BtBubbleArena()
    : m_combat(nullptr)
{

}

bool BtBubbleArena::init()
{
    m_combat = cocos2d::Layer::create();

    float arenaHeight = BtGetVisibleWidth() * d_combat_frame_ratio;
    cocos2d::Node* arenaRoot = cocos2d::Node::create();
    arenaRoot->setAnchorPoint(cocos2d::Vec2(0, 0));
    arenaRoot->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleOrigin().x, 
        BtGetVisibleTop() - arenaHeight));
    m_combat->addChild(arenaRoot);

    cocos2d::Sprite* sp = BtCreateSprite("get_combat_background");
    sp->setAnchorPoint(cocos2d::Vec2(0, 0));
    sp->setPosition(cocos2d::Vec2::ZERO);
    sp->setScale(BtGetVisibleWidth() / sp->getContentSize().width);
    arenaRoot->addChild(sp);

    cocos2d::Sprite* spVS = BtCreateSprite("get_combat_vs");
    spVS->setPositionX(BtGetVisibleWidth() / 2);
    spVS->setPositionY(arenaHeight * d_combat_calibration_y);
    arenaRoot->addChild(spVS);

    auto createCard = [](int num, const cocos2d::Vec2& pos, cocos2d::Node* parent) -> cocos2d::Sprite* {
        if (!parent)
            return nullptr;

        btlua_ref ret = BT_CALL_LUA("get_combat_card_path", num);
        if (!ret.isString())
            return nullptr;

        cocos2d::Sprite* card = cocos2d::Sprite::create(ret.tostring());
        if (!card)
            return nullptr;

        auto label = cocos2d::ui::Text::create("10", BtGui_DefaultMenuFont, 10);
        label->setPosition(cocos2d::Vec2(0.0f, card->getContentSize().height));
        card->addChild(label);

        card->setPosition(pos);
        parent->addChild(card);
        return card;
    };

    // player
    {
        auto card1 = createCard(1, cocos2d::Vec2(BtGetVisibleWidth() * d_combat_squad_front_x, arenaHeight * d_combat_calibration_y), arenaRoot);
        auto card2 = createCard(2, cocos2d::Vec2(BtGetVisibleWidth() * d_combat_squad_middle_x, arenaHeight * d_combat_calibration_y - 20), arenaRoot);
        auto card3 = createCard(3, cocos2d::Vec2(BtGetVisibleWidth() * d_combat_squad_middle_x, arenaHeight * d_combat_calibration_y + 20), arenaRoot);
        auto card4 = createCard(4, cocos2d::Vec2(BtGetVisibleWidth() * d_combat_squad_back_x, arenaHeight * d_combat_calibration_y - 20), arenaRoot);
        auto card5 = createCard(5, cocos2d::Vec2(BtGetVisibleWidth() * d_combat_squad_back_x, arenaHeight * d_combat_calibration_y + 20), arenaRoot);
    }

    // AI
    {
        auto card1 = createCard(1, cocos2d::Vec2(BtGetVisibleWidth() * (1 - d_combat_squad_front_x), arenaHeight * d_combat_calibration_y), arenaRoot);
        auto card2 = createCard(2, cocos2d::Vec2(BtGetVisibleWidth() * (1 - d_combat_squad_middle_x), arenaHeight * d_combat_calibration_y - 20), arenaRoot);
        auto card3 = createCard(3, cocos2d::Vec2(BtGetVisibleWidth() * (1 - d_combat_squad_middle_x), arenaHeight * d_combat_calibration_y + 20), arenaRoot);
        auto card4 = createCard(4, cocos2d::Vec2(BtGetVisibleWidth() * (1 - d_combat_squad_back_x), arenaHeight * d_combat_calibration_y - 20), arenaRoot);
        auto card5 = createCard(5, cocos2d::Vec2(BtGetVisibleWidth() * (1 - d_combat_squad_back_x), arenaHeight * d_combat_calibration_y + 20), arenaRoot);
    }

    return true;
}
