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

    void showArrowsAt(BtTiledMap* tileMap, const cocos2d::Rect& tileRect, float scale);
    void hideArrows();
    void markArrowsAvail(bool avail);

private:
    cocos2d::Sprite* m_arrows[BT_ArrowCount];
};

