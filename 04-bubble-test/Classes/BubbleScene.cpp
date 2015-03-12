#include <vector>
#include <array>
#include <list>
#include "BubbleScene.h"

using std::array;
using std::vector;
using std::list;
using cocos2d::Vec2;
using cocos2d::Touch;
using cocos2d::Event;
using cocos2d::Director;
using cocos2d::Sprite;

const int kScreenWidth = 720;
const int kScreenHeight = 1280;
const int kBubbleWidth = 64;
const int kBubbleHeight = 64;
const double kBubbleRealHeight = 64 * 0.866;
const double kBubbleScale = 2.0;

const int kGridWidth = kScreenWidth / kBubbleWidth;
const int kGridHeight = kScreenHeight / kBubbleRealHeight;
const int kBubbleCountPerLine = kScreenWidth / kBubbleWidth;

enum {
    COLOR_RED = 0,
    COLOR_BLUE,
    COLOR_GREEN,
    COLOR_ORANGE,
    COLOR_PURPLE,
    COLOR_WHITE,
    COLOR_YELLOW,
    COLOR_NR,
};

enum {
    KEYCODE_PAUSE = 59,
    KEYCODE_SPEEDUP = 77,
    KEYCODE_SPEEDDOWN = 78,
};
    
bool BubbleCollisionDetect(Vec2 loc1, Vec2 loc2) {
  auto dist = sqrt((loc1.x - loc2.x) * (loc1.x - loc2.x) + (loc1.y - loc2.y) * (loc1.y - loc2.y));
  if (dist <= kBubbleWidth)
    return true;
  return false;
}

class Grid {
 public:
  Grid(int x_, int y_) {
    x = x_;
    y = y_;
  }
  Grid() {}
  int x;
  int y;
};

class Bubble {
  friend class BubbleLayer;
 public:
  static Bubble * create(int color);
  void set_grid(Grid grid) {
    grid_ = grid;
  }
  void set_color(int color) {
    color_ = color;
  }
  void set_type(int type) {
    type_ = type;
  }
  cocos2d::Sprite * sprite() {
    return sprite_;
  }
 protected:
    Grid grid_;
    int color_;
    int type_;
    cocos2d::Sprite *sprite_;
    bool remove_ = false;
    bool mark_ = false;
};

Bubble * Bubble::create(int color) {
    char path[64];
    snprintf(path, 64, "bubble_%d.png", color);
    auto b = new Bubble();
    b->sprite_ = Sprite::create(path);
    b->sprite_->setScale(kBubbleScale);
    b->sprite_->retain();
    b->color_ = color;
    return b;
}

typedef array<Bubble *, kBubbleCountPerLine> BubbleLine;

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

  Vec2 Grid2Pos(Grid);
  Grid Pos2Grid(Vec2);

  void FillGrid(Grid dest, Bubble *bubble, bool replace = false);
  Bubble * GetFilled(Grid dest);
  void FindOverlapGrid(Vec2 pos, vector<Grid> *vec_grid);
    void GetAroundBubble(Bubble *center, vector<Bubble *> *vec_bubble);

  static GridLayer * Shared() {
    if (gridlayer == NULL) {
      gridlayer = create();
    }
    return gridlayer;
  }

 private:
  void FreeGridHeadLine();
  void GetAroundGrid(Grid v, vector<Grid> *vec_grid);

  vector<BubbleLine> lines_;
  float collision_height_ = 2000;
  double last_move_delta_ = 0.0;
  double move_delta_ = 0.0;
  static GridLayer *gridlayer;
};

GridLayer * GridLayer::gridlayer;

bool GridLayer::init() {
  return Layer::init();
}

void GridLayer::onEnter() {
  Layer::onEnter();
  scheduleUpdate();

  auto bubble = Bubble::create(COLOR_RED);
  // addChild(sprite);
  auto size = bubble->sprite()->getContentSize();
  auto s = Director::getInstance()->getWinSize();
  GridInit(s.width, s.height,
           size.width * kBubbleScale, size.height * kBubbleScale);
  // sprite->release();
}

void GridLayer::FreeGridHeadLine() {
  cocos2d::log("%s\n", __FUNCTION__);
  for (int i = 0; i < 2; i++) {
    auto line = lines_[0];
    for (auto& bubble : line) {
      if (bubble != nullptr) {
        auto s = bubble->sprite();
        removeChild(s);
        s->release();
        delete bubble;
        cocos2d::log("free bubble\n");
      }
    }
    lines_.erase(lines_.begin());

    // add new line
    BubbleLine new_line;
    new_line.fill(nullptr);
    lines_.push_back(new_line);
  }
}

void GridLayer::update(float dt) {
#if 0
  auto loc = getPosition();
  loc.y -= 10 * dt;
  last_move_delta_ += 10 * dt;
  if (last_move_delta_ >= kBubbleRealHeight * 2) {
    last_move_delta_ -= kBubbleRealHeight * 2;
    move_delta_ += kBubbleRealHeight * 2;
    FreeGridHeadLine();
  }
  setPosition(loc);
#endif
}

Bubble * GridLayer::GetFilled(Grid dest) {
    return lines_[dest.y][dest.x];
}

void GridLayer::FillGrid(Grid dest, Bubble *bubble, bool replace) {
    // cocos2d::log("fill grid xy %d, %d", dest.x, dest.y);
  if (dest.x >= kGridWidth || dest.y >= kGridHeight) {
    // assert(0);
    // return;
  }

  auto old = lines_[dest.y][dest.x];
  if (old != nullptr && !replace) {
    cocos2d::log("old %p\n", old);
    return;
  }

  if (bubble) {
      auto pos = Grid2Pos(dest);
      // cocos2d::log("fill pos xy %f, %f", pos.x, pos.y);
      addChild(bubble->sprite());
      bubble->sprite()->setPosition(pos);
      lines_[dest.y][dest.x] = bubble;
  } else {
      if (old) {
          removeChild(old->sprite());
      }
      lines_[dest.y][dest.x] = nullptr;
  }
}

Grid GridLayer::Pos2Grid(Vec2 pos) {
  Grid v;
  float offset = kBubbleWidth / 2;
  v.y = int((pos.y + last_move_delta_) / kBubbleRealHeight);
  if (v.y & 0x01)
    v.x = int((pos.x - offset) / kBubbleWidth);
  else
    v.x = int(pos.x / kBubbleWidth);
  // cocos2d::log("%s: %f, %f, %d, %d\n", __FUNCTION__, pos.x, pos.y, v.x, v.y);
  return v;
}

#define IS_VALID_GRID(g) \
  (((g).x >= 0 && (g).x < kGridWidth) && ((g).y >= 0 && (g).y < (kGridHeight + 2)))

//   O O O
//  O O O
//   O O O
//  O O O
void GridLayer::GetAroundGrid(Grid v, vector<Grid> *vec_grid) {
  static int offset0[][2] = {
    { -1,  0, },
    { -1,  1, },
    {  0,  1, },
    {  1,  0, },
    {  0, -1, },
    { -1, -1, },
  };

  static int offset1[][2] = {
    { -1,  0, },
    {  0,  1, },
    {  1,  1, },
    {  1,  0, },
    { -1, -1, },
    {  0, -1, },
  };

  int (*offset)[2] = v.y & 0x01 ? &offset1[0] : &offset0[0];

  for (int i = 0; i < 6; i++) {
    auto g = v;
    g.x += offset[i][0];
    g.y += offset[i][1];
    if (IS_VALID_GRID(g)) {
      vec_grid->push_back(g);
    }
  }
}

void GridLayer::GetAroundBubble(Bubble *center, vector<Bubble *> *vec_bubble) {
    auto g = Pos2Grid(center->sprite()->getPosition());
    vector<Grid> around;
    GetAroundGrid(g, &around);
    for (auto& v : around) {
        auto b = GetFilled(v);
        if (b) {
            vec_bubble->push_back(b);
        }
    }
}

void GridLayer::FindOverlapGrid(Vec2 pos, vector<Grid> *vec_grid) {
  auto g = Pos2Grid(pos);
  vector<Grid> around;
  GetAroundGrid(g, &around);
  // cocos2d::log("get around of: %d %d %d\n", g.x, g.y, around.size());
  for (auto& v : around) {
      if (BubbleCollisionDetect(pos, Grid2Pos(v))) {
        vec_grid->push_back(v);
        // cocos2d::log("around: %d %d\n", v.x, v.y);
      }
  }
}

Vec2 GridLayer::Grid2Pos(Grid grid) {
  float offset = kBubbleWidth / 2;
  if (grid.y & 0x01) {
    offset += kBubbleWidth / 2;
  }

  auto v = Vec2(grid.x * kBubbleWidth + offset,
                grid.y * kBubbleRealHeight + kBubbleRealHeight / 2 + move_delta_);
  return v;
}

void GridLayer::GridInit(float screen_width, float screen_height,
                         float bubble_width, float bubble_height) {
  cocos2d::log("%f %f %f %f\n", screen_width, screen_height, bubble_width, bubble_height);
  assert(screen_width == kScreenWidth);
  assert(screen_height == kScreenHeight);
  assert(bubble_width == kBubbleWidth);
  assert(bubble_height == kBubbleHeight);

  cocos2d::log("grid width, height %d %d\n", kGridWidth, kGridHeight);

  // init grid
  for (int i = 0; i < kGridHeight + 2; i++) {
    BubbleLine line;
    line.fill(nullptr);
    lines_.push_back(line);
  }

  collision_height_ = screen_height;
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
  bool CheckOutScreen();
  void StopBubble();
  // for touche
  // void registerWithTouchDispatcher(void);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode code, Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, Event* event);
  bool onTouchBegan(Touch *touch, Event* event);
  void onTouchMoved(Touch *touch, Event* event);
  void onTouchEnded(Touch *touch, Event* event);
  void onTouchCancelled(Touch *touch, Event* event);

private:
    Bubble *bubble_ = NULL;
    Bubble *next_ = NULL;
    float sin_a_ = 0.0;
    float cos_a_ = 0.0;
    bool pause_ = false;
    void SetNextBubble();
    void addNewSpriteWithCoords(Vec2 p);
    bool CheckCollision();
    bool DoCheckMatch(Bubble *start, Bubble *center, size_t match,
                      list<Bubble *> *remove_list);
    bool CheckMatch(Bubble *start);
};

void BubbleLayer::addNewSpriteWithCoords(Vec2 dest) {
  auto bubble = Bubble::create(COLOR_RED);
  if (bubble == NULL) {
    printf("create bubble faile\n");
    return;
  }
  addChild(bubble->sprite());
  auto s = Director::getInstance()->getWinSize();
  bubble->sprite()->setPosition(Vec2(s.width/2, 0));

  auto action = cocos2d::MoveTo::create(2, dest);
  bubble->sprite()->runAction(action);
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
    
    bubble->runAction( RepeatForever::create(seq) );
#endif
}

bool BubbleLayer::init() {
  if (!Layer::init())
    return false;

  // setTouchEnabled(true);
  return true;
}

void BubbleLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode code, Event* event)
{
}

void BubbleLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode code, Event* event)
{
    cocos2d::log("onKeyReleased, keycode: %d", code);
    switch (code) {
    case KEYCODE_PAUSE:
        pause_ = !pause_;
        break;
#if 0
    case 124: {
        auto gl = GridLayer::Shared();
        vector<Grid> grids;
        gl->FindOverlapGrid(bubble_->sprite()->getPosition(), &grids);
        for (auto& g : grids) {
            cocos2d::log("around %d,%d\n", g.x, g.y);
        }
    }
        break;
#endif
    case KEYCODE_SPEEDUP:
        break;
    case KEYCODE_SPEEDDOWN:
        break;
    }
}

bool BubbleLayer::onTouchBegan(Touch *touch, Event* event) {
  return true;
}

void BubbleLayer::onTouchMoved(Touch *touch, Event* event) {
  cocos2d::log("%s\n", __FUNCTION__);
}

static int RandomColor() {
    struct timeval now;
    cocos2d::gettimeofday(&now, nullptr);
    return now.tv_usec % COLOR_NR;
}

void BubbleLayer::SetNextBubble() {
    auto s = Director::getInstance()->getWinSize();
    auto origin = Vec2(s.width/2, 0);
    next_ = Bubble::create(RandomColor());
    if (next_ == NULL) {
        cocos2d::log("create bubble faile\n");
        return;
    }
    addChild(next_->sprite());
    next_->sprite()->setPosition(origin);
    next_->sprite()->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

void BubbleLayer::onTouchEnded(Touch *touch, Event* event) {
  if (bubble_)
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

  bubble_ = next_;
  SetNextBubble();
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

  auto keyboardListener = cocos2d::EventListenerKeyboard::create();
  keyboardListener->onKeyPressed = CC_CALLBACK_2(BubbleLayer::onKeyPressed, this);
  keyboardListener->onKeyReleased = CC_CALLBACK_2(BubbleLayer::onKeyReleased, this);
  _eventDispatcher->addEventListenerWithFixedPriority(keyboardListener, -11);
  // _keyboardListener = keyboardListener;

  SetNextBubble();
}

void BubbleLayer::TimeCallback(float dt) {
}

bool BubbleLayer::CheckOutScreen() {
  if (!bubble_)
    return false;

  auto loc = bubble_->sprite()->getPosition();
  auto size = bubble_->sprite()->getContentSize();
  auto screen = Director::getInstance()->getWinSize();

  if ((loc.y + size.height / 2) > screen.height) {
    return true;
  }
  return false;
}

void BubbleLayer::StopBubble() {
  if (bubble_ == NULL)
    return;
  removeChild(bubble_->sprite());
  // bubble_->sprite()->setParent(nullptr);
  // delete bubble_;
  bubble_ = NULL;
}

void BubbleLayer::CheckEdge() {
  if (bubble_ == NULL)
    return;
  auto s = Director::getInstance()->getWinSize();
  auto curr = bubble_->sprite()->getPosition();
  auto size = bubble_->sprite()->getContentSize();

  // check if out of screen
  auto y = curr.y - (size.height / 2);
  if (y > s.height) {
    // stop move and release current bubble
    StopBubble();
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

const int kBubbleMoveSpeed = 600;
void BubbleLayer::Move(float dt) {
  if (bubble_ == NULL)
    return;

  auto distance = kBubbleMoveSpeed * dt;
  Vec2 dest;
  Vec2 curr = bubble_->sprite()->getPosition();
  dest.x = curr.x + distance * cos_a_;
  dest.y = curr.y + distance * sin_a_;
  bubble_->sprite()->setPosition(dest);
}

bool BubbleLayer::CheckCollision() {
  if (!bubble_)
    return false;

  auto gl = GridLayer::Shared();
  vector<Grid> grids;
  gl->FindOverlapGrid(bubble_->sprite()->getPosition(), &grids);
  for (auto& g : grids) {
    if (gl->GetFilled(g)) {
      cocos2d::log("collision %d,%d\n", g.x, g.y);
      // for (auto& g2 : grids) {
      //     cocos2d::log("around %d,%d\n", g2.x, g2.y);
      // }
      return true;
    }
  }
  return false;
}

bool BubbleLayer::DoCheckMatch(Bubble *start, Bubble *center, size_t match,
                               list<Bubble *> *remove_list)
{
    vector<Bubble *> around;
    bool res = match >= 3;
    if (res) {
        center->remove_ = true;
        remove_list->push_back(center);
    }

    GridLayer::Shared()->GetAroundBubble(center, &around);
    if (around.size() == 0)
        return res;

    for (size_t i = 0; i < around.size(); i++) {
        if (around[i] == start)
            continue;
        if (center->color_ != around[i]->color_)
            continue;
        if (around[i]->mark_)
            continue;

        around[i]->mark_ = true;
        auto ismatched = DoCheckMatch(center, around[i], match + 1, remove_list);
        if (ismatched) {
            res = true;
            if (!center->remove_) {
                center->remove_ = true;
                remove_list->push_back(center);
            }
        }
        around[i]->mark_ = false;
    }

    return res;
}

bool BubbleLayer::CheckMatch(Bubble *start) {
  list<Bubble *> remove_list;
  start->mark_ = true;
  bool match = DoCheckMatch(NULL, start, 1, &remove_list);
  start->mark_ = false;
  if (match) {
      auto gl = GridLayer::Shared();
      while (remove_list.begin() != remove_list.end()) {
          auto b = remove_list.front();
          remove_list.pop_front();
          // cocos2d::log("remove %p\n", b);
          gl->FillGrid(gl->Pos2Grid(b->sprite()->getPosition()),
                       nullptr, true);
          b->sprite()->release();
          delete b;
      }
  } else {
      cocos2d::log("not match");
  }
  return match;
}

void BubbleLayer::update(float dt)
{
    if (pause_)
        return;

    Move(dt);
    CheckEdge();

    auto gl = GridLayer::Shared();
    if (CheckOutScreen()) {
        cocos2d::log("fill out screen");
        assert(gl);
        auto dest = gl->Pos2Grid(bubble_->sprite()->getPosition());
        auto s = bubble_;
        StopBubble();
        gl->FillGrid(dest, s, false);
        return;
    }
    if (CheckCollision()) {
        cocos2d::log("fill collision");
        auto dest = gl->Pos2Grid(bubble_->sprite()->getPosition());
        auto s = bubble_;
        StopBubble();
        gl->FillGrid(dest, s, false);
        CheckMatch(s);
        return;
    }
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

  GridLayer *gridlayer = GridLayer::Shared();
  addChild(gridlayer);
}
