#ifndef __BULLET_STORM_SCENE_H__
#define __BULLET_STORM_SCENE_H__

#include "cocos2d.h"

#include "GameGui.h"

enum class MoveStatus
{
    Left    = 1,
    Right   = 1 << 1,
    Up      = 1 << 2,
    Down    = 1 << 3,
};

const int GAME_BulletNum = 50;
const int GAME_BulletTriggerTimes = 5;
const int GAME_BulletNumOnce = GAME_BulletNum / GAME_BulletTriggerTimes;

const float GAME_SceneBorderMargin = 30.0f;

class ActiveBullet
{
public:
    cocos2d::Vec2 m_velo;
    cocos2d::Sprite * m_bullet;

    ActiveBullet(cocos2d::Vec2 velo, cocos2d::Sprite * bullet) : m_velo(velo), m_bullet(bullet) {}
};

class ShootTimer : public cocos2d::Timer
{
public:
    virtual void cancel()
    {
        if (onFinish)
        {
            onFinish();
        }
    }

    virtual void trigger()
    {
        if (onTrigger)
        {
            onTrigger();
        }
    }

    std::function<void ()> onTrigger;
    std::function<void ()> onFinish;
};

class BulletStormScene : public cocos2d::Layer, public GameGuiListener
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuCloseCallback(Ref* sender);

    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    virtual void update(float delta);

    // implement the "static node()" method manually
    CREATE_FUNC(BulletStormScene);

public:
    virtual ~BulletStormScene();

private:
    cocos2d::Sprite * m_plane;

    unsigned int m_movStatus;
    cocos2d::Vec2 m_velo;

    std::list< cocos2d::Sprite *> m_bulletPool;
    std::vector< ActiveBullet > m_activeBullets;

    int m_nextRound;
    float m_nextShootTime;

    ShootTimer* m_shootTimer;
    void OnShootTriggered();
    void OnShootFinished();

    bool IsOutOfScene(cocos2d::Sprite* bullet);

    GameGui* m_gui;

    virtual void OnMenuItem(Ref* sender);
};

#endif // __HELLOWORLD_SCENE_H__
