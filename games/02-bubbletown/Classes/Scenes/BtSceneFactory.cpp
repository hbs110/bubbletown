/*!
 * \file BtSceneFactory.cpp
 * \date 2015/03/03 13:57
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtSceneFactory
*/

#include "stdafx.h"
#include "BtSceneFactory.h"

#include <tinyformat.h>
#include "Core/BtCoreUtil.h"

BT_SINGLETON_IMPL(BtSceneFactory);

BtSceneFactory::BtSceneFactory()
{

}

BtSceneFactory::~BtSceneFactory()
{
    Destroy();
}

bool BtSceneFactory::Init()
{

    return true;
}

void BtSceneFactory::Destroy()
{
}

cocos2d::Scene* BtSceneFactory::Create(const std::string& sceneName)
{
    auto it = m_sceneCreators.find(sceneName);
    if (it == m_sceneCreators.end())
        return false;

    cocos2d::Scene* scene = it->second();
    BT_EXPECT_RET(scene, tfm::format("The creator of scene ('%s') not found, scene creation failed.", sceneName), nullptr);

    return scene;
}
