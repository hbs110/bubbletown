/*!
 * \file GameGui.h
 * \date 2014/07/25 16:40
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of GameGui 
*/

#pragma once

class GameGuiListener
{
public:
    virtual ~GameGuiListener() {}

    virtual void OnMenuItem(Ref* sender) = 0;
};

class GameGui 
{
public: 
    GameGui() : m_menu(nullptr) {}
    ~GameGui() {}

    bool init(GameGuiListener* listener);  

    Menu* getMenu() { return m_menu; }

private:
    Menu* m_menu;
};

