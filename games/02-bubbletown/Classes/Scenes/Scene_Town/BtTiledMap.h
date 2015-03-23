/*!
 * \file BtTiledMap.h
 * \date 2015/01/22 10:10
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtTiledMap 
*/

#pragma once

#include "Core/BtCoreDef.h"

BtConstStr BtLayer_Background   = "layer_background";
BtConstStr BtLayer_Sockets      = "layer_sockets";

class BtTiledMap 
{
public: 
    BtTiledMap() : m_tileMap(nullptr), m_spriteRoot(nullptr) {}
    ~BtTiledMap() { Release(); }

    bool Load(const std::string& filename);
    void Release(); 

    tileMap_t* GetTileMapRoot() const { return m_tileMap; }
    cocos2d::Node* GetSpriteRoot() const { return m_spriteRoot; }
    tileLayer_t* GetTileMapLayer(const std::string& layerName);
    cocos2d::Size GetTileSize() { return m_tileMap->getTileSize(); }

    bool getTouchedTileInfo(cocos2d::Touch * touch, cocos2d::Vec2* tileCoord, cocos2d::Vec2* tileCenterPos);
    bool getTileCoord(const cocos2d::Vec2& pos, cocos2d::Vec2* tileCoord);
    bool getTilePosition(const cocos2d::Vec2& tileCoord, cocos2d::Vec2* tileCenterPos);
    
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

    bool isInSocketArea(const cocos2d::Rect& tileRect);
    void beginSocketShimmering();
    void endSocketShimmering();

private:
    tileMap_t* m_tileMap;
    cocos2d::Node* m_spriteRoot;
};

