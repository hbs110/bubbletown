/*!
 * \file TexturePool.h
 * \date 2014/12/29 17:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePool 
*/

#pragma once

#include "Core/BtCoreDef.h"

const int   TexPool_MaxGroupCount = 2;

BtConstStr  TexPool_DefaulGroupName     = "__default__";
const int   TexPool_DefaulTextureSize = 2048;

BtConstStr  TexPool_Group_UI            = "UI";
BtConstStr  TexPool_Group_Character     = "characters";
BtConstStr  TexPool_Group_Effect        = "effects";


struct TexPoolGroupInitParam
{
public:
    const char* name;
    int size;
};

const TexPoolGroupInitParam GDefaultTexturePoolInitParams[TexPool_MaxGroupCount] = 
{
    { TexPool_DefaulGroupName,  TexPool_DefaulTextureSize },
    { TexPool_Group_UI,         TexPool_DefaulTextureSize },
    //{ TexPool_Group_Effect,     TexPool_DefaulTextureSize },
};

class TexturePoolGroup;

class TexturePool 
{
public: 
    TexturePool();
    ~TexturePool();

    /*
        ��ʼ������أ�����ͨ������Ĳ����������ڴ��ռ�������
            initParams - Ϊ����ÿһ�� group �ֱ��ƶ���ʼ������
            initParamCnt - initParams ������������ЧԪ�صĸ��� 
    */
    bool Init(const TexPoolGroupInitParam* initParams, int initParamCnt);
    
    // ��������أ��ͷ�����ռ�õ��ڴ�
    void Destroy();
    // ��ǰ��������Ƿ����
    bool IsValid();


    /*
        �Ѿ���������
            groupName - ָ��������������ᱻ�����Ӧ����ͼ�飻δָ�������Ʋ����������Ĭ����
            manualReleaseRequired - ���Ϊ true ������ֶ��ͷţ����Ϊ false �����������������ֶ�����ʱ���ͷ�

        ʵ��ϸ�ڣ�
            �� sprite ָ���ԭ��ͼ��������������أ������ɹ��� sprite ��ָ����ڵĶ�Ӧλ��
     */
    bool PushSprite(cocos2d::Sprite* sprite, const std::string& groupName = "", bool manualReleaseRequired = false);
    /* 
        ����������Ƴ��þ���
    */
    void ReleaseSprite(cocos2d::Sprite* sprite);


    /* 
        ֱ���� Image Ϊ��λ�����������������ͬ PushSprite / ReleaseSprite������׸��
    */
    bool PushImage(const std::string& imagePath, const std::string& groupName = "", bool manualReleaseRequired = false);
    void ReleaseImage(const std::string& imagePath);
    /*
        ��ȡ�Ѿ��ڳ��е� Image ��Ϣ
            - outTexture ��ͼ�����ڵ���ͼ
            - outRect ��ͼ�����ڵ�λ��
    */
    bool GetImageRect(const std::string& imagePath, cocos2d::Texture2D** outTexture, cocos2d::Rect* outRect);


    /* 
        ˢ���������ָ���� group�����δָ����ȫ��ˢ�¡�
    */
    void Flush(const std::string& groupName = "");
    /*
        ��������ص�ʹ�����

        ϸ�ڣ�
            ����ʱ���ͷ���Щ�ɻ��յľ��飨PushSprite ʱ manualReleaseRequired ʹ����Ĭ�ϲ��� false��
            �����ǹ�����һ�� bool flushAfterDefrag������û�мӣ���Ϊ����������£��ո� defrag���Ƿ� flush �������κοɼ�������
     */
    void Defrag();


private:
    TexturePoolGroup* FindGroup(const std::string& groupName);

    TexturePoolGroup* m_groups[TexPool_MaxGroupCount];
};

