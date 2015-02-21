/*!
 * \file BtCoreUtil.cpp
 * \date 2015/02/21 20:31
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtCoreUtil
*/

#include "stdafx.h"
#include "BtCoreUtil.h"

static int s_unexpectedCnt = 0;

void BtLogUnexpect(const char* exprStr, const std::string& errContext, const std::string& errMsg)
{
    CCLOGERROR("[BT_UNEXPECTED] unexpected error '#%d' occurred on '%s' : \n"
               "    Expression : %s\n"
               "    Err-detail : %s\n", 
               ++s_unexpectedCnt, errContext.c_str(), exprStr, errMsg.c_str());
}
