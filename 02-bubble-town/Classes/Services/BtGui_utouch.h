/*!
 * \file BtGui_utouch.h
 * \date 2015/02/27 10:14
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtGui_utouch 
*/

#pragma once

#include "json/document.h"

class BtValueMapUtil
{
public:
    static int GetPropertyAsInt(const rapidjson::Value& vm, const char* propName);
    static std::string GetPropertyAsString(const rapidjson::Value& vm, const char* propName);
};

class BtUINodeBuilder
{
public:
    static cocos2d::Node* BuildNode(const rapidjson::Value& layoutNode);

    static void BuildRootNode(cocos2d::Node* destNode, const rapidjson::Value& desc);
    static void BuildImage(cocos2d::Node* destNode, const rapidjson::Value& desc);
};

class BtGui_utouch 
{
public: 
    static cocos2d::Node* LoadLayout(const std::string& layoutFile);

private:
    static cocos2d::Node* LoadLayoutRecursively(const rapidjson::Value& jsonNode);
};

