/*!
 * \file BtBubbleScene.h
 * \date 2015/01/21 9:26
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtBubbleScene 
*/

#pragma once

#include "Scenes/BtBaseScene.h"

class BtBubbleScene : public BtBaseScene
{
public: 
    virtual ~BtBubbleScene() {}
    CREATE_FUNC(BtBubbleScene);

protected:
    virtual bool do_init();  
};

