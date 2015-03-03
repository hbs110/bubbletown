/*!
 * \file BtStartScene.cpp
 * \date 2015/03/03 17:22
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtStartScene
*/

#include "stdafx.h"
#include "BtStartScene.h"

#include "Services/BtGui_utouch.h"

bool BtStartScene::do_init()
{
    auto node = BtGui_utouch::LoadLayout("ui/titlescreen");
    m_uiRoot->addChild(node, 1);

    return true;
}
