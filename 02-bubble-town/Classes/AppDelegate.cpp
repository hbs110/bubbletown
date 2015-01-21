#include "stdafx.h"
#include "AppDelegate.h"
#include "AppMacros.h"
#include "AppStartScene.h"

#include "Scenes/BtSceneDef.h"
#include "Scenes/Scene_Bubble/BtBubbleScene.h"
#include "Scenes/Scene_Town/BtTownScene.h"
#include "Scenes/Scene_World/BtWorldScene.h"

#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
    BtMsgDispatcher::DestroyInst();
}

bool AppDelegate::applicationDidFinishLaunching() 
{
    // message handling
    BtMsgDispatcher::CreateInst();
    BtMsgDispatcher::Get()->Subscribe(BTMSG_GotoScene, std::bind(&AppDelegate::OnMsg_GotoScene, this, std::placeholders::_1));

    // initialize director
    auto director = cocos2d::Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = cocos2d::GLViewImpl::create("Cpp Empty Test");
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

    
    // ------------- Scene ------------- 

    m_sceneCreators[BTSCN_Start] = &BtCreateScene<AppStartScene>;
    m_sceneCreators[BTSCN_Bubble] = &BtCreateScene<BtBubbleScene>;
    m_sceneCreators[BTSCN_Town] = &BtCreateScene<BtTownScene>;
    m_sceneCreators[BTSCN_World] = &BtCreateScene<BtWorldScene>;

    auto scene = BtCreateScene<AppStartScene>();
    director->runWithScene(scene);
                            
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

    auto it = m_sceneCreators.find(msg.m_info);
    if (it == m_sceneCreators.end())
        return false;

    cocos2d::Scene* scene = it->second();
    if (!scene)
    {
        CCLOGERROR("The creator of scene ('%s') not found, scene creation failed.", msg.m_info.c_str());
        return false;
    }

    director->replaceScene(scene);
    return true;
}
