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

    // ----- cocos2d methods begin -----
    // please leave them alone without overriding, 
    // use do_xxx() below instead
    virtual bool init();  
    virtual void onEnter();
    virtual void onExit();
    // ----- cocos2d methods end -----

    void preEnter(const std::string& sceneConfig);

protected:
    // override this one to perform the actuall init 
    virtual bool do_init() = 0;
    virtual void do_enter() {}
    virtual void do_exit() {}

    cocos2d::Layer* m_sceneRoot;
    cocos2d::Layer* m_uiRoot;

    std::string m_preEnterConfig;
};
