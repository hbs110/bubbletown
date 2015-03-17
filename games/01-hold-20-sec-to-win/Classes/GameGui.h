/*!
 * \file GameGui.h
 * \date 2014/07/25 16:40
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of GameGui 
*/

#pragma once

class BtGuiListener
{
public:
    virtual ~BtGuiListener() {}

    virtual void OnMenuItem(Ref* sender) = 0;
};

class GameGui 
{
public: 
    GameGui() : m_menu(nullptr) {}
    ~GameGui() {}

    bool init(BtGuiListener* listener);  

    cocos2d::Menu* getMenu() { return m_menu; }

private:
    cocos2d::Menu* m_menu;
};

