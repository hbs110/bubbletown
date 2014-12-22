#include "stdafx.h"
#include "AppStartScene.h"
#include "AppMacros.h"

#include "GameGuiDef.h"
#include "BulletStormScene.h"
#include "Scenes/TownScene.h"

USING_NS_CC;


Scene* AppStartScene::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    AppStartScene *layer = AppStartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool AppStartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    m_gui = new GameGui;
    if (!m_gui->init(this))
    {
        return false;
    }
    Menu* m = m_gui->getMenu(); 
    addChild(m, 1);


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(AppStartScene::menuCloseCallback,this));
    
    closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("App Start Scene", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    label->setPosition(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height);

    // add the label as a child to this layer
    this->addChild(label, 1);

    return true;
}

void AppStartScene::menuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

AppStartScene::~AppStartScene()
{
    if (m_gui)
    {
        delete m_gui;
        m_gui = NULL;
    }
}

void AppStartScene::OnMenuItem(Ref* sender)
{
    auto mi = dynamic_cast<MenuItemLabel*>(sender);
    if (!mi)
        return;

    auto label = dynamic_cast<LabelProtocol*>(mi->getLabel());
    if (!label)
        return;

    CCLOG("AppStartScene::OnMenuItem -> %s", label->getString().c_str());

    if (label->getString() == MENU_Bubble)
    {
        auto director = Director::getInstance();
        director->replaceScene(BulletStormScene::scene());
    }
    else if (label->getString() == MENU_Town)
    {
        auto director = Director::getInstance();
        director->replaceScene(TownScene::scene());
    }
}
