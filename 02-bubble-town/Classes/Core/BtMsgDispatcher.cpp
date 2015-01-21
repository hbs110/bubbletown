/*!
 * \file BtMsgDispatcher.cpp
 * \date 2014/12/25 11:10
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtMsgDispatcher
*/

#include "stdafx.h"
#include "BtMsgDispatcher.h"

BT_SINGLETON_IMPL(BtMsgDispatcher);

BtMsgDispatcher::BtMsgDispatcher() 
{

}

BtMsgDispatcher::~BtMsgDispatcher()
{

}

bool BtMsgDispatcher::Subscribe(int msgID, msgHandler_t h)
{
    auto it = m_handlers.find(msgID);
    if (it == m_handlers.end())
    {
        auto result = m_handlers.insert(std::make_pair(msgID, msgHandlerList_t()));
        if (!result.second)
        {
            CCLOGERROR("%s failed: cannot insert '%d' into m_handlers.", __FUNCTION__, msgID);
            return false;
        }

        it = result.first;
    }

    msgHandlerList_t& handlerList = it->second;
    for (auto handler : handlerList)
    {
        if (handler == h)
        {
            CCLOG("inserting a handler (%d) which is already in the handler list, ignored.", msgID);
            return true;
        }
    }

    handlerList.push_back(h);
    return true;
}

bool BtMsgDispatcher::SubscribeExclusively(int msgID, msgHandler_t h)
{
    // 'exclusively' means there would be only one handler at most (would fail if there is alrady a handler)
    auto it = m_handlers.find(msgID);
    if (it != m_handlers.end() && !it->second.empty())
        return false;

    return Subscribe(msgID, h);
}

bool BtMsgDispatcher::Notify(BtMsg& msg, float deferred /*= 0.0f*/)
{
    auto it = m_handlers.find(msg.m_id);
    if (it == m_handlers.end())
        return false;

    bool handled = false;
    for (auto handler : it->second)
    {
        if (handler)
        {
            handled = handler(msg);
            if (handled)
            {
                return true;
            }
        }
    }

    return false;
}

void BtMsgDispatcher::ProcessedDeferred()
{

}

void BtEmitMessage(int msgId, const std::string& msgInfo /*= ""*/, float deferred /*= 0.0f*/)
{
    if (!BtMsgDispatcher::Get())
        return;

    BtMsg msg(msgId, msgInfo);
    BtMsgDispatcher::Get()->Notify(msg, deferred);
}
