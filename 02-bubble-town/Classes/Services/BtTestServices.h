/*!
 * \file BtTestServices.h
 * \date 2015/02/27 12:01
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtTestServices 
*/

#pragma once

#include "Core/BtCoreDef.h"
#include "BtTestDef.h"

class BtTestServices 
{
    BT_SINGLETON_DEF(BtTestServices);

public:
    bool Init();
    void Destroy();

    bool RegisterTest(const BtTest& test);
    bool RunTestsOnStage(BtTestStage stage);
    
private:
    std::map<BtTestStage, std::vector<BtTest>> m_tests;
};

