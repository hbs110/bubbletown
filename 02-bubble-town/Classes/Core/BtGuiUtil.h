/*!
 * \file BtGuiUtil.h
 * \date 2014/12/23 9:57
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtGuiUtil 
*/

#pragma once

class BtGuiListener
{
public:
    virtual ~BtGuiListener() {}

    virtual void OnMenuItem(cocos2d::Ref* sender) = 0;
};


class BtGuiUtil 
{
public: 
    static cocos2d::MenuItemLabel* CreateMenuItem(const char* val, BtGuiListener* listener);
    static cocos2d::Menu* CreateMenu(const char* itemTexts[], int itemCnt, BtGuiListener* listener);
};

