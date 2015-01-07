/*!
 * \file BtGuiUtil.cpp
 * \date 2014/12/23 9:57
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtGuiUtil
*/

#include "stdafx.h"
#include "BtGuiUtil.h"

BtTextMenuBuilder::BtTextMenuBuilder()
{
    m_itemAlignment = eItemAlign::Center;
    m_menuAlignment = eMenuAlign::Vertically;
}

void BtTextMenuBuilder::AddItems(BtConstStr items[], int itemCnt)
{
    if (!items)
        return;

    for (int i = 0; i < itemCnt; ++i)
    {
        AddItem(items[i]);
    }
}

cocos2d::Menu* BtTextMenuBuilder::Build()
{
    if (m_itemTexts.empty())
        return nullptr;

    if (!m_handler)
        return nullptr;

    cocos2d::Vector<cocos2d::MenuItem*> items;
    for (int i = 0; i < m_itemTexts.size(); ++i)
    {
        auto label = cocos2d::LabelTTF::create(m_itemTexts[i], "Arial", BtGui_DefaultMenuFontSize);
        auto item = cocos2d::MenuItemLabel::create(label);

        item->setCallback(m_handler);

        switch (m_itemAlignment)
        {
        case BtTextMenuBuilder::Left:
            item->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
            break;
        case BtTextMenuBuilder::Center:
            item->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
            break;
        case BtTextMenuBuilder::Right:
            item->setAnchorPoint(cocos2d::Vec2(1.0f, 0.5f));
            break;
        default:
            break;
        }
        items.pushBack(item);
    }

    auto menu = cocos2d::Menu::createWithArray(items);

    switch (m_menuAlignment)
    {
    case BtTextMenuBuilder::Horizontally:
        menu->alignItemsHorizontally();
        break;
    case BtTextMenuBuilder::Vertically:
        menu->alignItemsVertically();
        break;
    default:
        break;
    }
    return menu;
}


