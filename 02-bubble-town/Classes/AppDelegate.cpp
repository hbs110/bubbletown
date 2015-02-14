#include "stdafx.h"
#include "AppDelegate.h"
#include "AppMacros.h"
#include "AppStartScene.h"

#include "Scenes/Scene_Bubble/BtBubbleScene.h"
#include "Scenes/Scene_Town/BtTownScene.h"
#include "Scenes/Scene_World/BtWorldScene.h"

#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"

#include "Services/BtLuaService.h"

#include "AppNativeInterfaces.h"

AppDelegate::AppDelegate()
    : m_startTime(0.0)
{

}

AppDelegate::~AppDelegate() 
{
    CallLua_Destroy();
    BtLuaService::DestroyInst();
    BtMsgDispatcher::DestroyInst();
}

bool AppDelegate::applicationDidFinishLaunching() 
{
    // message handling
    BtMsgDispatcher::CreateInst();
    BtMsgDispatcher::Get()->subscribe((int)BtMsgID::GotoScene, std::bind(&AppDelegate::OnMsg_GotoScene, this, std::placeholders::_1));

    // initialize director
    auto director = cocos2d::Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = cocos2d::GLViewImpl::createWithRect("bubbletown", cocos2d::Rect(0, 0, 720, 1280), 0.5);
        director->setOpenGLView(glview);
    }

    director->setOpenGLView(glview);

    // Set the design resolution
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    // a bug in DirectX 11 level9-x on the device prevents ResolutionPolicy::NO_BORDER from working correctly
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
#else
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
#endif

	cocos2d::Size frameSize = glview->getFrameSize();
    
    std::vector<std::string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{
        searchPath.push_back(largeResource.directory);

        director->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(mediumResource.directory);
        
        director->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPath.push_back(smallResource.directory);

        director->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
    
    // set searching path
    cocos2d::FileUtils::getInstance()->setSearchPaths(searchPath);
	
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    m_startTime = cocos2d::utils::gettime();
    
    BtLuaService::CreateInst();
    if (!BtLuaService::Get()->Init())
        return false;

    BtLuaService::Get()->RegisterFunction("goto_scene", &AppNativeInterfaces::GotoScene);
    BtLuaService::Get()->RegisterFunction("get_current_time", &AppNativeInterfaces::GetCurrentGameTime);

    RegisterSceneCreator<AppStartScene>();
    RegisterSceneCreator<BtBubbleScene>();
    RegisterSceneCreator<BtTownScene>();
    RegisterSceneCreator<BtWorldScene>();

    auto luaTick = std::bind(&AppDelegate::CallLua_Tick, this, std::placeholders::_1);
    cocos2d::Director::getInstance()->getScheduler()->schedule(luaTick, this, 1.0f, false, "AppDelegate::TickLua_PerSecond");

    auto msgTick = std::bind(&AppDelegate::TickMsgDispatcher, this, std::placeholders::_1);
    cocos2d::Director::getInstance()->getScheduler()->schedule(msgTick, this, 0.0f, false, "AppDelegate::TickMsgDispatcher_PerFrame");

    if (!CallLua_Init())
        return false;

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    cocos2d::Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    cocos2d::Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

bool AppDelegate::OnMsg_GotoScene(BtMsg& msg)
{
    auto director = cocos2d::Director::getInstance();
    if (!director)
        return false;

    if (msg.m_extraParams.size() != 1)
        return false;

    auto it = m_sceneCreators.find(msg.m_info);
    if (it == m_sceneCreators.end())
        return false;

    cocos2d::Scene* scene = it->second();
    if (!scene)
    {
        CCLOGERROR("The creator of scene ('%s') not found, scene creation failed.", msg.m_info.c_str());
        return false;
    }

    for (auto c : scene->getChildren())
    {
        BtBaseScene* scene = dynamic_cast<BtBaseScene*>(c);
        if (scene)
        {
            scene->preEnter(msg.m_extraParams[0]);
        }
    }

    if (director->getRunningScene())
    {
        director->replaceScene(scene);
    }
    else
    {
        director->runWithScene(scene);
    }
    return true;
}

void AppDelegate::CallLua_Tick(float deltaSeconds)
{
    BT_CALL_LUA("hostcall_tick", deltaSeconds);
}

bool AppDelegate::CallLua_Init()
{
    bool succ = false;
    btlua_ref val = BT_CALL_LUA("hostcall_init");
    if (val.type() == LUA_TBOOLEAN)
        succ = val.cast<bool>();
    if (!succ)
    {
        CCLOG("hostcall_init() failed.");
        return false;
    }

    return true;
}

void AppDelegate::CallLua_Destroy()
{
    BT_CALL_LUA("hostcall_destroy");
}

void AppDelegate::TickMsgDispatcher(float deltaSeconds)
{
    BtMsgDispatcher::Get()->tick(GetCurTime(), deltaSeconds);
}
