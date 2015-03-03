/*!
 * \file BtSceneFactory.h
 * \date 2015/03/03 13:57
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtSceneFactory 
*/

#pragma once

#include "Core/BtCoreDef.h"

using sceneCreator_t = std::function<cocos2d::Scene* ()>;

class BtSceneFactory 
{
    BT_SINGLETON_DEF(BtSceneFactory);

public:
    bool Init();
    void Destroy();

    template <typename T>
    void RegisterSceneCreator()
    {
        m_sceneCreators[T::getSceneNameStatic()] = std::bind(&BtSceneFactory::CreateScene < T >, this);
    }

    cocos2d::Scene* Create(const std::string& sceneName);

private:
    template <typename T>
    cocos2d::Scene* CreateScene()
    {
        T *layer = T::create(); // create() should guarantee the layer pointer is autoreleased 
        if (!layer)
            return nullptr;
        layer->setName(T::getSceneNameStatic());

        auto scene = cocos2d::Scene::create();  // Scene::create() ensures creating autorelease object
        if (!scene)
            return nullptr;
        scene->setName(T::getSceneNameStatic());

        scene->addChild(layer);
        return scene;
    }

    std::map<std::string, sceneCreator_t> m_sceneCreators;
};

