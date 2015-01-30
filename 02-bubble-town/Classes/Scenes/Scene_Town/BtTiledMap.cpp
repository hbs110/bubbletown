/*!
 * \file BtTiledMap.cpp
 * \date 2015/01/22 10:10
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtTiledMap
*/

#include "stdafx.h"
#include "BtTiledMap.h"

#include "Core/BtCoreDef.h"

const int BT_TilemapFileMaxZ = 10;
const int BT_TilemapLogicalZ = BT_TilemapFileMaxZ + 10;

const float BtFadingDuration = 0.8f;
const float BtFadingTargetOpacity = 64;


bool BtTiledMap::Load(const std::string& filename)
{
    auto map = tileMap_t::create(filename);
    if (!map || map->getChildrenCount() == 0)
        return false;

    auto comp = [](cocos2d::Node* c1, cocos2d::Node* c2) { return !c1 || !c2 || c1->getLocalZOrder() < c2->getLocalZOrder(); };
    auto maxElem = std::max_element(map->getChildren().begin(), map->getChildren().end(), comp);
    if (maxElem == map->getChildren().end() || !(*maxElem) || (*maxElem)->getLocalZOrder() >= BT_TilemapFileMaxZ)
        return false;

    m_tileMap = map;
    m_tileMap->retain();
    auto layerBackground = GetTileMapLayer(BtLayer_Background);
    if (!layerBackground)
        return false;
    auto layerSockets = GetTileMapLayer(BtLayer_Sockets);
    if (!layerSockets)
        return false;

    // flush all tiles into sprites
    cocos2d::Size layerSize = layerSockets->getLayerSize();
    for (int y = 0; y < layerSize.height; ++y)
        for (int x = 0; x < layerSize.width; ++x)
            auto sprite = layerSockets->getTileAt(cocos2d::Vec2(x, y));

    auto s = m_tileMap->getContentSize();
    CCLOG("ContentSize: %f, %f", s.width,s.height);
    m_tileMap->setPosition(cocos2d::Vec2(0, 0));

    m_spriteRoot = cocos2d::Node::create();
    m_spriteRoot->retain();

    m_tileMap->addChild(m_spriteRoot, BT_TilemapLogicalZ);
    return true;
}

void BtTiledMap::Release()
{
    if (m_spriteRoot)
    {
        m_spriteRoot->release();
        m_spriteRoot = nullptr;
    }

    if (m_tileMap)
    {
        m_tileMap->release();
        m_tileMap = nullptr;
    }
}

tileLayer_t* BtTiledMap::GetTileMapLayer(const std::string& layerName)
{
    if (!m_tileMap)
        return nullptr;

    return m_tileMap->getLayer(layerName);
}

void BtTiledMap::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
    if (touches.empty() || !touches[0])
        return;

    if (!m_tileMap)
        return;

    auto diff = touches[0]->getDelta();
    m_tileMap->setPosition(m_tileMap->getPosition() + diff);
}

bool BtTiledMap::getTouchedTileInfo(cocos2d::Touch * touch, cocos2d::Vec2* tileCoord, cocos2d::Vec2* tileCenterPos)
{
    if (!touch)
        return false;

    auto layer = GetTileMapLayer(BtLayer_Background);
    if (!layer)
        return false;

    auto lLoc = layer->convertTouchToNodeSpace(touch);

    if (!getTileCoord(lLoc, tileCoord))
        return false;

    if (!getTilePosition(*tileCoord, tileCenterPos))
        return false;

    return true;
}

bool BtTiledMap::getTileCoord(const cocos2d::Vec2& pos, cocos2d::Vec2* tileCoord)
{
    auto layer = GetTileMapLayer(BtLayer_Background);
    if (!layer)
        return false;

    cocos2d::Mat4 n2t = layer->tileToNodeTransform().getInversed();
    auto tLoc = PointApplyTransform(pos, n2t);
    *tileCoord = cocos2d::Vec2(BtRound(tLoc.x), BtRound(tLoc.y));
    return true;
}

bool BtTiledMap::getTilePosition(const cocos2d::Vec2& tileCoord, cocos2d::Vec2* tileCenterPos)
{
    auto layer = GetTileMapLayer(BtLayer_Background);
    if (!layer)
        return false;

    *tileCenterPos = layer->getPositionAt(tileCoord);
    return true;
}

void BtTiledMap::beginSocketShimmering()
{
    auto layerSockets = GetTileMapLayer(BtLayer_Sockets);
    layerSockets->setOpacity(128);
    layerSockets->setCascadeOpacityEnabled(true);

    auto action = cocos2d::RepeatForever::create(
        cocos2d::Sequence::create(
        cocos2d::FadeTo::create(BtFadingDuration, BtFadingTargetOpacity),
        cocos2d::FadeTo::create(BtFadingDuration, 255), nullptr));
    layerSockets->runAction(action);
}

void BtTiledMap::endSocketShimmering()
{
    auto layerSockets = GetTileMapLayer(BtLayer_Sockets);
    layerSockets->setOpacity(255);
    layerSockets->stopAllActions();
}

