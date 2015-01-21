/*!
 * \file BtWorldScene.h
 * \date 2014/12/25 17:36
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtWorldScene 
*/

#pragma once

#include "Scenes/BtBaseScene.h"

class BtWorldScene : public BtBaseScene
{
public:
    BtWorldScene() {}
    virtual ~BtWorldScene() {}
    CREATE_FUNC(BtWorldScene);

protected:
    virtual bool do_init();  
};

