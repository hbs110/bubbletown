#include "stdafx.h"
#include "BtTestScene.h"
#include "AppMacros.h"
#include "AppStartScene.h"

std::pair<float, int> GShootSchedule[] = 
{
    std::make_pair(1.0f, 5),
    std::make_pair(2.0f, 10),
    std::make_pair(2.0f, 10),
    std::make_pair(2.0f, 10),
    std::make_pair(2.0f, 10),
    std::make_pair(1.0f, 5),
};

cocos2d::Scene* BtTestScene::scene()
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();
    
    // 'layer' is an autorelease object
    BtTestScene *layer = BtTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

BtTestScene::~BtTestScene()
{
    if (m_shootTimer)
    {
        delete m_shootTimer;
        m_shootTimer = NULL;
    }
}

// on "init" you need to initialize your instance
bool BtTestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    setKeyboardEnabled(true);

    m_movStatus = 0;
    m_velo = cocos2d::Vec2(0, 0);
    m_nextRound = 0;
    m_nextShootTime = 0.0f;

    m_shootTimer = new (std::nothrow) ShootTimer;
    m_shootTimer->setupTimerWithInterval(0.5, 4, 0.0f);
    m_shootTimer->onTrigger = std::bind(&BtTestScene::OnShootTriggered, this);
    m_shootTimer->onFinish = std::bind(&BtTestScene::OnShootFinished, this);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = cocos2d::MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(BtTestScene::menuCloseCallback,this));
    
    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::create(closeItem, NULL);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = cocos2d::LabelTTF::create("Hold 20 seconds to win!", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    label->setPosition(cocos2d::Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);


    // add the plane
    m_plane = cocos2d::Sprite::create("plane.png");
    m_plane->setPosition(cocos2d::Vec2(visibleSize / 2) + origin);
    addChild(m_plane, 2);

    // add bullets
    for (int i = 0; i < GAME_BulletNum; ++i)
    {
        cocos2d::Sprite* bullet = cocos2d::Sprite::create("bullet.png");
        bullet->setScale(0.3f);
        bullet->setVisible(false);
        addChild(bullet, 2);
        m_bulletPool.push_back(bullet);
    }

    // add background
    auto sprite = cocos2d::Sprite::create("stars.jpg");

    float ratio = visibleSize.height / visibleSize.width;
    cocos2d::Vec2 conSize = sprite->getContentSize();
    if (conSize.y / conSize.x < ratio)
    {
        conSize.x = conSize.y / ratio;
    } 
    else
    {
        conSize.y = conSize.x * ratio;
    }

    float scale = visibleSize.height / conSize.y;
    sprite->setScale(scale);    

    // position the sprite on the center of the screen
    sprite->setPosition(cocos2d::Vec2(visibleSize / 2) + origin);

    // add the sprite as a child to this layer
    this->addChild(sprite, -1);

    scheduleUpdate();

    return true;
}

void BtTestScene::menuCloseCallback(Ref* sender)
{
    auto director = cocos2d::Director::getInstance();
    director->replaceScene(AppStartScene::scene());
}

void BtTestScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A)
    {
        m_movStatus |= (unsigned int)MoveStatus::Left;
    }    
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D)
    {
        m_movStatus |= (unsigned int)MoveStatus::Right;
    }    
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_W)
    {
        m_movStatus |= (unsigned int)MoveStatus::Up;
    }    
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_S)
    {
        m_movStatus |= (unsigned int)MoveStatus::Down;
    }
}

void BtTestScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A)
    {
        m_movStatus &= ~(unsigned int)MoveStatus::Left;
    }    
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D)
    {
        m_movStatus &= ~(unsigned int)MoveStatus::Right;
    }    
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_W)
    {
        m_movStatus &= ~(unsigned int)MoveStatus::Up;
    }    
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_S)
    {
        m_movStatus &= ~(unsigned int)MoveStatus::Down;
    }    
}

void BtTestScene::update(float delta)
{
    Layer::update(delta);

    float speed = 100.0f;
    m_velo = cocos2d::Vec2::ZERO;
    if (m_movStatus & (unsigned int)MoveStatus::Left)
    {
        m_velo.x -= speed;
    } 
    if (m_movStatus & (unsigned int)MoveStatus::Right)
    {
        m_velo.x += speed;
    } 
    if (m_movStatus & (unsigned int)MoveStatus::Up)
    {
        m_velo.y += speed;
    } 
    if (m_movStatus & (unsigned int)MoveStatus::Down)
    {
        m_velo.y -= speed;
    } 

    if (!m_velo.isZero())
    {
        cocos2d::Vec2 pos = m_plane->getPosition();
        m_plane->setPosition(pos + m_velo * delta);
    }

    if (m_shootTimer)
    {
        m_shootTimer->update(delta);
    }

    for (ActiveBullet& ab : m_activeBullets)
    {
        cocos2d::Vec2 pos = ab.m_bullet->getPosition();
        ab.m_bullet->setPosition(pos + ab.m_velo * delta);

        if (IsOutOfScene(ab.m_bullet))
        {
            ab.m_bullet->setVisible(false);
        }
    }

    auto it = std::remove_if(m_activeBullets.begin(), m_activeBullets.end(),
        [&] (ActiveBullet& ab ) -> bool {
            bool shouldRemove = !ab.m_bullet->isVisible();
            if (shouldRemove)
            {
                m_bulletPool.push_back(ab.m_bullet);
            }
            return shouldRemove;
    });
    if (it != m_activeBullets.end())
    {
        m_activeBullets.erase(it, m_activeBullets.end());

        if (m_activeBullets.size() == 0)
        {
            m_shootTimer->setupTimerWithInterval(0.5, 4, 0.0f);
        }
    }
}

void BtTestScene::OnShootTriggered()
{
    CC_ASSERT(m_bulletPool.size() >= GAME_BulletNumOnce);
    
    static int style = 0;
    style = (++style) % 4;

    switch (style)
    {
    case 0:
        for (int i = 0; i < GAME_BulletNumOnce; ++i)
        {
            cocos2d::Sprite* bullet = m_bulletPool.back();
            bullet->setPosition(0.0f, 50.0f + i * 30);
            bullet->setVisible(true);
            m_bulletPool.pop_back();
            m_activeBullets.push_back(ActiveBullet(cocos2d::Vec2(rand() % 60 + 60, rand() % 20 - 20), bullet));
        }
        break;
    case 1:
        for (int i = 0; i < GAME_BulletNumOnce; ++i)
        {
            cocos2d::Sprite* bullet = m_bulletPool.back();
            bullet->setPosition(cocos2d::Director::getInstance()->getVisibleSize().width, 50.0f + i * 30);
            bullet->setVisible(true);
            m_bulletPool.pop_back();
            m_activeBullets.push_back(ActiveBullet(cocos2d::Vec2(-(rand() % 60 + 60), rand() % 20 - 20), bullet));
        }
        break;
    case 2:
        for (int i = 0; i < GAME_BulletNumOnce; ++i)
        {
            cocos2d::Sprite* bullet = m_bulletPool.back();
            bullet->setPosition(50.0f + i * 30, 0.0f);
            bullet->setVisible(true);
            m_bulletPool.pop_back();
            m_activeBullets.push_back(ActiveBullet(cocos2d::Vec2(rand() % 20 - 20, (rand() % 60 + 60)), bullet));
        }
        break;
    case 3:
        for (int i = 0; i < GAME_BulletNumOnce; ++i)
        {
            cocos2d::Sprite* bullet = m_bulletPool.back();
            bullet->setPosition(50.0f + i * 30, cocos2d::Director::getInstance()->getVisibleSize().height);
            bullet->setVisible(true);
            m_bulletPool.pop_back();
            m_activeBullets.push_back(ActiveBullet(cocos2d::Vec2(rand() % 20 - 20, -(rand() % 60 + 60)), bullet));
        }
        break;
    }
}

void BtTestScene::OnShootFinished()
{
    m_shootTimer->setupTimerWithInterval(kRepeatForever, 1, 0.0f);
}

bool BtTestScene::IsOutOfScene(cocos2d::Sprite* bullet)
{
    cocos2d::Vec2 pos = bullet->getPosition();
    cocos2d::Rect viewport;
    viewport.origin = cocos2d::Director::getInstance()->getVisibleOrigin() - cocos2d::Vec2(GAME_SceneBorderMargin, GAME_SceneBorderMargin);
    viewport.size = cocos2d::Director::getInstance()->getVisibleSize() + cocos2d::Size(GAME_SceneBorderMargin, GAME_SceneBorderMargin);
    return !viewport.containsPoint(pos);
}

void BtTestScene::OnMenuItem(cocos2d::Ref* sender)
{
    auto mi = dynamic_cast<cocos2d::MenuItemLabel*>(sender);
    if (!mi)
        return;

    auto label = dynamic_cast<cocos2d::LabelProtocol*>(mi->getLabel());
    if (!label)
        return;

    CCLOG("HelloWorld::OnMenuItem -> %s", label->getString().c_str());
}


