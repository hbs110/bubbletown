/*!
 * \file TexturePoolTestScene_Anim.h
 * \date 2015/01/04 16:51
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePoolTestScene_Anim 
*/

#pragma once

#include <cocostudio/CocoStudio.h>

class TexturePool;

class TexturePoolTestScene_Anim : public cocos2d::Layer
{
public: 
    TexturePoolTestScene_Anim() : m_texturePool(nullptr) {}
    virtual ~TexturePoolTestScene_Anim();

    virtual bool init();  

    static cocos2d::Scene* scene();

    CREATE_FUNC(TexturePoolTestScene_Anim);

private:
    void OnMenuItem(cocos2d::Ref* sender);

    TexturePool* m_texturePool;

    void initArmatrueName();

    std::vector<std::string> m_vArmatrueName;
    std::vector<cocostudio::Armature *>m_vArmature;
};

