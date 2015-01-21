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

    auto root = cocos2d::Node::create();
    if (!root)
        return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto callback = [] (cocos2d::Ref*) { BtEmitMessage(BTMSG_GotoScene, BTSCN_Start); };
    auto closeItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png", std::bind(callback, std::placeholders::_1));
    if (!closeItem)
        return false;
    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    if (!menu)
        return false;
    menu->setPosition(cocos2d::Vec2::ZERO);
    root->addChild(menu, 1);

    addChild(root, 1);
    m_uiRoot = root;

    if (!do_init())
        return false;

    return true;
}
