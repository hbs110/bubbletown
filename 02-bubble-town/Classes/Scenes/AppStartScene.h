#ifndef __APP_START_SCENE_H__
#define __APP_START_SCENE_H__

#include "cocos2d.h"

#include "Core/BtGuiUtil.h"

class AppStartScene : public cocos2d::Layer, public BtGuiListener
{
public:
    virtual ~AppStartScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuCloseCallback(Ref* sender);

    virtual void OnMenuItem(Ref* sender);

    
    // implement the "static node()" method manually
    CREATE_FUNC(AppStartScene);
};

#endif // __HELLOWORLD_SCENE_H__
