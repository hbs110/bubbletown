#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "Core/BtCoreUtil.h"
#include "Core/BtMsgDispatcher.h"

#include "Services/BtLuaService.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();


protected:
    bool OnMsg_GotoScene(BtMsg& msg);

    typedef std::function<cocos2d::Scene* ()> sceneCreator_t;
    std::map<std::string, sceneCreator_t> m_sceneCreators;

    template <typename T>
    void RegisterSceneCreator()
    {
        m_sceneCreators[T::getSceneNameStatic()] = std::bind(&AppDelegate::CreateScene < T >, this);
    }

    template <typename T>
    cocos2d::Scene* CreateScene()
    {
        T *layer = T::create(); // create() should guarantee the layer pointer is autoreleased 
        if (!layer)
            return nullptr;
        layer->setName(T::getSceneNameStatic());

        auto scene = cocos2d::Scene::create();  // Scene::create() ensures creating autorelease object
        if (!scene)
            return nullptr;
        scene->addChild(layer);
        return scene;
    }

    bool CallLua_Init();
    void CallLua_Tick(float deltaSeconds);
    void CallLua_Destroy();

    void TickPerFrame(float deltaSeconds);
};

#endif // _APP_DELEGATE_H_

