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

void BtLogUnexpected(const char* exprStr, const std::string& errContext, const std::string& errMsg);

//TODO: add debugger checking inside these macros (break immediately on failure if a debugger is attached)
#define BT_EXPECT(expr, errMsg)              if (!(expr)) { BtLogUnexpected(#expr, __FUNCTION__, errMsg); }
#define BT_EXPECT_RET(expr, errMsg, ret)     if (!(expr)) { BtLogUnexpected(#expr, __FUNCTION__, errMsg); return ret; }  // 'ret' could be ignored, in this case the function returns void
#define BT_UNEXPECTED(errMsg)                { BtLogUnexpected("<null>", __FUNCTION__, errMsg); }

