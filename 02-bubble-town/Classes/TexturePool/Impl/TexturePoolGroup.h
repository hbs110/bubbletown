/*!
 * \file TexturePoolGroup.h
 * \date 2014/12/30 14:48
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePoolGroup 
*/

#pragma once

#include "Core/BtCoreDef.h"

#include "binpack2d.hpp"


class TexturePoolCell
{
public:
    TexturePoolCell();

    void Clear();
    bool IsEmpty() { return !m_sprite;  }
    bool IsRecyclable() { return IsEmpty() || m_couldBeRecycled; }
    void TryToResetSpriteToOriginal();

    cocos2d::Sprite* m_sprite;
    std::string m_originalRes;
    bool m_couldBeRecycled;
};

class TexturePoolGroup
{
public:
    TexturePoolGroup(); 
    ~TexturePoolGroup();

    bool Init(const std::string& name, int textureSize);

    bool AppendCell(const TexturePoolCell& cell);
    bool RemoveCell(cocos2d::Sprite* sprite);

    bool Flush();
    void Defrag();

    std::string GetName() const { return m_name; }

private:
    std::string m_name;
    int m_textureSize;

    BinPack2D::ContentAccumulator<TexturePoolCell> m_contentList;

    cocos2d::RenderTexture* m_renderTargetTexture;
};

