/*!
 * \file BtTownSceneWidgets.h
 * \date 2015/01/28 19:34
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtTownSceneWidgets 
*/

#pragma once

const int BT_ArrowCount = 4;

class BtTiledMap;

class BtTownSceneWidgets 
{
public: 
    BtTownSceneWidgets();
    ~BtTownSceneWidgets() {}

    bool init(cocos2d::Node* parent);

    cocos2d::Sprite* getArrow(int i) { return m_arrows[i]; }
    void showArrowsAt(BtTiledMap* tileMap, const cocos2d::Vec2& coord, float scale, float dist);
    void hideArrows();

private:
    cocos2d::Sprite* m_arrows[BT_ArrowCount];
};

