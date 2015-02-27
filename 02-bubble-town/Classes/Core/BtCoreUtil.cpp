/*!
 * \file BtCoreUtil.cpp
 * \date 2015/02/21 20:31
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtCoreUtil
*/

#include "stdafx.h"
#include "BtCoreUtil.h"

double GAppStartTime = 0.0f;
double GAppCurrentTime = 0.0f;

#if _DEBUG
BtLogMode GAppLogMode = BtLogMode::Verbose;
#else
BtLogMode GAppLogMode = BtLogMode::Typical;
#endif

void BtLogStandard(BtLogType type, const std::string& err)
{
    switch (type)
    {
    case BtLogType::Verbose:
        if (GAppLogMode >= BtLogMode::Verbose)
        {
            CCLOG(err.c_str());
        }
        break;

    case BtLogType::Normal:
        if (GAppLogMode >= BtLogMode::Typical)
        {
            CCLOG(err.c_str());
        }
        break;

    case BtLogType::Error:
        CCLOGERROR(err.c_str());
        break;
    default:
        break;
    }
}

void BtSetLogMode(BtLogMode mode)
{
    GAppLogMode = mode;
}

static int s_unexpectedCnt = 0;

void BtLogUnexpect(const char* exprStr, const std::string& errContext, const std::string& errMsg)
{
    BT_ERROR("[BT_UNEXPECTED] unexpected error '#%d' occurred: \n"
             "    Expression : %s\n"
             "    Context    : %s\n"
             "    Err-detail : %s\n", 
             ++s_unexpectedCnt, exprStr, errContext, errMsg);
}

