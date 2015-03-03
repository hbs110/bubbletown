/*!
 * \file BtGui_utouch_NodeBuilder.cpp
 * \date 2015/03/02 11:36
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtGui_utouch_NodeBuilder
*/

#include "stdafx.h"
#include "BtGui_utouch_NodeBuilder.h"

#include "Core/BtCoreDef.h"
#include "Core/BtCoreUtil.h"

#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"

#include "BtGui_utouch_Util.h"


BtUINodeBuilder_utouch::BtUINodeBuilder_utouch(const cocos2d::Vec2& designRes, const cocos2d::Vec2& runtimeRes)
    : m_scaleRatio(BT_INVALID_FLOAT)
{
    if (designRes.x > 0 && designRes.y > 0 && runtimeRes.x > 0 && runtimeRes.y > 0)
    {
        float widthScale = runtimeRes.x / designRes.x;
        float heightScale = runtimeRes.y / designRes.y;
        m_scaleRatio = std::min(widthScale, heightScale);   // for safety we choose the smaller ratio
    }

    m_creators[BtUI_RootNode]  = [](){ return cocos2d::Node::create(); };
    m_creators[BtUI_Image] = [](){ return cocos2d::ui::ImageView::create(); };
    m_creators[BtUI_Button] = [](){ return cocos2d::ui::Button::create(); };

    using namespace std::placeholders;
    m_builders[BtUI_RootNode]  = std::bind(&BtUINodeBuilder_utouch::BuildNode, this, _1, _2, _3);
    m_builders[BtUI_Image] = std::bind(&BtUINodeBuilder_utouch::BuildImage, this, _1, _2, _3);
    m_builders[BtUI_Button] = std::bind(&BtUINodeBuilder_utouch::BuildButton, this, _1, _2, _3);
}

cocos2d::Node* BtUINodeBuilder_utouch::Create(const std::string& nodeType, const rapidjson::Value& layoutNode, const cocos2d::Rect& parentRect)
{
    BT_EXPECT_RET_V2(m_scaleRatio > 0, "trying to parse a node before a valid scale ratio is set.", false);

    auto it = m_creators.find(nodeType);
    BT_EXPECT_RET_V2(it != m_creators.end(), tfm::format("the *creator* of layout node type '%s' is unregistored.", nodeType), nullptr);

    auto node = it->second();
    BT_EXPECT_RET_V2(node, tfm::format("ui node creation failed using layout node type '%s'.", nodeType), nullptr);

    auto it_b = m_builders.find(nodeType);
    BT_EXPECT_RET_V2(it_b != m_builders.end(), tfm::format("the *builder* of layout node type '%s' is unregistored.", nodeType), nullptr);

    it_b->second(node, layoutNode, parentRect);
    return node;
}

cocos2d::Vec2 BtUINodeBuilder_utouch::GetScaled(const rapidjson::Value& desc, const char* prop)
{
    auto v = BtJsonValue::GetVec2Prop(desc, prop);
    if (!BtIsValid(v))
        return v;

    return cocos2d::Vec2(v.x * m_scaleRatio, v.y * m_scaleRatio);
}

bool BtUINodeBuilder_utouch::BuildNode(cocos2d::Node* destNode, const rapidjson::Value& desc, const cocos2d::Rect& parentRect)
{
    BT_EXPECT_RET_V2(destNode, "invalid cc node.", false);

    // name (optional)
    std::string name = BtJsonValue::GetStrProp(desc, "Name");
    if (name.size())
    {
        destNode->setName(name);
    }

    cocos2d::ui::Widget* widget = dynamic_cast<cocos2d::ui::Widget*>(destNode);
    if (widget)
        widget->ignoreContentAdaptWithSize(false);

    cocos2d::Vec2 pos = GetScaled(desc, "Position");
    cocos2d::Vec2 size = GetScaled(desc, "Size");
    BT_EXPECT_RET_V2(BtIsValid(pos) && BtIsValid(size), "property parsing failed.", false);

    destNode->setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f)); // use upper-left corner for alignment
    destNode->setContentSize(cocos2d::Size(size));
    destNode->setPosition(pos.x, parentRect.size.height - pos.y);

    return true;
}

bool BtUINodeBuilder_utouch::BuildImage(cocos2d::Node* destNode, const rapidjson::Value& desc, const cocos2d::Rect& parentRect)
{
    if (!BuildNode(destNode, desc, parentRect))
        return false;

    std::string tileName = BtJsonValue::GetResProp(desc, "Res");
    BT_EXPECT_RET_V2(tileName.size(), "resource of the image not found or not specified", false);

    cocos2d::ui::ImageView* imageView = dynamic_cast<cocos2d::ui::ImageView*>(destNode);
    BT_EXPECT_RET_V2(imageView, "node type is not ImageView when calling BuildImage()", false);
    imageView->loadTexture(tileName, cocos2d::ui::Widget::TextureResType::PLIST);

    BT_VERB("image node '%s' (res '%s') is created.", BtJsonValue::GetStrProp(desc, "Name"), tileName);
    return true;
}


bool BtUINodeBuilder_utouch::BuildButton(cocos2d::Node* destNode, const rapidjson::Value& desc, const cocos2d::Rect& parentRect)
{
    if (!BuildNode(destNode, desc, parentRect))
        return false;

    cocos2d::ui::Button* button = dynamic_cast<cocos2d::ui::Button*>(destNode);
    BT_EXPECT_RET_V2(button, "node type is not ImageView when calling BuildImage()", false);

    // button text
    std::string btText = BtJsonValue::GetResProp(desc, "ButtonText");
    if (btText.size())
    {
        button->setTitleText(btText);
    }

    // button handler
    button->addTouchEventListener(&BtUIStdHandlers::onButtonTouch);

    // button appearance
    std::string resNormal = BtJsonValue::GetResProp(desc, "Res_Normal");
    BT_EXPECT_RET_V2(resNormal.size(), "'Res_Normal' of the button not found or not specified", false);
    button->loadTextureNormal(resNormal, cocos2d::ui::Widget::TextureResType::PLIST);

    std::string resPressed = BtJsonValue::GetResProp(desc, "Res_Pressed");
    BT_EXPECT_RET_V2(resPressed.size(), "'Res_Pressed' of the button not found or not specified", false);
    button->loadTexturePressed(resPressed, cocos2d::ui::Widget::TextureResType::PLIST);

    BT_VERB("button node '%s' (text: '%s', resNormal: '%s', resPressed: '%s') is created.",
            BtJsonValue::GetStrProp(desc, "Name"),
            BtJsonValue::GetStrProp(desc, "ButtonText"),
            resNormal, resPressed);

    return true;
}

