/*!
 * \file BtTiledMap.h
 * \date 2015/01/22 10:10
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtTiledMap 
*/

#pragma once

#include "Core/BtCoreDef.h"

typedef cocos2d::experimental::TMXTiledMap tileMap_t;
typedef cocos2d::experimental::TMXLayer tileLayer_t;

BtConstStr BtLayer_Background = "layer_background";

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

    bool getTouchedTileInfo(cocos2d::Touch * touch, cocos2d::Vec2* tileCoord, cocos2d::Vec2* tileCenterPos);

    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

private:
    tileMap_t* m_tileMap;
    cocos2d::Node* m_spriteRoot;
};

