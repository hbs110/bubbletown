/*!
 * \file TexturePool.cpp
 * \date 2014/12/29 17:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TexturePool
*/

#include "stdafx.h"
#include "TexturePool.h"

#include "Impl/TexturePoolGroup.h"

#include <cocostudio/CocoStudio.h>

TexturePool::TexturePool()
{
    memset(m_groups, 0, sizeof(m_groups));
}

TexturePool::~TexturePool()
{
    Destroy();
}

bool TexturePool::Init(const TexPoolGroupInitParam* initParams, int initParamCnt)
{
    // 目前初始化参数必须明确指定每一个 group 的名称和容量，不允许有不明确的 group 存在
    // 所以要求参数的数量与 group 的数量必须精确匹配
    if (initParamCnt != TexPool_MaxGroupCount)
        return false;

    Destroy();
    for (int i = 0; i < TexPool_MaxGroupCount; ++i)
    {
        m_groups[i] = new TexturePoolGroup();
        if (!m_groups[i]->Init(initParams[i].name, initParams[i].size))
        {
            Destroy();
            return false;
        }
    }

    return true;
}

void TexturePool::Destroy()
{
    // 请注意这里不使用 BtDeleteArray 而是每一个 group 单独处理
    for (int i = 0; i < TexPool_MaxGroupCount; ++i)
        BtDeletePointer(m_groups[i]);
}

bool TexturePool::PushSprite(cocos2d::Sprite* sprite,
                             const std::string& groupName /*= ""*/, 
                             const std::string& originalResourcePath /*= ""*/,
                             bool manualReleaseRequired /*= true*/)
{
    if (!IsValid())
        return false;

    if (!sprite || !sprite->getTexture())
        return false;

    TexturePoolGroup* targetGroup = FindGroup(groupName);
    if (!targetGroup)
        return false;

    TexturePoolCell cell;
    cell.m_sprite = sprite;
    cell.m_originalRes = originalResourcePath;
    cell.m_couldBeRecycled = !manualReleaseRequired;

    return targetGroup->AppendCell(cell);
}

bool TexturePool::PushArmature(cocostudio::Armature* armature, const std::string& groupName /*= ""*/, const std::string& originalResourcePath /*= ""*/, bool manualReleaseRequired /*= false*/)
{
    if (!IsValid())
        return false;

    //if (!armature || !armature->getsp ->getTexture())
    //    return false;

    //TexturePoolGroup* targetGroup = FindGroup(groupName);
    //if (!targetGroup)
    //    return false;

    //TexturePoolCell cell;
    //cell.m_sprite = sprite;
    //cell.m_originalRes = originalResourcePath;
    //cell.m_couldBeRecycled = !manualReleaseRequired;

    //return targetGroup->AppendCell(cell);
    return true;
}

void TexturePool::ReleaseSprite(cocos2d::Sprite* sprite, const std::string& groupName /*= ""*/)
{
    if (!IsValid())
        return;

    if (!sprite || !sprite->getTexture())
        return;

    TexturePoolGroup* targetGroup = FindGroup(groupName);
    if (targetGroup)
    {
        targetGroup->RemoveCell(sprite);
    }
    else
    {
        for (int i = 0; i < TexPool_MaxGroupCount; ++i)
            if (m_groups[i]->RemoveCell(sprite))
                break;
    }
}

void TexturePool::Flush(const std::string& groupName /*= ""*/)
{
    if (!IsValid())
        return;

    TexturePoolGroup* targetGroup = FindGroup(groupName);
    if (targetGroup)
    {
        targetGroup->Flush();
    }
    else
    {
        for (int i = 0; i < TexPool_MaxGroupCount; ++i)
            m_groups[i]->Flush();
    }
}

void TexturePool::Defrag()
{
    if (!IsValid())
        return;

    for (int i = 0; i < TexPool_MaxGroupCount; ++i)
        m_groups[i]->Defrag();
}

TexturePoolGroup* TexturePool::FindGroup(const std::string& groupName)
{
    if (!IsValid())
        return nullptr;

    for (int i = 0; i < TexPool_MaxGroupCount; ++i)
        if (m_groups[i]->GetName() == groupName)
            return m_groups[i];

    for (int i = 0; i < TexPool_MaxGroupCount; ++i)
        if (m_groups[i]->GetName() == TexPool_DefaulGroupName)
            return m_groups[i];

    return nullptr;
}

bool TexturePool::IsValid()
{
    for (int i = 0; i < TexPool_MaxGroupCount; ++i)
        if (!m_groups[i])
            return false;

    return true;
}
