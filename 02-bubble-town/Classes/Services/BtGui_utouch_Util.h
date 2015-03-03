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

BtConstStr BtUI_RootNode = "RootNode";
BtConstStr BtUI_Image = "Image";
BtConstStr BtUI_Button = "Button";

class BtJsonValue
{
public:
    static int GetIntProp(const rapidjson::Value& vm, const char* propName);
    static std::string GetStrProp(const rapidjson::Value& vm, const char* propName);

    static cocos2d::Vec2 GetVec2Prop(const rapidjson::Value& vm, const char* propName);
    static std::string GetResProp(const rapidjson::Value& vm, const char* propName);

    static bool IsRootNode(const rapidjson::Value& val);
};

class BtUIStdHandlers
{
public:
    static void onButtonTouch(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
