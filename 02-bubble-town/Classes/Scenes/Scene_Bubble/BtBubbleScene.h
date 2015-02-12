/*!
 * \file BtBubbleScene.h
 * \date 2015/01/21 9:26
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtBubbleScene 
*/

#pragma once

#include "Core/BtMsgDef.h"
#include "Scenes/BtBaseScene.h"

class BtBubbleScene : public BtBaseScene
{
public:
    BtBubbleScene();
    virtual ~BtBubbleScene() {}
    BT_DEF_SCENE(BtBubbleScene, BTSCN_bubble);

protected:
    virtual bool do_init();  
    virtual void do_enter();
    virtual void do_exit();

    void onButton_Loot();
    void onButton_Restart();
    void onButton_Next();

    void showEndScreen(bool show);

    cocos2d::ui::Button* m_btLoot;
    cocos2d::ui::Button* m_btRestart;
    cocos2d::ui::Button* m_btNext;
};

