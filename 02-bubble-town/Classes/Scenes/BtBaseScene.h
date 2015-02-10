/*!
 * \file BtBaseScene.h
 * \date 2015/01/21 9:15
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtBaseScene 
*/

#pragma once

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
