/*!
 * \file BtMsgDispatcher.h
 * \date 2014/12/25 11:10
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtMsgDispatcher 
*/

#pragma once

#include "Core/BtCoreDef.h"
#include "Core/BtMsgDef.h"
#include "Core/BtMsgHandlerSet.h"

enum class BtMsgFlag
{
    Sys = 1 << 0,
    User = 1 << 1,
    Exclusively = 1 << 2,   // 'exclusively' means only one user handler is allowed at most (would fail if there is already a handler)
};

class BtMsgDispatcher 
{
    BT_SINGLETON_DEF(BtMsgDispatcher);

public: 
    bool subscribe(int msgID, msgHandler_t h, btflags flags = (btflags)BtMsgFlag::User);
    bool notify(BtMsg& msg, float deferred = 0.0f);
    void tick(float currentTime, float deltaSeconds);   // msgs which are deferred will be broadcasted while ticking

private:
    float m_curTime;
    std::vector<BtMsg> m_incomingMsgs; // a standalone incoming queue to prevent adding new messages to the ongoing processing queue
    std::vector<BtMsg> m_deferredMsgs;
    std::vector<BtMsg> m_unhandledMsgs;

    // handlers
    BtMsgHandlerSet m_sysHandlers;
    BtMsgHandlerSet m_userHandlers;
};
