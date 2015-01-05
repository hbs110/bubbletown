/*!
 * \file TexturePool.h
 * \date 2014/12/29 17:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePool 
*/

#pragma once

#include "Core/BtCoreDef.h"

const int   TexPool_MaxGroupCount = 3;

BtConstStr  TexPool_DefaulGroupName = "__default__";
const int   TexPool_DefaulTextureSize = 256;

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
    { TexPool_Group_Character,  TexPool_DefaulTextureSize },
    { TexPool_Group_Effect,     TexPool_DefaulTextureSize },
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
            groupName - ���ָ��������������ᱻ�����Ӧ����ͼ�飻δָ�������Ʋ����������Ĭ����
            originalResourcePath - ���ָ�������������ô��ʽ�ص��� ReleaseSprite ���Զ������Ƴ�����ʱ���᳢�Դ����·���ָ��þ����ԭʼͼƬ��Դ������
            manualReleaseRequired - ���Ϊ true ������ֶ��ͷţ����Ϊ false �����������������ֶ�����ʱ���ͷ�

        ʵ��ϸ�ڣ�
            �� sprite ָ���ԭ��ͼ��������������أ������ɹ��� sprite ��ָ����ڵĶ�Ӧλ��
     */
    bool PushSprite(cocos2d::Sprite* sprite, const std::string& groupName = "", const std::string& originalResourcePath = "", bool manualReleaseRequired = false);

    /* 
        ����������Ƴ��þ���
            groupName - ���ָ����������������ڶ�Ӧ����ͼ���ڲ��ң�δָ�������Ʋ�������ȫ�ֲ���

        ע�⣺
            �þ��齫��ָ���ԭʼ��ͼ�����ã�����еĻ���������ò����ɹ����Կ����ں�������
    */
    void ReleaseSprite(cocos2d::Sprite* sprite, const std::string& groupName = "");

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

