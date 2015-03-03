/*!
 * \file BtGui_utouch.cpp
 * \date 2015/02/27 10:14
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtGui_utouch
*/

#include "stdafx.h"
#include "BtGui_utouch.h"

#include "Core/BtCoreDef.h"
#include "Core/BtCoreUtil.h"

#include "json/document.h"

#include "BtGui_utouch_Util.h"
#include "BtGui_utouch_NodeBuilder.h"

cocos2d::Node* BtGui_utouch::LoadLayout(const std::string& layoutFile)
{
    std::string atlasFile = layoutFile + ".plist";
    std::string atlasTexture = layoutFile + ".png";
    std::string layoutFilename = layoutFile + ".ui_layout";

    auto sfc = cocos2d::SpriteFrameCache::getInstance();
    BT_EXPECT_RET(sfc, tfm::format("cocos2d::SpriteFrameCache not available. (while loading '%s')", layoutFile), nullptr);
    sfc->addSpriteFramesWithFile(atlasFile, atlasTexture);

    std::string layoutContent = cocos2d::FileUtils::getInstance()->getStringFromFile(layoutFilename);
    BT_EXPECT_RET(layoutContent.size(), tfm::format("layout file reading failed. ('%s')", layoutFilename), nullptr);

    rapidjson::Document doc;
    doc.Parse<0>(layoutContent.c_str());
    BT_EXPECT_RET(doc.IsObject(), tfm::format("layout file parsing (as a json doc) failed. ('%s')", layoutFilename), nullptr);

    BT_EXPECT_RET(BtJsonValue::IsRootNode(doc), "root node not found.", nullptr);
    cocos2d::Vec2 designRes = BtJsonValue::GetVec2Prop(doc, "DesignTimeResolution");
    BT_EXPECT_RET(BtIsValid(designRes) && !designRes.isZero(), "design time resolution is not set.", false);

    cocos2d::Rect runtimeRect;
    runtimeRect.origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    runtimeRect.size = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize());
    BtUINodeBuilder_utouch builder(designRes, runtimeRect.size);

    auto node = LoadLayoutRecursively(&builder, doc, runtimeRect);
    BT_EXPECT_RET(node, tfm::format("layout file parsing (recursively) failed. ('%s')", layoutFilename), nullptr);
    
    node->setAnchorPoint(cocos2d::Vec2::ZERO);
    node->setPosition(runtimeRect.origin);
    return node;
}

cocos2d::Node* BtGui_utouch::LoadLayoutRecursively(BtUINodeBuilder_utouch* builder, const rapidjson::Value& jsonNode, const cocos2d::Rect& parentRect)
{
    std::string ctrlType = BtJsonValue::GetStrProp(jsonNode, "__type_info__");
    BT_EXPECT_RET(ctrlType.size(), "layout node type not specified.", nullptr);

    auto node = builder->Create(ctrlType, jsonNode, parentRect);
    BT_EXPECT_RET(node, tfm::format("ui node '%s' creation failed using layout node type '%s'.", BtJsonValue::GetStrProp(jsonNode, "Name"), ctrlType), nullptr);

    cocos2d::Rect currentRect;
    currentRect.origin = parentRect.origin + node->getPosition();
    currentRect.size = node->getContentSize();

    auto& children = jsonNode["Children"];
    if (children.IsArray())
    {
        for (auto it = children.onBegin(); it != children.onEnd(); ++it)
        {
            if (it->IsObject())
            {
                const rapidjson::Value& name = (*it)["Name"];
                BT_VERB("reading layout element '%s'...", name.IsString() ? name.GetString() : "<null>");

                auto subNode = LoadLayoutRecursively(builder, *it, currentRect);
                if (subNode)
                {
                    BT_VERB("attaching element '%s'...", name.IsString() ? name.GetString() : "<null>");
                    node->addChild(subNode);
                }
            }
        }
    }

    return node;
}


