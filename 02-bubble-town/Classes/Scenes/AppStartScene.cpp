#include "stdafx.h"
#include "AppStartScene.h"
#include "AppMacros.h"

#include "Core/BtGuiUtil.h"

#include "MsgHandling/BtMsgDef.h"
#include "MsgHandling/BtMsgDispatcher.h"

#include "Scenes/BtSceneUtil.h"

const char* const MENU_Bubble  = "Bubble";
const char* const MENU_Town    = "Town";
const char* const MENU_World   = "World";
const char* const MENU_TexturePool = "TexturePool(Temp)";
const char* const MENU_TexturePool_Anim = "TexturePool(Anim)";

// on "init" you need to initialize your instance
bool AppStartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = cocos2d::MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(AppStartScene::menuCloseCallback,this));
    
    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = cocos2d::LabelTTF::create("Start Scene", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    label->setPosition(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height);

    // add the label as a child to this layer
    this->addChild(label, 1);


    BtTextMenuBuilder mb;
    mb.AddItem(MENU_Bubble);
    mb.AddItem(MENU_Town);
    mb.AddItem(MENU_World);
    mb.AddItem(MENU_TexturePool);
    mb.AddItem(MENU_TexturePool_Anim);
    mb.SetHandler(std::bind(&AppStartScene::OnMenuItem, this, std::placeholders::_1));
    cocos2d::Menu* menuMain = mb.Build();
    if (menuMain)
    {
        cocos2d::Vec2 menuPos;
        menuPos.x = origin.x + visibleSize.width / 2;
        menuPos.y = origin.y + visibleSize.height / 4;
        menuMain->setPosition(menuPos);
        addChild(menuMain, 1);
    }

    return true;
}

void AppStartScene::menuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    cocos2d::Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

AppStartScene::~AppStartScene()
{
}

void AppStartScene::OnMenuItem(Ref* sender)
{
    auto mi = dynamic_cast<cocos2d::MenuItemLabel*>(sender);
    if (!mi)
        return;

    auto label = dynamic_cast<cocos2d::LabelProtocol*>(mi->getLabel());
    if (!label)
        return;

    CCLOG("AppStartScene::OnMenuItem -> %s", label->getString().c_str());

    if (label->getString() == MENU_Bubble)
    {
        BtMsgGotoScene_Emit(BTSCN_Test);
    }
    else if (label->getString() == MENU_Town)
    {
        BtMsgGotoScene_Emit(BTSCN_Town);
    }
    else if (label->getString() == MENU_World)
    {
        BtMsgGotoScene_Emit(BTSCN_World);
    }
    else if (label->getString() == MENU_TexturePool)
    {
        BtMsgGotoScene_Emit(BTSCN_TexturePool);
    }
    else if (label->getString() == MENU_TexturePool_Anim)
    {
        BtMsgGotoScene_Emit(BTSCN_TexturePool_Anim);
    }
}
