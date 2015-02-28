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

int BtValueMapUtil::GetPropertyAsInt(const rapidjson::Value& vm, const char* propName)
{
    if (!vm.HasMember(propName))
        return BT_INVALID_ID;

    const auto& val = vm[propName];
    if (!val.IsInt())
        return BT_INVALID_ID;

    return val.IsInt();
}

std::string BtValueMapUtil::GetPropertyAsString(const rapidjson::Value& vm, const char* propName)
{
    if (!vm.HasMember(propName))
        return "";

    const auto& val = vm[propName];
    if (!val.IsString())
        return "";

    return val.GetString();
}

using fnCreateNode = std::function < cocos2d::Node* () > ;
using fnBuildNode = std::function < void(cocos2d::Node* destNode, const rapidjson::Value& desc) > ;

using creatorMap_t = std::map < std::string, fnCreateNode > ;
using builderMap_t = std::map < std::string, fnBuildNode > ;

creatorMap_t  s_uiNodeCreators{
    { "RootNode", [](){ return cocos2d::Node::create(); } },
    { "Image", [](){ return cocos2d::ui::ImageView::create(); } },
};

builderMap_t  s_uiNodeBuilders{
    { "RootNode", BtUINodeBuilder::BuildRootNode },
    { "Image", BtUINodeBuilder::BuildImage },
};

cocos2d::Node* BtUINodeBuilder::BuildNode(const rapidjson::Value& layoutNode)
{
    std::string ctrlType = BtValueMapUtil::GetPropertyAsString(layoutNode, "__type_info__");
    BT_EXPECT_RET_VAL(ctrlType.size(), "layout node type not specified.", BT_DUMMY_FUNC, nullptr);

    auto it = s_uiNodeCreators.find(ctrlType);
    BT_EXPECT_RET_VAL(it != s_uiNodeCreators.end(), tfm::format("the *creator* of layout node type '%s' is unregistored.", ctrlType), BT_DUMMY_FUNC, nullptr);

    auto node = it->second();
    BT_EXPECT_RET_VAL(node, tfm::format("ui node creation failed using layout node type '%s'.", ctrlType), BT_DUMMY_FUNC, nullptr);

    auto it_b = s_uiNodeBuilders.find(ctrlType);
    BT_EXPECT_RET_VAL(it_b != s_uiNodeBuilders.end(), tfm::format("the *builder* of layout node type '%s' is unregistored.", ctrlType), BT_DUMMY_FUNC, nullptr);

    it_b->second(node, layoutNode);
    return node;
}

void BtUINodeBuilder::BuildRootNode(cocos2d::Node* destNode, const rapidjson::Value& desc)
{
    BT_LOG("root node '%s' is being created.", BtValueMapUtil::GetPropertyAsString(desc, "Name"));
}

void BtUINodeBuilder::BuildImage(cocos2d::Node* destNode, const rapidjson::Value& desc)
{
    BT_LOG("image node '%s' is being created.", BtValueMapUtil::GetPropertyAsString(desc, "Name"));
}

cocos2d::Node* BtGui_utouch::LoadLayout(const std::string& layoutFile)
{
    std::string atlasFile = layoutFile + ".plist";
    std::string atlasTexture = layoutFile + ".png";
    std::string layoutFilename = layoutFile + ".ui_layout";

    auto sfc = cocos2d::SpriteFrameCache::getInstance();
    BT_EXPECT_RET_V2(sfc, tfm::format("cocos2d::SpriteFrameCache not available. (while loading '%s')", layoutFile), nullptr);
    sfc->addSpriteFramesWithFile(atlasFile, atlasTexture);

    std::string layoutContent = cocos2d::FileUtils::getInstance()->getStringFromFile(layoutFilename);
    BT_EXPECT_RET_V2(layoutContent.size(), tfm::format("layout file reading failed. ('%s')", layoutFilename), nullptr);

    rapidjson::Document doc;
    doc.Parse<0>(layoutContent.c_str());
    BT_EXPECT_RET_V2(doc.IsObject(), tfm::format("layout file parsing (as a json doc) failed. ('%s')", layoutFilename), nullptr);

    auto node = LoadLayoutRecursively(doc);
    BT_EXPECT_RET_V2(node, tfm::format("layout file parsing (recursively) failed. ('%s')", layoutFilename), nullptr);

    return node;
}

cocos2d::Node* BtGui_utouch::LoadLayoutRecursively(const rapidjson::Value& jsonNode)
{
    cocos2d::Node* node = BtUINodeBuilder::BuildNode(jsonNode);
    if (!node)
    {
        BT_ERROR("node '%s' loading failed.", BtValueMapUtil::GetPropertyAsString(jsonNode, "Name"));
        return nullptr;
    }

    auto& children = jsonNode["Children"];
    if (children.IsArray())
    {
        for (auto it = children.onBegin(); it != children.onEnd(); ++it)
        {
            if (it->IsObject())
            {
                const rapidjson::Value& name = (*it)["Name"];
                BT_VERB("reading layout element '%s'...", name.IsString() ? name.GetString() : "<null>" );
                
                auto subNode = LoadLayoutRecursively(*it);
                if (subNode)
                {
                    node->addChild(subNode);
                }
            }
        }
    }

    return node;
}


