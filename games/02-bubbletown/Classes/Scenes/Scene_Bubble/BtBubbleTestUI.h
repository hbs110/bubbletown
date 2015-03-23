/*!
 * \file BtBubbleTestUI.h
 * \date 2015/03/23 9:27
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtBubbleTestUI 
*/

#pragma once

class BtBubbleTestUI 
{
public: 
    BtBubbleTestUI();
    ~BtBubbleTestUI() {}

    bool init(cocos2d::Layer* uiRoot);
    
    void showEndScreen(bool show);

    cocos2d::ui::Button* m_btLoot;
    cocos2d::ui::Button* m_btRestart;
    cocos2d::ui::Button* m_btNext;
    cocos2d::ui::Button* m_btBackToTown;

    cocos2d::ui::Text* m_labelPlaying;
    cocos2d::ui::Text* m_labelEnd;
};

