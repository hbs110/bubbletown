#ifndef __MP_BUBBLE_SCENE_H__
#define __MP_BUBBLE_SCENE_H__

#include <cocos2d.h>
//#include <cocos-ext.h>

class BubbleScene : public cocos2d::Scene {
 public:
  CREATE_FUNC(BubbleScene);
  bool init();
  void onEnter();
};

#endif
