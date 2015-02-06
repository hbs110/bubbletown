/*!
 * \file BtGuiService.cpp
 * \date 2015/02/06 11:08
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtGuiService
*/

#include "stdafx.h"
#include "BtGuiService.h"

BT_SINGLETON_IMPL(BtGuiService);

BtGuiService::BtGuiService()
{

}

BtGuiService::~BtGuiService()
{
    Destroy();
}

bool BtGuiService::Init()
{

    return true;
}

void BtGuiService::Destroy()
{
}

cocos2d::Node* BtGuiService::LoadLayout(const char* layoutFilePath)
{

    return nullptr;
}
