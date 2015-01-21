/*!
 * \file BtBaseScene.cpp
 * \date 2015/01/21 9:15
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtBaseScene
*/

#include "stdafx.h"
#include "BtBaseScene.h"

#include "BtSceneUtil.h"

#include "Core/BtMsgDef.h"

bool BtBaseScene::init()
{
    if ( !Layer::init() )
        return false;

    auto callback = [] (cocos2d::Ref*) { BtEmitMessage(BTMSG_GotoScene, BTSCN_Start); };
    auto root = BtCreateDefaultUIElements(std::bind(callback, std::placeholders::_1));
    if (!root)
        return false;

    addChild(root, 1);
    m_uiRoot = root;

    if (!do_init())
        return false;

    return true;
}
