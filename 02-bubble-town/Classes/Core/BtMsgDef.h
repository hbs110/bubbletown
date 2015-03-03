/*!
 * \file BtMsgDef.h
 * \date 2014/12/25 12:10
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtMsgDef 
*/

#pragma once

#include "BtMsgDef_AUTOGEN.h"

enum class BtMsgHandlingResult
{
    Untouched,      // no handlers registered
    Successfully,   // returns true by at least one handler
    Failed,         // every possible handler returns false
};

inline bool BtMsgHandled(BtMsgHandlingResult result)  {    return result == BtMsgHandlingResult::Successfully; }


class BtMsg
{
public:
    BtMsg() : m_id(BT_INVALID_ID), m_info(""), m_creationTime(0.0f), m_delayTimer(0.0f), m_processed(false) {}
    BtMsg(int msgId, const std::string& info) : m_id(msgId), m_info(info), m_creationTime(0.0f), m_delayTimer(0.0f), m_processed(false) {}
    explicit BtMsg(int msgId) : m_id(msgId), m_info(""), m_creationTime(0.0f), m_delayTimer(0.0f), m_processed(false) {}
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
