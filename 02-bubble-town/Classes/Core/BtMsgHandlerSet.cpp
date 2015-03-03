/*!
 * \file BtMsgHandlerSet.cpp
 * \date 2015/03/03 11:23
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtMsgHandlerSet
*/

#include "stdafx.h"
#include "BtMsgHandlerSet.h"

#include "Core/BtCoreUtil.h"

bool BtMsgHandlerSet::subscribe(int msgID, msgHandler_t h, bool exclusively)
{
    auto it = m_handlers.find(msgID);
    if (it != m_handlers.end())
    {
        bool hasAlreadRegistered = it->second.size() != 0;
        if (hasAlreadRegistered && exclusively)
            return false;
    }
    else
    {
        auto result = m_handlers.insert(std::make_pair(msgID, msgHandlerList_t()));
        BT_EXPECT_RET(result.second, tfm::format("subscribe handler failed (msg: %d).", msgID), false);

        it = result.first;
    }

    msgHandlerList_t& handlerList = it->second;
    for (auto handler : handlerList)
    {
        if (handler == h) // see the overloaded '==' in header
        {
            CCLOG("inserting a handler (%d) which is already in the handler list, ignored.", msgID);
            return true;
        }
    }

    handlerList.push_back(h);
    return true;
}

BtMsgHandlingResult BtMsgHandlerSet::process(BtMsg& msg)
{
    auto it = m_handlers.find(msg.m_id);
    if (it == m_handlers.end())
        return BtMsgHandlingResult::Untouched;

    bool handled = false;
    for (auto& handler : it->second)
    {
        if (handler && handler(msg))
        {
            handled = true;
        }
    }
    return handled ? BtMsgHandlingResult::Successfully : BtMsgHandlingResult::Failed;
}
