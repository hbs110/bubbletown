/*!
 * \file BtBubbleArena.h
 * \date 2015/03/23 9:21
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtBubbleArena 
*/

#pragma once

#include "Core/BtCoreDef.h"

class BtBubbleArena 
{
public: 
    BtBubbleArena();
    virtual ~BtBubbleArena() {}

    bool init();

    cocos2d::Layer* getCombatLayer() const { return m_combat; }

protected:
    cocos2d::Layer* m_combat;
};

