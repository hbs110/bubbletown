/*!
 * \file BtCoreUtil.h
 * \date 2015/02/21 20:30
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtCoreUtil 
*/

#pragma once

#define BT_EXPECT(expr, errContext, errMsg, callback)                   if (!(expr)) { BtLogUnexpect(#expr, errContext, errMsg); callback(); }
#define BT_EXPECT_RET(expr, errContext, errMsg, callback)               if (!(expr)) { BtLogUnexpect(#expr, errContext, errMsg); callback(); return; }
#define BT_EXPECT_RET_VAL(expr, errContext, errMsg, callback, retVal)   if (!(expr)) { BtLogUnexpect(#expr, errContext, errMsg); callback(); return retVal; }
//TODO: add debugger checking inside these macros (break immediately on failure if a debugger is attached)

void BtLogUnexpect(const char* exprStr, const std::string& errContext, const std::string& errMsg);


