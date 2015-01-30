/*!
 * \file BtTownSceneWidgets.cpp
 * \date 2015/01/28 19:34
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtTownSceneWidgets
*/

#include "stdafx.h"
#include "BtTownSceneWidgets.h"

#include "Core/BtCoreDef.h"

#include "BtTiledMap.h"

BtConstStr BtArrowImages[] = {
    "elements/fx/arrow_downleft.png",
    "elements/fx/arrow_downright.png",
    "elements/fx/arrow_upleft.png",
    "elements/fx/arrow_upright.png"
};

BtTownSceneWidgets::BtTownSceneWidgets()
{
    memset(m_arrows, 0, sizeof(cocos2d::Sprite*) * BT_ArrowCount);
}

bool BtTownSceneWidgets::init(cocos2d::Node* parent)
{
    if (!parent)
        return false;

    for (int i = 0; i < BT_ArrowCount; ++i)
    {
        m_arrows[i] = cocos2d::Sprite::create(BtArrowImages[i]);
        parent->addChild(m_arrows[i], 2);
    }

    hideArrows();
    return true;
}

void BtTownSceneWidgets::hideArrows()
{
    for (int i = 0; i < BT_ArrowCount; ++i)
        m_arrows[i]->setVisible(false);
}

void BtTownSceneWidgets::showArrowsAt(BtTiledMap* tileMap, const cocos2d::Vec2& coord, float scale, float dist)
{
    if (!tileMap)
        return;

    cocos2d::Vec2 tilePos;
    if (tileMap->getTilePosition(cocos2d::Vec2(coord.x, coord.y + dist), &tilePos))
        m_arrows[0]->setPosition(tilePos);
    if (tileMap->getTilePosition(cocos2d::Vec2(coord.x + dist, coord.y), &tilePos))
        m_arrows[1]->setPosition(tilePos);
    if (tileMap->getTilePosition(cocos2d::Vec2(coord.x - dist, coord.y), &tilePos))
        m_arrows[2]->setPosition(tilePos);
    if (tileMap->getTilePosition(cocos2d::Vec2(coord.x, coord.y - dist), &tilePos))
        m_arrows[3]->setPosition(tilePos);

    for (auto arrow : m_arrows)
    {
        cocos2d::Size size = tileMap->GetTileSize();
        arrow->setScale(size.width / arrow->getContentSize().width * scale,
                        size.height / arrow->getContentSize().height * scale);
        arrow->setVisible(true);
    }
}

void BtTownSceneWidgets::markArrowsAvail(bool avail)
{
    for (auto arrow : m_arrows)
        arrow->setOpacity(avail ? 255 : 128);
}
