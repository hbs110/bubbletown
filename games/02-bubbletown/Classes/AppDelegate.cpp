#include "stdafx.h"
#include "AppDelegate.h"
#include "AppMacros.h"

#include "Scenes/Scene_Start/BtStartScene.h"
#include "Scenes/Scene_Bubble/BtBubbleScene.h"
#include "Scenes/Scene_Town/BtTownScene.h"
#include "Scenes/Scene_World/BtWorldScene.h"
#include "Scenes/BtSceneFactory.h"

#include "Core/BtCoreUtil.h"
#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"

#include "Services/BtLuaService.h"
#include "Services/BtTestServices.h"

#include "Tests/TestGui_utouch.h"

#include "AppNativeInterfaces.h"
#include "AppMsgHandlers.h"

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
    BtSceneFactory::DestroyInst();

    if (BT_L)
    {
        CallLua_Destroy();
        BtLuaService::DestroyInst();
    }
    
    BtMsgDispatcher::DestroyInst();
    BtTestServices::DestroyInst();
}

bool AppDelegate::applicationDidFinishLaunching() 
{
    BT_LOG("app started.");
    const auto& paths = cocos2d::FileUtils::getInstance()->getSearchPaths();
    for (const auto& p : paths)
        BT_LOG("    search path: %s", p);
    BT_LOG("    writable path: %s", cocos2d::FileUtils::getInstance()->getWritablePath());

    BtTestServices::CreateInst();

    BtRegisterGuiTests_utouch();

    if (!BtTestServices::Get()->RunTestsOnStage(BtTestStage::PreInit))
        return false;

    // message handling
    BtMsgDispatcher::CreateInst();

    // initialize director
    auto director = cocos2d::Director::getInstance();

    WCHAR utf16Path[MAX_PATH] = { 0 };
    GetCurrentDirectoryW(sizeof(utf16Path) - 1, utf16Path);

    char utf8Path[MAX_PATH] = { 0 };
    int nNum = WideCharToMultiByte(CP_UTF8, 0, utf16Path, -1, utf8Path, sizeof(utf8Path), nullptr, nullptr);

    std::string s_resourcePath = utf8Path;
    s_resourcePath.append("/");

    cocos2d::FileUtils::getInstance()->addSearchPath(s_resourcePath, true);

    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = cocos2d::GLViewImpl::createWithRect("bubbletown", cocos2d::Rect(0, 0, 720, 1280), 0.6f);
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

    GAppStartTime = cocos2d::utils::gettime();
    
    BtLuaService::CreateInst();
    if (!BtLuaService::Get()->Init())
        return false;

    AppNativeInterfaces::registerGlobally();

    AppMsgHandlers::registerGlobally();

    BtSceneFactory::CreateInst();
    BtSceneFactory::Get()->RegisterSceneCreator<BtStartScene>();
    BtSceneFactory::Get()->RegisterSceneCreator<BtBubbleScene>();
    BtSceneFactory::Get()->RegisterSceneCreator<BtTownScene>();
    BtSceneFactory::Get()->RegisterSceneCreator<BtWorldScene>();

    auto luaTick = std::bind(&AppDelegate::CallLua_Tick, this, std::placeholders::_1);
    cocos2d::Director::getInstance()->getScheduler()->schedule(luaTick, this, 1.0f, false, "AppDelegate::TickLua_PerSecond");

    auto frameTick = std::bind(&AppDelegate::TickPerFrame, this, std::placeholders::_1);
    cocos2d::Director::getInstance()->getScheduler()->schedule(frameTick, this, 0.0f, false, "AppDelegate::TickPerFrame");

    if (!CallLua_Init())
        return false;

    if (!BtTestServices::Get()->RunTestsOnStage(BtTestStage::PostInit))
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

void AppDelegate::TickPerFrame(float deltaSeconds)
{
    GAppCurrentTime = cocos2d::utils::gettime() - GAppStartTime;
    BtMsgDispatcher::Get()->tick(GAppCurrentTime, deltaSeconds);
}
