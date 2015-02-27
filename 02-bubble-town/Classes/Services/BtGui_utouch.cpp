/*!
 * \file BtGui_utouch.cpp
 * \date 2015/02/27 10:14
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtGui_utouch
*/

#include "stdafx.h"
#include "BtGui_utouch.h"

#include "json/document.h"

#include "Core/BtCoreDef.h"
#include "Core/BtCoreUtil.h"

int BtValueMapUtil::GetPropertyAsInt(const cocos2d::ValueMap& vm, const std::string& propName)
{
    auto it = vm.find(propName);
    if (it == vm.end())
    {
        return BT_INVALID_ID;
    }

    return it->second.asInt();
}

std::string BtValueMapUtil::GetPropertyAsString(const cocos2d::ValueMap& vm, const std::string& propName)
{
    auto it = vm.find(propName);
    if (it == vm.end())
    {
        return "";
    }

    return it->second.asString();
}

creatorMap_t  BtUINodeBuilder::s_creators{ 
    { "RootNode", [](){ return cocos2d::Node::create(); } },
    { "Image", [](){ return cocos2d::ui::ImageView::create(); } },
};

cocos2d::Node* BtUINodeBuilder::BuildNode(const cocos2d::ValueMap& layoutNode)
{
    std::string ctrlType = BtValueMapUtil::GetPropertyAsString(layoutNode, "__type_info__");
    BT_EXPECT_RET_VAL(ctrlType.empty(), "layout node type not specified.", BT_DUMMY_FUNC, nullptr);

    auto it = s_creators.find(ctrlType);
    BT_EXPECT_RET_VAL(it != s_creators.end(), tfm::format("layout node type '%s' unrecoganized.", ctrlType), BT_DUMMY_FUNC, nullptr);

    auto node = it->second();
    return node;
}


std::string BtGui_utouch::LoadAtlas(const char* atlasLutFile, const char* atlasTextureFile)
{
    //cocos2d::SpriteFrame* sf = cocos2d::SpriteFrame::create(atlasTextureFile);

    //std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(atlasLutFile);
    //BT_EXPECT_RET_VAL(str.size(), tfm::format("Atlas lookup-table loading failed. ('%s')", atlasLutFile), BT_DUMMY_FUNC, atlasLut());

    //cocos2d::ValueMap dict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(atlasLutFile);

    //rapidjson::Document doc;
    //doc.Parse<0>(str.c_str());
    //BT_EXPECT_RET_VAL(doc.IsObject(), tfm::format("invalid atlas file content. ('%s')", atlasLutFile), BT_DUMMY_FUNC, atlasLut());
    //
    //auto& frames = doc["frames"];
    //BT_EXPECT_RET_VAL(frames.IsObject(), tfm::format("'frames' node not found in atlas file. ('%s')", atlasLutFile), BT_DUMMY_FUNC, atlasLut());

    //for (auto it = frames.MemberonBegin(); it != frames.MemberonEnd(); ++it)
    //{
    //    BT_VERB("reading atlas element '%s'...", (*it).name.GetString());
    //    if (it->name.IsString() && it->value.IsObject())
    //    {
    //        auto& frame = it->value["frame"];
    //        if (frame.IsObject())
    //        {

    //        }
    //    }
    //}

    return atlasLutFile;
}

cocos2d::Node* BtGui_utouch::LoadLayout(const std::string& layoutFile)
{
    std::string atlasFile = layoutFile + ".plist";
    std::string atlasTexture = layoutFile + ".png";
    std::string layoutFilename = layoutFile + ".ui_layout";

    auto sfc = cocos2d::SpriteFrameCache::getInstance();
    BT_EXPECT_RET_VAL(sfc, tfm::format("cocos2d::SpriteFrameCache not available. (while loading '%s')", layoutFile), BT_DUMMY_FUNC, nullptr);
    sfc->addSpriteFramesWithFile(atlasFile, atlasTexture);

    cocos2d::ValueMap layoutDict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(layoutFilename);
    BT_EXPECT_RET_VAL(layoutDict.size(), tfm::format("invalid layout file. ('%s')", layoutFilename), BT_DUMMY_FUNC, nullptr);
    return LoadLayoutRecursively(layoutDict);
}

cocos2d::Node* BtGui_utouch::LoadLayoutRecursively(const cocos2d::ValueMap& layoutNode)
{
    cocos2d::Node* node = BtUINodeBuilder::BuildNode(layoutNode);
    if (!node)
    {
        BT_ERROR("node '%s' loading failed.", BtValueMapUtil::GetPropertyAsString(layoutNode, "Name").c_str());
        return nullptr;
    }

    auto it = layoutNode.find("Children");
    if (it != layoutNode.end())
    {
        const auto& children = it->second.asValueVector();
        for (const auto& child : children)
        {
            auto subNode = LoadLayoutRecursively(child.asValueMap());
            if (subNode)
            {
                node->addChild(subNode);
            }
        }
    }
    return node;
}


