/*!
 * \file TexturePoolTestScene.h
 * \date 2014/12/29 17:39
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePoolTestScene 
*/

#pragma once

class TexturePoolTestScene : public cocos2d::Layer
{
public: 
    TexturePoolTestScene() {}
    virtual ~TexturePoolTestScene() {}

    virtual bool init();  

    static cocos2d::Scene* scene();

    CREATE_FUNC(TexturePoolTestScene);

private:
    void menuCloseCallback(Ref* sender);

};

