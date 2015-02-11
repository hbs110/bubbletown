/*!
 * \file BtBubbleScene.cpp
 * \date 2015/01/21 9:26
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtBubbleScene
*/

#include "stdafx.h"
#include "BtBubbleScene.h"

#include "Core/BtGuiUtil.h"

#include "Core/BtMsgDef.h"

#include "Services/BtLuaService.h"

bool BtBubbleScene::do_init()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    cocos2d::ui::Button* btLoot = cocos2d::ui::Button::create("done.png", "done.png");
    btLoot->setName("Loot");
    btLoot->setPosition(origin + cocos2d::Vec2(visibleSize) / 2);
    m_uiRoot->addChild(btLoot, 1);

    BtSetButtonHandler(btLoot, [btLoot]()
    {
        BT_POST_LUA_WITH_FLUSH((int) BtMsgID::UI_ButtonPressed, BTSCN_bubble, btLoot->getName());
    });

    return true;
}
