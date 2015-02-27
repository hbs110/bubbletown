/*!
 * \file BtTestServices.cpp
 * \date 2015/02/27 12:01
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtTestServices
*/

#include "stdafx.h"
#include "BtTestServices.h"

#include "Core/BtCoreUtil.h"
#include "Core//BtCoreDef.h"

BT_SINGLETON_IMPL(BtTestServices);

BtTestServices::BtTestServices()
{

}

BtTestServices::~BtTestServices()
{
    Destroy();
}

bool BtTestServices::Init()
{

    return true;
}

void BtTestServices::Destroy()
{
}


bool BtTestServices::RegisterTest(const BtTest& test)
{
    BT_EXPECT_RET_BOOL(!test.name.empty(), "invalid test name", BT_DUMMY_FUNC);
    BT_EXPECT_RET_BOOL(test.func, "no test func specified", BT_DUMMY_FUNC);

    // prerequisite checking
    auto& testList = m_tests[test.stage];
    if (test.prerequisite.size())
    {
        bool found = false;
        for (auto& test : testList)
        {
            if (test.name == test.prerequisite)
            {
                found = true;
                break;
            }
        }

        BT_EXPECT_RET_BOOL(found, tfm::format("test has a prerequisite '%s' but not found in existing test list.", test.prerequisite), BT_DUMMY_FUNC);
    }

    // always append the test at the tail so that it always runs after its prerequisite
    testList.push_back(test);
    return true;
}

bool BtTestServices::RunTestsOnStage(BtTestStage stage)
{
    auto& testList = m_tests[stage];
    for (auto& test : testList)
    {
        BT_VERB("running test '%s'...", test.name);
        if (!test.func())
        {
            BT_UNEXPECTED(tfm::format("test '%s' failed!", test.name), BT_DUMMY_FUNC);
            BT_ERROR("test stage '%d' aborted.", (int)stage);
            return false;
        }
    }

    return true;
}
