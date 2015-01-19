#include "BubbleScene.h"

using cocos2d::Vec2;
using cocos2d::Touch;
using cocos2d::Event;
using cocos2d::Director;
using cocos2d::Sprite;

class GridLayer : public cocos2d::Layer {
 public:
  CREATE_FUNC(GridLayer);

  bool init();
  void onEnter();
  void update(float dt);

  void GridInit(float screen_width, float screen_height,
                float bubble_width, float bubble_height);

  //Vec2 GetNearestGrid(Vec2 loc);
  //void SetGrid(Vec2 dest);
  //void ClearGrid(Vec2 dest);

  Vec2 Grid2Pos(int m, int n);
  Vec2 Pos2Grid();

 private:
  int grid_x_ = 0;
  int grid_y_ = 0;

  float bubble_width_;
  float bubble_height_;
  float pos_height_;
};

bool GridLayer::init() {
  return Layer::init();
}

void GridLayer::onEnter() {
  Layer::onEnter();
  scheduleUpdate();

  auto sprite = Sprite::create("bubble.png");
  // addChild(sprite);
  auto size = sprite->getContentSize();
  auto s = Director::getInstance()->getWinSize();
  GridInit(s.width, s.height, size.width, size.height);
  // sprite->release();
}

void GridLayer::update(float dt) {
}

Vec2 GridLayer::Grid2Pos(int m, int n) {
  float offset = bubble_width_ / 2;
  if (n & 0x01) {
    offset += bubble_width_ / 2;
  }

  auto v = Vec2(m * bubble_width_ + offset,
                n * pos_height_ + pos_height_ / 2);
  return v;
}

void GridLayer::GridInit(float screen_width, float screen_height,
                         float bubble_width, float bubble_height) {
  bubble_width_ = bubble_width;
  bubble_height_ = bubble_height;
  pos_height_ = bubble_height * 0.866;

  grid_x_ = screen_width / bubble_width;
  grid_y_ = screen_height / pos_height_;

  cocos2d::log("grid x, y %d %d\n", grid_x_, grid_y_);

  // test
  for (int m = 0; m < grid_x_; m++) {
    for (int n = 0; n < grid_y_; n++) {
      auto v = Grid2Pos(m, n);
      cocos2d::log("grid ps %f, %f", v.x, v.y);
      auto sprite = Sprite::create("bubble.png");
      sprite->setPosition(v);
      addChild(sprite);
    }
  }
}

class BubbleLayer : public cocos2d::Layer {
 public:
  CREATE_FUNC(BubbleLayer);

  bool init();
  void onEnter();
  void update(float dt);

  void TimeCallback(float dt);
  void InitSelf();
  void Move(float dt);
  void CheckEdge();

  // for touche
  // void registerWithTouchDispatcher(void);
  bool onTouchBegan(Touch *touch, Event* event);
  void onTouchMoved(Touch *touch, Event* event);
  void onTouchEnded(Touch *touch, Event* event);
  void onTouchCancelled(Touch *touch, Event* event);

 private:
  Sprite *sprite_ = NULL;
  float sin_a_ = 0.0;
  float cos_a_ = 0.0;
  void addNewSpriteWithCoords(Vec2 p);
};

void BubbleLayer::addNewSpriteWithCoords(Vec2 dest) {
  auto sprite = Sprite::create("bubble.png");
  if (sprite == NULL) {
    printf("create sprite faile\n");
    return;
  }
  addChild(sprite);
  auto s = Director::getInstance()->getWinSize();
  sprite->setPosition(Vec2(s.width/2, 0));

  auto action = cocos2d::MoveTo::create(2, dest);
  sprite->runAction(action);
#if 0    
    ActionInterval* action;
    float random = CCRANDOM_0_1();
    
    if( random < 0.20 )
        action = ScaleBy::create(3, 2);
    else if(random < 0.40)
        action = RotateBy::create(3, 360);
    else if( random < 0.60)
        action = Blink::create(1, 3);
    else if( random < 0.8 )
        action = TintBy::create(2, 0, -255, -255);
    else 
        action = FadeOut::create(2);
    auto action_back = action->reverse();
    auto seq = Sequence::create( action, action_back, nullptr );
    
    sprite->runAction( RepeatForever::create(seq) );
#endif
}

bool BubbleLayer::init() {
  if (!Layer::init())
    return false;

  // setTouchEnabled(true);
  return true;
}

bool BubbleLayer::onTouchBegan(Touch *touch, Event* event) {
  return true;
}

void BubbleLayer::onTouchMoved(Touch *touch, Event* event) {
  cocos2d::log("%s\n", __func__);
}

void BubbleLayer::onTouchEnded(Touch *touch, Event* event) {
  if (sprite_)
    return;

  // addNewSpriteWithCoords(touch->getLocation());
  auto s = Director::getInstance()->getWinSize();
  auto origin = Vec2(s.width/2, 0);
  auto dest = touch->getLocation();
  auto direction = 0;

  if (dest.x <= origin.x) {
    dest.x = origin.x - dest.x;
    direction = 1;
  } else {
    dest.x = dest.x - origin.x;
  }

  sin_a_ = dest.y / sqrt(dest.x*dest.x + dest.y*dest.y);
  cos_a_ = dest.x / sqrt(dest.x*dest.x + dest.y*dest.y);
  if (direction)
    cos_a_ = -cos_a_;

  sprite_ = Sprite::create("bubble.png");
  if (sprite_ == NULL) {
    cocos2d::log("create sprite faile\n");
    return;
  }
  addChild(sprite_);
  sprite_->setPosition(origin);
  sprite_->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

void BubbleLayer::onTouchCancelled(Touch *touch, Event* event) {
  cocos2d::log("%s\n", __FUNCTION__);
}

void BubbleLayer::onEnter() {
  Layer::onEnter();

  scheduleUpdate();
  schedule(schedule_selector(BubbleLayer::TimeCallback), 0.05, -1, 0);

  // Register Touch Event
  auto listener = cocos2d::EventListenerTouchOneByOne::create();
  listener->setSwallowTouches(true);
    
  listener->onTouchBegan = CC_CALLBACK_2(BubbleLayer::onTouchBegan, this);
  listener->onTouchMoved = CC_CALLBACK_2(BubbleLayer::onTouchMoved, this);
  listener->onTouchEnded = CC_CALLBACK_2(BubbleLayer::onTouchEnded, this);    
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void BubbleLayer::TimeCallback(float dt) {
}

void BubbleLayer::CheckEdge() {
  if (sprite_ == NULL)
    return;
  auto s = Director::getInstance()->getWinSize();
  auto curr = sprite_->getPosition();
  auto size = sprite_->getContentSize();

  // check if out of screen
  auto y = curr.y - (size.height / 2);
  if (y > s.height) {
    // stop move and release current bubble
    removeChild(sprite_);
    sprite_ = NULL;
    return;
  }

  auto x = curr.x - (size.width / 2);
  if (x < 0) {
    // cocos2d::log("left edge!");
    cos_a_ = -cos_a_;
    return;
  }

  x = curr.x + (size.width / 2);
  if (x > s.width) {
    // cocos2d::log("right edge!");
    cos_a_ = -cos_a_;
    return;
  }
}

const int kBubbleMoveSpeed = 200;
void BubbleLayer::Move(float dt) {
  if (sprite_ == NULL)
    return;

  auto distance = kBubbleMoveSpeed * dt;
  Vec2 dest;
  Vec2 curr = sprite_->getPosition();
  dest.x = curr.x + distance * cos_a_;
  dest.y = curr.y + distance * sin_a_;
  sprite_->setPosition(dest);
}

void BubbleLayer::update(float dt) {
  Move(dt);
  CheckEdge();
}

bool BubbleScene::init() {
  if (!Scene::init())
    return false;

  return true;
}

void BubbleScene::onEnter() {
  Scene::onEnter();

  BubbleLayer *layer = BubbleLayer::create();
  assert(layer != NULL);
  addChild(layer);

  GridLayer *gridlayer = GridLayer::create();
  addChild(gridlayer);
}
