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

#include "Scenes/BtSceneDef.h"

bool BtBaseScene::init()
{
    if ( !Layer::init() )
        return false;

    auto root = cocos2d::Layer::create();
    if (!root)
        return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto callback = [] (Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
    { 
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
        {
            BtEmitMessage(BTMSG_GotoScene, BTSCN_Start);
        }
    };

    cocos2d::ui::Button* btClose = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png");
    btClose->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(btClose->getContentSize() / 2));
    btClose->addTouchEventListener(std::bind(callback, std::placeholders::_1, std::placeholders::_2));
    root->addChild(btClose, 1);

    addChild(root, 1);
    m_uiRoot = root;

    if (!do_init())
        return false;

    return true;
}
