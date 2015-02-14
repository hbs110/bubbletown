/*!
 * \file BtMsgDispatcher.h
 * \date 2014/12/25 11:10
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtMsgDispatcher 
*/

#pragma once

#include "Core/BtCoreDef.h"

class BtMsg
{
public:
    BtMsg() : BtMsg(BT_INVALID_ID) {}
    BtMsg(int msgId, const std::string& info) : m_id(msgId), m_info(info), m_creationTime(0.0f), m_delayTimer(0.0f), m_processed(false) {}
    explicit BtMsg(int msgId) : BtMsg(msgId, "") {}
    virtual ~BtMsg() {} // derive from this class is allowed (but not encouraged, use userdata for lightweight information delivering)

    void markAsReceived(float created, float deferred)
    {
        m_creationTime = created;
        m_delayTimer = deferred;
    }

    int m_id;
    std::string m_info;
    float m_creationTime;
    float m_delayTimer;
    bool m_processed;

    std::vector<std::string> m_extraParams;
};

typedef std::function<bool(BtMsg& msg)> msgHandler_t;
typedef std::vector<msgHandler_t> msgHandlerList_t;

inline bool operator == (msgHandler_t h1, msgHandler_t h2)
{
    return h1.target<bool(BtMsg& msg)>() == h2.target<bool(BtMsg& msg)>();
}

class BtMsgDispatcher 
{
    BT_SINGLETON_DEF(BtMsgDispatcher);

public: 
    bool subscribe(int msgID, msgHandler_t h);
    bool subscribeExclusively(int msgID, msgHandler_t h);

    bool receive(BtMsg& msg, float deferred = 0.0f);
    void tick(float currentTime, float deltaSeconds);   // msgs which are deferred will be broadcasted while ticking

private:
    std::unordered_map<int, msgHandlerList_t> m_handlers;
    std::vector<BtMsg> m_deferredMsgs;

    float m_curTime;

    bool processMessage(BtMsg& msg, msgHandlerList_t& handlers);
};

void BtEmitMessage(int msgId, const std::string& msgInfo = "", float deferred = 0.0f);
