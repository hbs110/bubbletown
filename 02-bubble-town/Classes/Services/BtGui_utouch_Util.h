/*!
 * \file BtGui_utouch_Util.h
 * \date 2015/03/02 11:37
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtGui_utouch_Util 
*/

#pragma once

#include "json/document.h"

#include "Core/BtCoreDef.h"

BtConstStr BtStr_RootNode = "RootNode";
BtConstStr BtStr_ImageNode = "Image";

class BtJsonValue
{
public:
    static int GetIntProp(const rapidjson::Value& vm, const char* propName);
    static std::string GetStrProp(const rapidjson::Value& vm, const char* propName);
    static cocos2d::Vec2 GetVec2Prop(const rapidjson::Value& vm, const char* propName);

    static bool IsRootNode(const rapidjson::Value& val);
};
