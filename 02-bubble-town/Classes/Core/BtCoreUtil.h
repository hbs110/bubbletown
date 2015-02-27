/*!
 * \file BtCoreUtil.h
 * \date 2015/02/21 20:30
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtCoreUtil 
*/

#pragma once

#include "Core/tinyformat/tinyformat.h"

extern double GAppStartTime;
extern double GAppCurrentTime;


/* ----- logging family ----- */

enum class BtLogType
{
    Verbose,    // normally depressed, should be enabled explicitly
    Normal,     // the 'typical' mode, depressed in 'silent' mode
    Error,      // outputs even in 'silent' mode
};

void BtLogStandard(BtLogType type, const std::string& err);

#define BT_VERB(info, ...)      BtLogStandard(BtLogType::Verbose, tfm::format("%.2f %s", GAppCurrentTime, tfm::format(info, __VA_ARGS__)))
#define BT_LOG(info, ...)       BtLogStandard(BtLogType::Normal, tfm::format("%.2f %s", GAppCurrentTime, tfm::format(info, __VA_ARGS__)))
#define BT_ERROR(info, ...)     BtLogStandard(BtLogType::Error, tfm::format("%.2f %s", GAppCurrentTime, tfm::format(info, __VA_ARGS__)))

enum class BtLogMode
{
    Silent,
    Typical,
    Verbose,
};

void BtSetLogMode(BtLogMode mode);


/* ----- BT_EXPECT family ----- */

#define BT_DUMMY_FUNC   [](){}          

void BtLogUnexpect(const char* exprStr, const std::string& errContext, const std::string& errMsg);

#define BT_UNEXPECTED(errMsg, callback)                     { BtLogUnexpect("unexpected error", __FUNCTION__, errMsg); callback(); }

#define BT_EXPECT(expr, errMsg, callback)                   if (!(expr)) { BtLogUnexpect(#expr, __FUNCTION__, errMsg); callback(); }
#define BT_EXPECT_RET(expr, errMsg, callback)               if (!(expr)) { BtLogUnexpect(#expr, __FUNCTION__, errMsg); callback(); return; }
#define BT_EXPECT_RET_VAL(expr, errMsg, callback, retVal)   if (!(expr)) { BtLogUnexpect(#expr, __FUNCTION__, errMsg); callback(); return retVal; }
#define BT_EXPECT_RET_BOOL(expr, errMsg, callback)          if (!(expr)) { BtLogUnexpect(#expr, __FUNCTION__, errMsg); callback(); return false; }
//TODO: add debugger checking inside these macros (break immediately on failure if a debugger is attached)

