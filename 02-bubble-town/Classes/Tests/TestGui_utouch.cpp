/*!
 * \file TestGui_utouch.cpp
 * \date 2015/02/27 11:59
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TestGui_utouch
*/

#include "stdafx.h"
#include "TestGui_utouch.h"

#include "Core/BtCoreUtil.h"
#include "Services/BtTestServices.h"

#include "Services/BtGui_utouch.h"

#define BT_REGISTER_TEST(stage, func)    BT_EXPECT_RET_V2( \
    BtTestServices::Get()->RegisterTest(BtTest(stage, #func, func)), \
    tfm::format("registering test '%s' failed.", #func), \
    false);

bool BtTestGui_utouch_LoadAtlas()
{
    auto node = BtGui_utouch::LoadLayout("ui/titlescreen");
    BT_EXPECT_RET_V2(node, "", false);

    return true;
}

bool BtRegisterGuiTests_utouch()
{
    BT_EXPECT_RET_V2(BtTestServices::Get(), "test service not available.", false);

    BT_REGISTER_TEST(BtTestStage::PostInit, BtTestGui_utouch_LoadAtlas);

    return true;
}
