/*!
 * \file AppStartScene.cpp
 * \date 2015/01/21 9:56
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of AppStartScene
*/

#include "stdafx.h"
#include "AppStartScene.h"
#include "AppMacros.h"

#include "Core/BtGuiUtil.h"
#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"

#include "Services/BtLuaService.h"
#include "Services/BtGui_utouch.h"

bool AppStartScene::init()
{
    if ( !Layer::init() )
        return false;
    
    auto root = cocos2d::Node::create();
    if (!root)
        return false;

    auto node = BtGui_utouch::LoadLayout("ui/titlescreen");
    root->addChild(node, 1);

    addChild(root, 1);
    return true;
}
