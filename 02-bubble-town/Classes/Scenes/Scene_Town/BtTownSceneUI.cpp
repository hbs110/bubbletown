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

#include "Services/BtLuaService.h"

#include "Core/BtMsgDef.h"

#include "Core/tinyformat/tinyformat.h"

BtTownSceneUI::BtTownSceneUI() 
    : m_btBuild(nullptr)
    , m_btUpgrade(nullptr)
    , m_btMenuBuild(nullptr)
    , m_playerInfo(nullptr)
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

    // get building list from lua
    std::vector<std::string> buildings;
    BtLua_Iterate(luabridge::getGlobal(BT_L, "t_buildings"), [&](btlua_ref key, btlua_ref value) {buildings.push_back(key.tostring());});

    BtTextMenuBuilder mb;
    mb.SetTextSize(12);
    mb.SetItemAlign(BtTextMenuBuilder::Left);
    for (auto& b : buildings)
        mb.AddItem(b, std::bind(&BtTownSceneUI::onMenu_Build, this, b));
    m_btMenuBuild = mb.Build();
    if (!m_btMenuBuild)
        return false;

    m_btMenuBuild->setVisible(false);
    m_btMenuBuild->setPosition(cocos2d::Vec2(0, m_btBuild->getSize().height + 50));
    m_btBuild->addChild(m_btMenuBuild, 1);

    cocos2d::ui::Button* btNext = cocos2d::ui::Button::create("ui_test/next.png", "ui_test/next.png");
    btNext->setPosition(cocos2d::Vec2(origin.x + visibleSize.width - btNext->getContentSize().width * 1.5 - spacing.x * 2, btNext->getContentSize().height / 2 + spacing.y));
    BtSetButtonHandler(btNext, []() { BT_POST_LUA_AND_FLUSH(BtMsgID::StartNextLevel, ""); });
    parent->addChild(btNext, 1);

    cocos2d::ui::Button* btWorld = cocos2d::ui::Button::create("ui_test/world.png", "ui_test/world.png");
    btWorld->setPosition(cocos2d::Vec2(origin.x + visibleSize.width - btWorld->getContentSize().width / 2 - spacing.x, btWorld->getContentSize().height / 2 + spacing.y));
    BtSetButtonHandler(btWorld, []() { BT_POST_LUA_AND_FLUSH(BtMsgID::GotoScene, BTSCN_world); });
    parent->addChild(btWorld, 1);

    m_playerInfo = cocos2d::ui::Text::create("test", "Arial", 6);
    m_playerInfo->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));
    m_playerInfo->setPosition(origin + cocos2d::Vec2(spacing.x, visibleSize.height - 50));
    parent->addChild(m_playerInfo, 1);

    return true;
}

void BtTownSceneUI::onButton_Build()
{
    m_btMenuBuild->setVisible(!m_btMenuBuild->isVisible());
}

void BtTownSceneUI::onButton_Upgrade()
{

}

void BtTownSceneUI::setUpgradeVisible(bool show)
{
    if (m_btUpgrade)
        m_btUpgrade->setVisible(show);
}

void BtTownSceneUI::onMenu_Build(const std::string& buildingName)
{
    if (m_onPlacingBuildingBegan)
    {
        m_onPlacingBuildingBegan(buildingName);
    }
}

void BtTownSceneUI::setPlayerProperty(const std::string& playerProperty, int value, bool refresh /*= true*/)
{
    m_displayedPlayerProperties[playerProperty] = value;

    if (refresh)
    {
        refreshDisplayedPlayerProperties();
    }
}

void BtTownSceneUI::refreshDisplayedPlayerProperties()
{
    std::string composed;
    for (auto& p : m_displayedPlayerProperties)
        composed += tfm::format("%s: %d  ", p.first, p.second);
    m_playerInfo->setText(composed);
}
