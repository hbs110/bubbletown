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
    BtBaseScene() : m_uiRoot(nullptr) {}
    virtual ~BtBaseScene() {}

    // leave it alone without overriding, it does some common initialization works
    virtual bool init();  

protected:
    // override this one to perform the actuall init work
    virtual bool do_init() = 0;

    cocos2d::Node* m_uiRoot;
};

