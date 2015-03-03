/*!
 * \file BtGui_utouch.h
 * \date 2015/02/27 10:14
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtGui_utouch 
*/

#pragma once

#include "json/document.h"

class BtUINodeBuilder_utouch;

class BtGui_utouch 
{
public: 
    static cocos2d::Node* LoadLayout(const std::string& layoutFile);

private:
    static cocos2d::Node* LoadLayoutRecursively(BtUINodeBuilder_utouch* builder, const rapidjson::Value& jsonNode, const cocos2d::Rect& parentRect);
};

