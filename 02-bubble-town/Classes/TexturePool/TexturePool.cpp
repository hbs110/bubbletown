/*!
 * \file TexturePool.cpp
 * \date 2014/12/29 17:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TexturePool
*/

#include "stdafx.h"
#include "TexturePool.h"


bool TexturePool::Init(int internalTextureCount, int internalTextureSize)
{

    return true;
}

void TexturePool::Destroy()
{

}

bool TexturePool::PushSprite(cocos2d::Sprite* sprite, const std::string& tag /*= ""*/, bool manualReleaseRequired /*= true*/)
{

    return true;
}

void TexturePool::ReleaseSprite(cocos2d::Sprite* sprite)
{

}

void TexturePool::Defrag(bool releaseIfPossible /*= true*/)
{

}
