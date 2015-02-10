/*!
 * \file BtTownBuilding.cpp
 * \date 2015/01/22 14:28
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtTownBuilding
*/

#include "stdafx.h"
#include "BtTownBuilding.h"

#include "Core/BtGuiUtil.h"


BtTownBuilding* BtTownBuilding::create(const std::string& name, const std::string& resfile, int borderPixels)
{
    BtTownBuilding* sprite = new BtTownBuilding();
    if (!sprite->initWithFile(resfile))
    {
        delete sprite;
        return nullptr;
    }

    sprite->autorelease();

    if (borderPixels > 0)
    {
        auto sf = sprite->getSpriteFrame();
        cocos2d::SpriteFrame *frame = cocos2d::SpriteFrame::createWithTexture(sf->getTexture(), sf->getRect());
        auto rect = sf->getRectInPixels();
        rect.origin.x = borderPixels;
        rect.origin.y = borderPixels;
        rect.size.width -= borderPixels * 2;
        rect.size.height -= borderPixels * 2;
        frame->setRectInPixels(rect);
        sprite->setDisplayFrame(frame);
    }

    cocos2d::Size size = sprite->getContentSize();
    sprite->setAnchorPoint(cocos2d::Vec2(0.5f, size.width * 0.25f / size.height));
    sprite->setName(name);
    sprite->setOpacity(0);
    return sprite;
}

BtTownBuilding::BtTownBuilding() 
    : m_countDownLabel(nullptr)
    , m_countdown(0.0f)
{
}

bool BtTownBuilding::initDeco()
{
    auto label = cocos2d::ui::Text::create("0%", BtGui_DefaultMenuFont, BtGui_DefaultMenuFontSize);
    if (!label)
        return false;
    label->setPosition(cocos2d::Vec2(getContentSize().width / 2, 0.0f));
    addChild(label, 1);
    m_countDownLabel = label;
    m_countdown = 2.0f;

    scheduleUpdate();

    return true;
}

void BtTownBuilding::update(float delta)
{
    cocos2d::Sprite::update(delta);

    if (m_countdown > 0.0f)
    {
        m_countdown = std::max(0.0f, m_countdown - delta);

        float percent = (2.0f - m_countdown) / 2.0f;
        setOpacity((int)(255.0f * percent));

        if (m_countDownLabel)
        {
            std::ostringstream oss;
            oss << (int)(100.0f * percent);
            m_countDownLabel->setString(oss.str() + "%");

            if (BtIsZero(m_countdown))
            {
                unscheduleUpdate();
                removeChild(m_countDownLabel, true);
                m_countDownLabel = nullptr;
            }
        }
    }
}

void BtTownBuilding::playAnim_Selected()
{
    auto action = cocos2d::Sequence::create(
        cocos2d::ScaleTo::create(BtBuilding_DefaultAnimLength, BtBuilding_DefaultScale * 1.2f),
        cocos2d::ScaleTo::create(BtBuilding_DefaultAnimLength, BtBuilding_DefaultScale), nullptr);
    runAction(action);
}

void BtTownBuilding::select()
{
    m_isSelected = true;
    playAnim_Selected();
}

void BtTownBuilding::deselect()
{
    m_isSelected = false;
}

