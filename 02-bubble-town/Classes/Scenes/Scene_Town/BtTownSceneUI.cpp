/*!
 * \file BtTownSceneUI.cpp
 * \date 2015/02/13 17:00
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtTownSceneUI
*/

#include "stdafx.h"
#include "BtTownSceneUI.h"

#include "Core/BtGuiUtil.h"

#include "AppMacros.h"


BtTownSceneUI::BtTownSceneUI() 
    : m_btBuild(nullptr)
    , m_btUpgrade(nullptr)
{

}

bool BtTownSceneUI::init(cocos2d::Node* parent)
{
    if (!parent)
        return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto spacing = cocos2d::Vec2(10.0f, 10.0f);

    auto titleLabel = cocos2d::LabelTTF::create("TownScene", "Arial", TITLE_FONT_SIZE);
    titleLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - titleLabel->getContentSize().height);
    parent->addChild(titleLabel, 1);

    m_btBuild = cocos2d::ui::Button::create("ui_test/build.png", "ui_test/build.png");
    m_btBuild->setPosition(origin + spacing + cocos2d::Vec2(m_btBuild->getContentSize()) / 2);
    BtSetButtonHandler(m_btBuild, std::bind(&BtTownSceneUI::onButton_Build, this));
    parent->addChild(m_btBuild, 1);

    m_btUpgrade = cocos2d::ui::Button::create("ui_test/upgrade.png", "ui_test/upgrade.png");
    m_btUpgrade->setPositionX(origin.x + spacing.x * 2 + m_btUpgrade->getContentSize().width * 1.5f);
    m_btUpgrade->setPositionY(origin.y + spacing.y + m_btUpgrade->getContentSize().height / 2);
    m_btUpgrade->setVisible(false);
    BtSetButtonHandler(m_btUpgrade, std::bind(&BtTownSceneUI::onButton_Upgrade, this));
    parent->addChild(m_btUpgrade, 1);

    return true;
}

void BtTownSceneUI::onButton_Build()
{
}

void BtTownSceneUI::onButton_Upgrade()
{

}

void BtTownSceneUI::setUpgradeVisible(bool show)
{
    if (m_btUpgrade)
        m_btUpgrade->setVisible(show);
}
