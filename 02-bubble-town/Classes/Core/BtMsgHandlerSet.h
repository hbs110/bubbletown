/*!
 * \file BtMsgHandlerSet.h
 * \date 2015/03/03 11:23
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtMsgHandlerSet 
*/

#pragma once

#include "Core/BtMsgDef.h"

typedef std::function<bool(BtMsg& msg)> msgHandler_t;
typedef std::vector<msgHandler_t> msgHandlerList_t;

using msgHandlerMap_t = std::unordered_map < int, msgHandlerList_t > ;

inline bool operator == (msgHandler_t h1, msgHandler_t h2)
{
    return h1.target<bool(BtMsg& msg)>() == h2.target<bool(BtMsg& msg)>();
}

class BtMsgHandlerSet 
{
public: 
    BtMsgHandlerSet() {}
    ~BtMsgHandlerSet() {}

    bool subscribe(int msgID, msgHandler_t h, bool exclusively = false);
    BtMsgHandlingResult process(BtMsg& msg);

private:
    msgHandlerMap_t m_handlers;
};

