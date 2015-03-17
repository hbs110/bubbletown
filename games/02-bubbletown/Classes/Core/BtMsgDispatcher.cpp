/*!
 * \file BtMsgDispatcher.cpp
 * \date 2014/12/25 11:10
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtMsgDispatcher
*/

#include "stdafx.h"
#include "BtMsgDispatcher.h"

#include "BtCoreUtil.h"

const float BT_UnhandledMsgRemovalTime = 3.0f;

BT_SINGLETON_IMPL(BtMsgDispatcher);

BtMsgDispatcher::BtMsgDispatcher()
    : m_curTime(0.0f)
{
}

BtMsgDispatcher::~BtMsgDispatcher()
{

}

bool BtMsgDispatcher::subscribe(int msgID, msgHandler_t h, btflags flags)
{
    bool exclusively = BtHasFlag(flags, BtMsgFlag::Exclusively);

    if (BtHasFlag(flags, BtMsgFlag::User))
    {
        return m_userHandlers.subscribe(msgID, h, exclusively);
    }
    if (BtHasFlag(flags, BtMsgFlag::Sys))
    {
        return m_sysHandlers.subscribe(msgID, h, exclusively);
    }

    BT_ERROR("subscribe handler failed: neither 'Sys' nor 'User' is marked. (msg: %d).", msgID);
    return false;
}

bool BtMsgDispatcher::notify(BtMsg& msg, float deferred /*= 0.0f*/)
{
    BtMsg added(msg);
    added.markAsReceived(m_curTime, deferred);
    m_incomingMsgs.push_back(added);
    return false;
}

void BtMsgDispatcher::tick(float currentTime, float deltaSeconds)
{
    m_curTime = currentTime;

    for (auto& msg : m_incomingMsgs)
        m_deferredMsgs.push_back(msg);

    m_incomingMsgs.clear();

    for (auto& msg : m_deferredMsgs)
    {
        if (currentTime - msg.m_creationTime >= msg.m_delayTimer)
        {
            BtMsgHandlingResult resultSys = m_sysHandlers.process(msg);
            BtMsgHandlingResult resultUser = m_userHandlers.process(msg);

            msg.m_processed = true;

            if (!BtMsgHandled(resultSys) && !BtMsgHandled(resultUser))
            {
                BT_VERB("warn: message (%d) is unhandled.", msg.m_id);
                m_unhandledMsgs.push_back(msg);
            }
        }
    }

    if (m_deferredMsgs.size())
    {
        auto it = std::remove_if(m_deferredMsgs.begin(), m_deferredMsgs.end(), [](BtMsg& msg) { return msg.m_processed; });
        if (it != m_deferredMsgs.end())
        {
            m_deferredMsgs.erase(it);
        }
    }

    if (m_unhandledMsgs.size())
    {
        auto it = std::remove_if(m_unhandledMsgs.begin(), m_unhandledMsgs.end(), [currentTime](BtMsg& msg) { return currentTime - msg.m_creationTime > BT_UnhandledMsgRemovalTime; });
        if (it != m_unhandledMsgs.end())
        {
            m_unhandledMsgs.erase(it);
        }
    }
}

