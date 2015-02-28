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

#define REG_PRE_INIT_TEST(testFunc)  BtTestServices::Get()->RegisterTest(BtTest(BtTestStage::PreInit, #testFunc, testFunc))
#define BT_EXPECT_REG_TEST(testFunc) BT_EXPECT_RET_BOOL(REG_PRE_INIT_TEST(testFunc), "register test failed.", BT_DUMMY_FUNC);

bool BtTestGui_utouch_LoadAtlas()
{
    auto node = BtGui_utouch::LoadLayout("ui/titlescreen");
    BT_EXPECT_RET_BOOL(node, "", BT_DUMMY_FUNC);

    return true;
}

bool BtRegisterGuiTests_utouch()
{
    BT_EXPECT_RET_BOOL(BtTestServices::Get(), "test service not available.", BT_DUMMY_FUNC);

    BT_EXPECT_REG_TEST(BtTestGui_utouch_LoadAtlas);

    return true;
}
