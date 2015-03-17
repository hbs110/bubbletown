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
    m_textSize = BtGui_DefaultMenuFontSize;
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

    cocos2d::Vector<cocos2d::MenuItem*> items;
    for (size_t i = 0; i < m_itemTexts.size(); ++i)
    {
        auto label = cocos2d::LabelTTF::create(m_itemTexts[i], BtGui_DefaultMenuFont, m_textSize);
        auto item = cocos2d::MenuItemLabel::create(label);

        auto it = m_namedHandlers.find(m_itemTexts[i]);
        if (it != m_namedHandlers.end())
        {
            if (it->second)
                item->setCallback(it->second);
        }
        else
        {
            if (m_handler)
                item->setCallback(m_handler);
        }

        if (!item)
            continue;

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

void BtTextMenuBuilder::AddItem(const std::string& itemText, cocos2d::ccMenuCallback handler)
{
    AddItem(itemText);
    m_namedHandlers[itemText] = handler;
}

void BtSetButtonHandler(cocos2d::ui::Button* button, buttonHandler_t handler, bool onReleased /*= false*/)
{
    if (!button)
        return;

    auto callback = [onReleased, handler] (cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
    { 
        if ((!onReleased && type == cocos2d::ui::Widget::TouchEventType::BEGAN) ||
            (onReleased && type == cocos2d::ui::Widget::TouchEventType::ENDED))
        {
            handler();
        }
    };

    button->addTouchEventListener(std::bind(callback, std::placeholders::_1, std::placeholders::_2));
}

