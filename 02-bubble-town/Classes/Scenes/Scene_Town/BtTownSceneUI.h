/*!
 * \file BtTownSceneUI.h
 * \date 2015/02/13 17:00
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtTownSceneUI 
*/

#pragma once

class BtTownSceneUI 
{
public: 
    BtTownSceneUI();
    virtual ~BtTownSceneUI() {}

    bool init(cocos2d::Node* parent);
    void setUpgradeVisible(bool show);

private:
    void onButton_Build();
    void onButton_Upgrade();

    cocos2d::ui::Button* m_btBuild;
    cocos2d::ui::Button* m_btUpgrade;
};

