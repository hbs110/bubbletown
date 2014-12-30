/*!
 * \file TexturePoolGroup.cpp
 * \date 2014/12/30 14:48
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TexturePoolGroup
*/

#include "stdafx.h"
#include "TexturePoolGroup.h"


TexturePoolCell::TexturePoolCell()
{
    Clear();
}

void TexturePoolCell::Clear()
{
    m_sprite = nullptr;
    m_originalRes = "";
    m_couldBeRecycled = true;
}

void TexturePoolCell::TryToResetSpriteToOriginal()
{
    Clear();

    // !! not implemented yet !!    - would write the actual recovery logic on demand
}

TexturePoolGroup::TexturePoolGroup() 
    : m_contentCanvas(nullptr)
    , m_renderTargetTexture(nullptr)
{

}

TexturePoolGroup::~TexturePoolGroup()
{
    if (m_renderTargetTexture)
    {
        m_renderTargetTexture->release();
        cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }
}

bool TexturePoolGroup::Init(const std::string& name, int textureSize)
{
    m_renderTargetTexture = cocos2d::RenderTexture::create(textureSize, textureSize, cocos2d::Texture2D::PixelFormat::RGBA8888);
    if (!m_renderTargetTexture)
        return false;

    m_name = name;
    m_contentCanvas = new BinPack2D::Canvas<TexturePoolCell>(textureSize, textureSize);
    return true;
}

void TexturePoolGroup::AppendCell(const TexturePoolCell& cell, bool flushImmediately /*= false*/)
{
    if (!cell.m_sprite || !cell.m_sprite->getTexture())
        return;

    BinPack2D::Size size = BinPack2D::Size(cell.m_sprite->getTexture()->getPixelsWide(), cell.m_sprite->getTexture()->getPixelsHigh());
    m_contentList += BinPack2D::Content<TexturePoolCell>(cell, BinPack2D::Coord(), size, false);

    if (flushImmediately)
    {
        Flush();
    }
}

bool TexturePoolGroup::RemoveCell(cocos2d::Sprite* sprite)
{
    for (auto it = m_contentList.Get().begin(); it != m_contentList.Get().end(); ++it)
    {
        if (it->content.m_sprite == sprite)
        {
            it->content.TryToResetSpriteToOriginal();
            return true;
        }
    }

    return false;
}

void TexturePoolGroup::Flush()
{
    // 1. 使用 BinPack2D 生成布局
    m_contentList.Sort();

    // 2. 把贴图刷到 render target 里


    // 3. 把 sprite 修改为指向新贴图内的矩形

}

void TexturePoolGroup::Defrag()
{
    std::vector<BinPack2D::Content<TexturePoolCell>> validCells;
    for (auto it = m_contentList.Get().begin(); it != m_contentList.Get().end(); ++it)
    {
        if (it->content.IsRecyclable())
        {
            it->content.TryToResetSpriteToOriginal();
        }
        else
        {
            validCells.push_back(*it);
        }
    }
    m_contentList.Get().resize(validCells.size());
    std::copy(validCells.begin(), validCells.end(), m_contentList);

    Flush();
}

