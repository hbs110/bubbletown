/*!
 * \file TexturePoolTestScene.cpp
 * \date 2014/12/29 17:39
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TexturePoolTestScene
*/

#include "stdafx.h"
#include "TexturePoolTestScene.h"

#include "Core/BtGuiUtil.h"
#include "Scenes/BtSceneUtil.h"

BtConstStr MI_RenderImages      = "render images";
BtConstStr MI_RenderPooled      = "render images (pooled)";
BtConstStr MI_RenderDefragged   = "render images (defragged)";

static BtConstStr s_menuItems[] =   
{                           // (ÿһ�λ��ƣ���ע���ؼ� drawcall �Ĵ���)
    MI_RenderImages,        //  ���� 9 �����飬���λ�����Ļ��
    MI_RenderPooled,        //  ���� 9 �����飬�ŵ� pool �������Ļ��
    MI_RenderDefragged,     //  ���� 9 �����飬�ŵ� pool �����һ������أ�������Ļ��
};                         

bool TexturePoolTestScene::init()
{
    if ( !Layer::init() )
        return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = cocos2d::MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(TexturePoolTestScene::menuCloseCallback,this));

    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);

    BtMenuBuilder mb;
    mb.AddItems(s_menuItems, BT_ARRAY_SIZE(s_menuItems));
    mb.SetItemAlign(BtMenuBuilder::Left);
    mb.SetHandler(std::bind(&TexturePoolTestScene::OnMenuItem, this, std::placeholders::_1));
    cocos2d::Menu* menuBuild = mb.Build();
    if (menuBuild)
    {
        cocos2d::Vec2 menuPos;
        menuPos.x = origin.x + 20;
        menuPos.y = origin.y + visibleSize.height - 150;
        menuBuild->setPosition(menuPos);
        addChild(menuBuild, 1);
    }


    auto sprite = cocos2d::Sprite::create("__test_texture_pool__/1419872836_balloons.png");
    sprite->setPosition(cocos2d::Vec2(200, 200));
    addChild(sprite, -1);

    return true;
}

cocos2d::Scene* TexturePoolTestScene::scene()
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();

    // 'layer' is an autorelease object
    cocos2d::Layer *layer = TexturePoolTestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene                                           
    return scene;
}

void TexturePoolTestScene::menuCloseCallback(Ref* sender)
{
    BtMsgGotoScene_Emit(BTSCN_Start);
}

void TexturePoolTestScene::OnMenuItem(Ref* sender)
{

}
