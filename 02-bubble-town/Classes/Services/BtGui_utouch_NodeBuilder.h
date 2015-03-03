/*!
 * \file BtGui_utouch_NodeBuilder.h
 * \date 2015/03/02 11:36
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtGui_utouch_NodeBuilder 
*/

#pragma once

#include "json/document.h"

using fnCreateNode = std::function < cocos2d::Node* () > ;
using fnBuildNode = std::function < bool(cocos2d::Node* destNode, const rapidjson::Value& desc, const cocos2d::Rect& parentRect) >;

using creatorMap_t = std::map < std::string, fnCreateNode > ;
using builderMap_t = std::map < std::string, fnBuildNode > ;

class BtUINodeBuilder_utouch
{
public:
    BtUINodeBuilder_utouch(const cocos2d::Vec2& designRes, const cocos2d::Vec2& runtimeRes);

    cocos2d::Node* Create(const std::string& nodeType, const rapidjson::Value& layoutNode, const cocos2d::Rect& parentRect);

private:
    float m_scaleRatio;

    creatorMap_t  m_creators;
    builderMap_t  m_builders;

    bool BuildNode(cocos2d::Node* destNode, const rapidjson::Value& desc, const cocos2d::Rect& parentRect);
    bool BuildImage(cocos2d::Node* destNode, const rapidjson::Value& desc, const cocos2d::Rect& parentRect);

    cocos2d::Vec2 GetScaled(const rapidjson::Value& desc, const char* prop);
};
