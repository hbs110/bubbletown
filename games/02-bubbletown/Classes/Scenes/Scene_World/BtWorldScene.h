/*!
 * \file BtWorldScene.h
 * \date 2014/12/25 17:36
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtWorldScene 
*/

#pragma once

#include "Core/BtMsgDef.h"
#include "Scenes/BtBaseScene.h"

class BtWorldScene : public BtBaseScene
{
public:
    BtWorldScene() {}
    virtual ~BtWorldScene() {}
    BT_DEF_SCENE(BtWorldScene, BTSCN_world);

protected:
    virtual bool do_init();  
};

