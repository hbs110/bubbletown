/*!
 * \file BtBaseScene.h
 * \date 2015/01/21 9:15
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtBaseScene 
*/

#pragma once

// by using this macro, developer won't forget to define the scene name in Lua (and regenerate the AUTOGEN header)
#define BT_DEF_SCENE(class_name, scene_name) \
    CREATE_FUNC(class_name); \
    public: static BtConstStr getSceneName() { return scene_name; }

class BtBaseScene : public cocos2d::Layer
{
public: 
    BtBaseScene() : m_sceneRoot(nullptr), m_uiRoot(nullptr) {}
    virtual ~BtBaseScene() {}

    // leave it alone without overriding, it does some common init works
    virtual bool init();  

protected:
    // override this one to perform the actuall init 
    virtual bool do_init() = 0;

    cocos2d::Layer* m_sceneRoot;
    cocos2d::Layer* m_uiRoot;
};
