#ifndef __APP_START_SCENE_H__
#define __APP_START_SCENE_H__

#include "cocos2d.h"

class AppStartScene : public cocos2d::Layer
{
public:
    virtual ~AppStartScene();
    CREATE_FUNC(AppStartScene);

    virtual bool init();  

    void onMenuItem(Ref* sender);
};

#endif // __HELLOWORLD_SCENE_H__
