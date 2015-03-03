/*!
 * \file BtGui_utouch_Util.cpp
 * \date 2015/03/02 11:37
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtGui_utouch_Util
*/

#include "stdafx.h"
#include "BtGui_utouch_Util.h"

#include "Core/BtCoreDef.h"
#include "Core/BtCoreUtil.h"

#include "Core/BtMsgDef.h"
#include "Core/BtMsgDispatcher.h"

int BtJsonValue::GetIntProp(const rapidjson::Value& vm, const char* propName)
{
    if (!vm.HasMember(propName))
        return BT_INVALID_ID;

    const auto& val = vm[propName];
    if (!val.IsInt())
        return BT_INVALID_ID;

    return val.IsInt();
}

std::string BtJsonValue::GetStrProp(const rapidjson::Value& vm, const char* propName)
{
    if (!vm.HasMember(propName))
        return "";

    const auto& val = vm[propName];
    if (!val.IsString())
        return "";

    return val.GetString();
}

cocos2d::Vec2 BtJsonValue::GetVec2Prop(const rapidjson::Value& vm, const char* propName)
{
    std::string val = GetStrProp(vm, propName);
    BT_EXPECT_RET(val.size(), tfm::format("property (%s) not found.", propName), BT_INVALID_VEC2);

    cocos2d::Vec2 v;
    int cnt = sscanf_s(val.c_str(), "%f, %f", &v.x, &v.y);
    BT_EXPECT_RET(cnt == 2, tfm::format("bad format for property (%s).", propName), BT_INVALID_VEC2);

    return v;
}

std::string BtJsonValue::GetResProp(const rapidjson::Value& vm, const char* propName)
{
    std::string val = GetStrProp(vm, propName);
    BT_EXPECT_RET(val.size(), tfm::format("property (%s) not found.", propName), "");

    size_t pos = val.rfind(":"); 
    BT_EXPECT_RET(pos != std::string::npos, tfm::format("the value (%s) of property (%s) is invalid.", val, propName), "");

    std::string ret = val.substr(pos + 1);
    return ret;
}

bool BtJsonValue::IsRootNode(const rapidjson::Value& val)
{
    std::string typeInfo = BtJsonValue::GetStrProp(val, "__type_info__");
    return typeInfo == BtUI_RootNode;
}

void BtUIStdHandlers::onButtonTouch(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    cocos2d::ui::Button* button = dynamic_cast<cocos2d::ui::Button*>(pSender);
    BT_EXPECT_RET(button, "a non-button control emits a button event, ignored.", ;);

    if (type == cocos2d::ui::Widget::TouchEventType::BEGAN)
    {
        BtMsgDispatcher::Get()->notify(BtMsg((int) BtMsgID::UI_ButtonPressed, button->getName()));
    }
    else if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        BtMsgDispatcher::Get()->notify(BtMsg((int) BtMsgID::UI_ButtonReleased, button->getName()));
    }
}
