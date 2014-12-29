/*!
 * \file TexturePool.h
 * \date 2014/12/29 17:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePool 
*/

#pragma once

class TexturePool 
{
public: 
    TexturePool() {}
    ~TexturePool() {}

    /*
        ��ʼ������أ�����ͨ������Ĳ����������ڴ��ռ�������
            internalTextureCount - �ڲ�ά����ͼ������
            internalTextureSize - �ڲ�ά����ͼ�ĳߴ� 
    */
    bool Init(int internalTextureCount, int internalTextureSize);
    
    // ��������أ��ͷ�����ռ�õ��ڴ�
    void Destroy();


    /*
        �Ѿ���������
            tag - ���ָ�������������ͬ tag ����ͼ���ᱻ����ͬһ���ڲ���ͼ
            manualReleaseRequired - ���Ϊ true ������ֶ��ͷţ����Ϊ false �����������������ֶ�����ʱ���ͷš�

        ʵ��ϸ�ڣ�
            �� sprite ָ���ԭ��ͼ��������������أ������ɹ��� sprite ��ָ����ڵĶ�Ӧλ��
     */
    bool PushSprite(cocos2d::Sprite* sprite, const std::string& tag = "", bool manualReleaseRequired = false);
    
    // ����������Ƴ��þ��飬ע�⣬�þ��齫��ָ���ԭʼ��ͼ�����ã�����ɹ��Ļ��Կ����ں�������
    void ReleaseSprite(cocos2d::Sprite* sprite);

    /*
        ���������ʹ�����
            releaseIfPossible - ���Ϊ true ����ͷ���Щû��ǿ��Ҫ���ֶ��ͷŵľ��飬���Ϊ false �����ֻ�����������������е�λ��
     */
    void Defrag(bool releaseIfPossible = true);

private:

};

