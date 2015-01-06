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

    CREATE_FUNC(TexturePoolTestScene_Anim);

private:
    void OnMenuItem(cocos2d::Ref* sender);

    TexturePool* m_texturePool;

    std::vector<std::string> m_vArmatrueName;
    std::vector<cocostudio::Armature *>m_vArmature;

    void addArmatures_Single();
    void addArmatures_Uniform();
    void addArmatures_Different();
    void flushArmatures();
    void clearArmatures();
};

