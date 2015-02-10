/*!
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
    BtSetButtonHandler(btClose, []() { BT_CALL_LUA("goto_scene", "scn_start"); });

    m_uiRoot->addChild(btClose, 1);

    if (!do_init())
        return false;

    return true;
}
