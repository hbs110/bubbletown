﻿/*!
 * \file BtBaseScene.cpp
 * \date 2015/01/21 9:15
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtBaseScene
*/

#include "stdafx.h"
#include "BtBaseScene.h"

#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"
#include "Core/BtGuiUtil.h"
#include "Core/BtCoreUtil.h"

#include "Services/BtLuaService.h"

bool BtBaseScene::init()
{
    if ( !Layer::init() )
        return false;

    m_sceneRoot = cocos2d::Layer::create();
    if (!m_sceneRoot)
        return false;
    addChild(m_sceneRoot, 1);

    m_uiRoot = cocos2d::Layer::create();
    if (!m_uiRoot)
        return false;
    addChild(m_uiRoot, 2);

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    cocos2d::ui::Button* btClose = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png");
    btClose->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(btClose->getContentSize() / 2));
    
    // the close button handler here should be moved to script
    BtSetButtonHandler(btClose, []() { BT_POST_LUA_AND_FLUSH(BtMsgID::GotoScene, BTSCN_start); });

    m_uiRoot->addChild(btClose, 1);

    if (!do_init())
        return false;

    return true;
}

void BtBaseScene::preEnter(const std::string& sceneConfig)
{
    m_preEnterConfig = sceneConfig;
}

void BtBaseScene::onEnter()
{
    cocos2d::Layer::onEnter();

    do_enter();

    m_preEnterConfig = "";
}

void BtBaseScene::onExit()
{
    cocos2d::Layer::onExit();

    do_exit();
}

void BtBaseScene::registerLuaHandler(cocos2d::ui::Button* widget, const std::string& name /*= ""*/)
{
    BT_EXPECT_RET_V2(widget, "invalid widget.");
    BT_EXPECT_RET_V2(name.size() || widget->getName().size(), "widget name not specified.");

    std::string scnName = getSceneName();
    std::string regName = name;
    if (regName.empty())
        regName = widget->getName();

    BtSetButtonHandler(widget, [scnName, regName]() { BT_POST_LUA_AND_FLUSH(BtMsgID::UI_ButtonPressed, scnName, regName); });
}
