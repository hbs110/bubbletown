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

    // leave it alone without overriding, it does some common init works
    virtual bool init();  

protected:
    // override this one to perform the actuall init 
    virtual bool do_init() = 0;

    cocos2d::Node* m_uiRoot;
};


// the default scene creation process
template <typename T>
cocos2d::Scene* BtCreateScene()
{
    T *layer = T::create(); // create() should guarantee the layer pointer is autoreleased 
    if (!layer)
        return nullptr;

    auto scene = cocos2d::Scene::create();  // Scene::create() ensures creating autorelease object
    if (!scene)
        return nullptr;

    scene->addChild(layer);
    return scene;
}
