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

/*
    ��ע�⣬�����ַ�ʽʹ��һ�� TexturePoolCell��
        ���Դ�һ�� sprite Ҳ���Դ�һ��ͼƬ��Դ������Ӧ���� Assign() ������

    �����ǰ�ߣ�����ػ��ڽ��� sprite ���õ�������ˢ����к��Զ����� sprite ָ��������ڵ��µ�ַ��
    ����Ǻ��ߣ�����ػ�ά��һ���ڲ��� sprite ���ⲿ��ʼ�����Կ�ʹ��ͼƬ·�����ʶ�Ӧ����Դ��

    Ҳ����˵��������ڲ�ʼ��ʹ�� sprite ���������� cell��ʹ�� cocos2d::Sprite::getContentSize() �������������ʵ�ʵĿ�����
*/
class TexturePoolCell
{
public:
    TexturePoolCell();

    bool Assign(cocos2d::Sprite* sprite, bool recyclable);
    bool Assign(const std::string& imagePath, bool recyclable);

    void Clear();
    bool IsEmpty() { return !m_sprite;  }
    bool IsRecyclable() { return IsEmpty() || m_recyclable; }
    bool ContainsImage(const std::string& image) { return m_imagePath == image; }
    cocos2d::Sprite* GetSprite() const { return m_sprite; }

private:
    bool m_recyclable;
    cocos2d::Sprite* m_sprite;
    std::string m_imagePath;
};

class TexturePoolGroup
{
public:
    TexturePoolGroup(); 
    ~TexturePoolGroup();

    bool Init(const std::string& name, int textureSize);

    bool AppendCell(const TexturePoolCell& cell);
    bool RemoveCell(cocos2d::Sprite* sprite);
    bool RemoveCell(const std::string& imagePath);
    bool GetCellRect(const std::string& imagePath, cocos2d::Texture2D** outTexture, cocos2d::Rect* outRect);

    bool Flush();
    void Defrag();

    std::string GetName() const { return m_name; }

private:
    std::string m_name;
    int m_textureSize;

    BinPack2D::ContentAccumulator<TexturePoolCell> m_contentList;

    cocos2d::RenderTexture* m_renderTargetTexture;

    void ClearCell(TexturePoolCell& cell);
};

