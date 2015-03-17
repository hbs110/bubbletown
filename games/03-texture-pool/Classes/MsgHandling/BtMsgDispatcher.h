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
    BtMsg() : m_id(BT_INVALID_ID), m_creationTime(0.0f) {}
    BtMsg(int msgId, const std::string& info) : m_id(msgId), m_info(info), m_creationTime(0.0f) {}
    explicit BtMsg(int msgId) : m_id(msgId), m_creationTime(0.0f) {}
    virtual ~BtMsg() {} // derive from this class is allowed (but not encouraged, use userdata for lightweight information delivering)

    int m_id;
    std::string m_info;
    float m_creationTime;
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
    bool Subscribe(int msgID, msgHandler_t h);
    bool SubscribeExclusively(int msgID, msgHandler_t h);

    //===================================
    // !!!NOTE!!! the feature 'deferred' is not implemented yet, would do it on demand
    bool Notify(BtMsg& msg, float deferred = 0.0f);
    void ProcessedDeferred();   // msgs which are deferred will be broadcasted while ticking

private:
    std::unordered_map<int, msgHandlerList_t> m_handlers;
    std::vector<BtMsg> m_deferredMsgs;
};

