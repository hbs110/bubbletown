/*!
 * \file AppMsgHandlers.h
 * \date 2015/03/03 13:51
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of AppMsgHandlers 
*/

#pragma once

#include "Core/BtMsgDispatcher.h"

class AppMsgHandlers 
{
public:
    static void registerGlobally();

    // handlers are intended to be private, so that the only way to call them is 'send a message'
private:
    static bool onGotoScene(BtMsg& msg);

    static bool onButtonPressed(BtMsg& msg);
    static bool onButtonReleased(BtMsg& msg);
};

