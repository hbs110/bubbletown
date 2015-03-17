/*!
 * \file BtStartScene.h
 * \date 2015/03/03 17:22
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtStartScene 
*/

#pragma once

#include "Core/BtMsgDef.h"
#include "Scenes/BtBaseScene.h"

class BtStartScene : public BtBaseScene
{
public: 
    BtStartScene() {}
    virtual ~BtStartScene() {}
    BT_DEF_SCENE(BtStartScene, BTSCN_start);

protected:
    virtual bool do_init();
};

