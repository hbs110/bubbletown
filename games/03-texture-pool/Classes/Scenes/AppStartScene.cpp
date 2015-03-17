#include "stdafx.h"
#include "AppStartScene.h"
#include "AppMacros.h"

#include "Core/BtGuiUtil.h"

#include "MsgHandling/BtMsgDef.h"
#include "MsgHandling/BtMsgDispatcher.h"

#include "Scenes/BtSceneUtil.h"

const char* const MENU_TexturePool = "TexturePool(Temp)";
const char* const MENU_TexturePool_Anim = "TexturePool(Anim)";

// on "init" you need to initialize your instance
bool AppStartScene::init()
{
    if ( !Layer::init() )
        return false;
    
    auto root = BtCreateDefaultUIElements(std::bind(&BtStdHandler_QuitGame, std::placeholders::_1), BTSCN_Start);
    if (root)
        addChild(root, 1);
    
    BtTextMenuBuilder mb;
    mb.AddItem(MENU_TexturePool);
    mb.AddItem(MENU_TexturePool_Anim);
    mb.SetHandler(std::bind(&AppStartScene::onMenuItem, this, std::placeholders::_1));
    auto menuMain = mb.Build();
    if (menuMain)
    {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        cocos2d::Vec2 menuPos;
        menuPos.x = origin.x + visibleSize.width / 2;
        menuPos.y = origin.y + visibleSize.height / 4;
        menuMain->setPosition(menuPos);
        addChild(menuMain, 1);
    }

    return true;
}

AppStartScene::~AppStartScene()
{
}

void AppStartScene::onMenuItem(Ref* sender)
{
    auto mi = dynamic_cast<cocos2d::MenuItemLabel*>(sender);
    if (!mi)
        return;

    auto label = dynamic_cast<cocos2d::LabelProtocol*>(mi->getLabel());
    if (!label)
        return;

    CCLOG("AppStartScene::OnMenuItem -> %s", label->getString().c_str());

    if (label->getString() == MENU_TexturePool)
    {
        BtMsgGotoScene_Emit(BTSCN_TexturePool);
    }
    else if (label->getString() == MENU_TexturePool_Anim)
    {
        BtMsgGotoScene_Emit(BTSCN_TexturePool_Anim);
    }
}
