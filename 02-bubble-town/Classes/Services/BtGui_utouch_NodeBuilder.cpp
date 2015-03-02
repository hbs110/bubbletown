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

#include "BtGui_utouch_Util.h"

BtUINodeBuilder_utouch::BtUINodeBuilder_utouch(const cocos2d::Vec2& designRes, const cocos2d::Vec2& runtimeRes)
    : m_scaleRatio(BT_INVALID_FLOAT)
{
    if (designRes.x > 0 && designRes.y > 0 && runtimeRes.x > 0 && runtimeRes.y > 0)
    {
        float widthScale = designRes.x / runtimeRes.x;
        float heightScale = designRes.y / runtimeRes.y;
        m_scaleRatio = std::min(widthScale, heightScale);   // for safety we choose the smaller ratio
    }

    m_creators[BtStr_RootNode]  = [](){ return cocos2d::Node::create(); };
    m_creators[BtStr_ImageNode] = [](){ return cocos2d::ui::ImageView::create(); };

    using namespace std::placeholders;
    m_builders[BtStr_RootNode]  = std::bind(&BtUINodeBuilder_utouch::BuildRootNode, this, _1, _2);
    m_builders[BtStr_ImageNode] = std::bind(&BtUINodeBuilder_utouch::BuildImage, this, _1, _2);
}

cocos2d::Node* BtUINodeBuilder_utouch::Create(const std::string& nodeType, const rapidjson::Value& layoutNode)
{
    BT_EXPECT_RET_V2(m_scaleRatio > 0, "trying to parse a node before a valid scale ratio is set.", false);

    auto it = m_creators.find(nodeType);
    BT_EXPECT_RET_V2(it != m_creators.end(), tfm::format("the *creator* of layout node type '%s' is unregistored.", nodeType), nullptr);

    auto node = it->second();
    BT_EXPECT_RET_V2(node, tfm::format("ui node creation failed using layout node type '%s'.", nodeType), nullptr);

    auto it_b = m_builders.find(nodeType);
    BT_EXPECT_RET_V2(it_b != m_builders.end(), tfm::format("the *builder* of layout node type '%s' is unregistored.", nodeType), nullptr);

    it_b->second(node, layoutNode);
    return node;
}

bool BtUINodeBuilder_utouch::BuildNode(cocos2d::Node* destNode, const rapidjson::Value& desc)
{
    BT_EXPECT_RET_V2(destNode, "invalid cc node.", false);

    cocos2d::Vec2 pos = BtJsonValue::GetVec2Prop(desc, "Position");
    cocos2d::Vec2 size = BtJsonValue::GetVec2Prop(desc, "Size");
    BT_EXPECT_RET_V2(BtIsValid(pos) && BtIsValid(size), "property parsing failed.", false);


    return true;
}

bool BtUINodeBuilder_utouch::BuildRootNode(cocos2d::Node* destNode, const rapidjson::Value& desc)
{
    BT_LOG("root node '%s' is being created.", BtJsonValue::GetStrProp(desc, "Name"));
    return true;
}

bool BtUINodeBuilder_utouch::BuildImage(cocos2d::Node* destNode, const rapidjson::Value& desc)
{
    BT_LOG("image node '%s' is being created.", BtJsonValue::GetStrProp(desc, "Name"));
    return true;
}

