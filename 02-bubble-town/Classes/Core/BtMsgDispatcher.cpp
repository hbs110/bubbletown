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
    : m_curTime(0.0f)
{
}

BtMsgDispatcher::~BtMsgDispatcher()
{

}

bool BtMsgDispatcher::subscribe(int msgID, msgHandler_t h)
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

bool BtMsgDispatcher::subscribeExclusively(int msgID, msgHandler_t h)
{
    // 'exclusively' means there would be only one handler at most (would fail if there is alrady a handler)
    auto it = m_handlers.find(msgID);
    if (it != m_handlers.end() && !it->second.empty())
        return false;

    return subscribe(msgID, h);
}

bool BtMsgDispatcher::receive(BtMsg& msg, float deferred /*= 0.0f*/)
{
    BtMsg added(msg);
    added.markAsReceived(m_curTime, deferred);
    m_deferredMsgs.push_back(added);
    return false;
}

void BtMsgDispatcher::tick(float currentTime, float deltaSeconds)
{
    m_curTime = currentTime;

    for (auto& msg : m_deferredMsgs)
    {
        if (currentTime - msg.m_creationTime >= msg.m_delayTimer)
        {
            auto it = m_handlers.find(msg.m_id);
            if (it != m_handlers.end())
            {
                processMessage(msg, it->second);
            }
        }
    }

    if (m_deferredMsgs.size())
    {
        m_deferredMsgs.erase(std::remove_if(m_deferredMsgs.begin(), m_deferredMsgs.end(), [](BtMsg& msg) { return msg.m_processed; }));
    }
}

bool BtMsgDispatcher::processMessage(BtMsg& msg, msgHandlerList_t& handlers)
{
    bool handled = false;
    for (auto& handler : handlers)
    {
        if (handler && handler(msg))
        {
            handled = true;
        }
    }

    msg.m_processed = true;
    return handled;
}

void BtEmitMessage(int msgId, const std::string& msgInfo /*= ""*/, float deferred /*= 0.0f*/)
{
    if (!BtMsgDispatcher::Get())
        return;

    BtMsg msg(msgId, msgInfo);
    BtMsgDispatcher::Get()->receive(msg, deferred);
}
