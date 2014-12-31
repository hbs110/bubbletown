/*!
 * \file TexturePoolGroup.cpp
 * \date 2014/12/30 14:48
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TexturePoolGroup
*/

#include "stdafx.h"
#include "TexturePoolGroup.h"

#include "TexturePool/TexturePool.h"

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
    : m_renderTargetTexture(nullptr)
    , m_textureSize(TexPool_DefaulTextureSize)
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
    m_renderTargetTexture->retain();

    m_name = name;
    m_textureSize = textureSize;
    return true;
}

bool TexturePoolGroup::AppendCell(const TexturePoolCell& cell)
{
    if (!cell.m_sprite || !cell.m_sprite->getTexture())
        return false;

    BinPack2D::Size size = BinPack2D::Size(cell.m_sprite->getContentSize().width, cell.m_sprite->getContentSize().height);
    m_contentList += BinPack2D::Content<TexturePoolCell>(cell, BinPack2D::Coord(), size, false);
    return true;
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

bool TexturePoolGroup::Flush()
{
    if (!m_renderTargetTexture)
        return false;

    // 1. 使用 BinPack2D 生成布局
    m_contentList.Sort();

    BinPack2D::Canvas<TexturePoolCell> canvas(m_textureSize, m_textureSize);
    BinPack2D::ContentAccumulator<TexturePoolCell> remainder;
    bool success = canvas.Place( m_contentList.Get(), remainder.Get() );
    if (!success)
        return false;

    m_contentList.Get().clear();
    for (auto c : canvas.GetContents())
    {
        m_contentList += c;
    }

    // 2. 把每一个 sprite 刷到 render target 里，并修改为指向新贴图内的矩形
    m_renderTargetTexture->clear(0.0f, 0.0f, 0.0f, 0.0f);
    m_renderTargetTexture->begin();
    for( BinPack2D::Content<TexturePoolCell>& cell : m_contentList.Get() ) 
    {
        cocos2d::Sprite* sprite = cell.content.m_sprite;
        if (sprite)
        {
            auto tempSprite = cocos2d::Sprite::create(cell.content.m_originalRes);
            tempSprite->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
            tempSprite->setPosition(cell.coord.x, cell.coord.y);
            tempSprite->visit();

            if (sprite->getTexture() != m_renderTargetTexture->getSprite()->getTexture())
                sprite->setTexture(m_renderTargetTexture->getSprite()->getTexture());

            sprite->setFlippedY(true);

            sprite->setTextureRect(cocos2d::Rect(cell.coord.x, cell.coord.y, cell.size.w, cell.size.h), cell.rotated, cocos2d::Size(cell.size.w, cell.size.h));
        }
    }
    m_renderTargetTexture->end();

    cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();
    return true;
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
    m_contentList.Get().clear();
    // 这里因为 BinPack2D::Content 类没有默认的构造函数，只能挨个添加，不能使用 std::copy
    //std::copy(validCells.begin(), validCells.end(), m_contentList.Get().begin());
    for (auto c : validCells)
    {
        m_contentList += c;
    }

    Flush();
}

