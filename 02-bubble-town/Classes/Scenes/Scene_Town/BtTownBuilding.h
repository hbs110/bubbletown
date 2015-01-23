/*!
 * \file BtTownBuilding.h
 * \date 2015/01/22 14:28
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtTownBuilding 
*/

#pragma once

const float BtBuilding_DefaultScale = 1.0f;
const float BtBuilding_DefaultAnimLength = 0.1f;

class BtTownBuilding : public cocos2d::Sprite
{
public: 
    static BtTownBuilding* create(const std::string& name, const std::string& resfile, int borderPixels = 0);
    virtual ~BtTownBuilding() {}

    bool initDeco();

    void select();
    void deselect();

    virtual void update(float delta);

protected:
    cocos2d::ui::Text* m_countDownLabel;
    float m_countdown;

    void playAnim_Selected();
    bool m_isSelected;

private:
    BtTownBuilding();
};

