/*!
 * \file BtTestDef.h
 * \date 2015/02/27 14:37
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtTestDef 
*/

#pragma once

#include <functional>

enum class BtTestStage
{
    PreInit,    // run separated individual tests before they are initialized as game modules/services
    PostInit,   // run end-to-end tests after they are initialized and assembled
};

using fnTest = std::function < bool() >;

class BtTest
{
public: 
    BtTest(BtTestStage testStage, const char* testName, fnTest testFunc)
        : BtTest(testStage, testName, "", "", testFunc) {}

    BtTest(BtTestStage testStage, const char* testName, const char* testPrerequisite, fnTest testFunc)
        : BtTest(testStage, testName, "", testPrerequisite, testFunc) {}

    BtTest(BtTestStage testStage, const char* testName, const char* testTag, const char* testPrerequisite, fnTest testFunc)
        : name(testName), tag(testTag), prerequisite(testPrerequisite), func(testFunc), stage(testStage) {}

    std::string name;
    std::string tag;
    std::string prerequisite;
    fnTest func;
    BtTestStage stage;
};


