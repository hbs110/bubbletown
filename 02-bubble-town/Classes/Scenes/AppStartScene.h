#ifndef __APP_START_SCENE_H__
#define __APP_START_SCENE_H__

#include "cocos2d.h"

class AppStartScene : public cocos2d::Layer
{
public:
    virtual ~AppStartScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // a selector callback
    void menuCloseCallback(Ref* sender);

    void OnMenuItem(Ref* sender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(AppStartScene);
};

#endif // __HELLOWORLD_SCENE_H__
