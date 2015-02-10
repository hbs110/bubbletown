/*!
 * \file BtGuiService.h
 * \date 2015/02/06 11:08
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtGuiService 
*/

#pragma once

#include "Core/BtCoreDef.h"

class BtGuiService 
{
    BT_SINGLETON_DEF(BtGuiService);

public:
    bool Init();
    void Destroy();

protected:
    cocos2d::Node* LoadLayout(const char* layoutFilePath);
};

