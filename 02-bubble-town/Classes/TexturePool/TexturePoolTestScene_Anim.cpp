/*!
 * \file TexturePoolTestScene_Anim.cpp
 * \date 2015/01/04 16:52
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TexturePoolTestScene_Anim
*/

#include "stdafx.h"
#include "TexturePoolTestScene_Anim.h"

#include "Core/BtCoreDef.h"

#include "Scenes/BtSceneUtil.h"

#include "TexturePool/TexturePool.h"

static BtConstStr MI_Close      = "bt_close";


TexturePoolTestScene_Anim::~TexturePoolTestScene_Anim()
{
    BtDeletePointer(m_texturePool);
}

bool TexturePoolTestScene_Anim::init()
{
    if ( !Layer::init() )
        return false;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = cocos2d::MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(TexturePoolTestScene_Anim::OnMenuItem,this));
    closeItem->setName(MI_Close);
    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);

    m_texturePool = new TexturePool;
    m_texturePool->Init(GDefaultTexturePoolInitParams, TexPool_MaxGroupCount);
    if (!m_texturePool)
        return false;

    BtConstStr resources[] = {
        "__test_texture_pool__/1419872819_box1.png",
        "__test_texture_pool__/1419872828_box3.png",
        "__test_texture_pool__/1419872831_box2.png",
        "__test_texture_pool__/1419872836_balloons.png",
        "__test_texture_pool__/1419872854_dog.png",
        "__test_texture_pool__/1419872856_rings.png",
        "__test_texture_pool__/1419872859_bouquet.png",
        "__test_texture_pool__/1419872860_rose.png",
        "__test_texture_pool__/1419872876_cake.png",
    }; 

    for (int i = 0; i < BT_ARRAY_SIZE(resources); ++i)
    {
        BtConstStr res = resources[i];
        auto sprite = cocos2d::Sprite::create(res);
        sprite->setPosition(cocos2d::Vec2(50 + 80 * (i % 5), 100 + 80 * (i / 5)));
        addChild(sprite, -1);
        m_texturePool->PushSprite(sprite, "", res);
    }

    m_texturePool->Flush();

    return true;
}

cocos2d::Scene* TexturePoolTestScene_Anim::scene()
{
    // 'scene' is an autorelease object
    auto scene = cocos2d::Scene::create();

    // 'layer' is an autorelease object
    cocos2d::Layer *layer = TexturePoolTestScene_Anim::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene                                           
    return scene;
}

void TexturePoolTestScene_Anim::OnMenuItem(cocos2d::Ref* sender)
{
    auto image = dynamic_cast<cocos2d::MenuItemImage*>(sender);
    if (image && image->getName() == MI_Close)
    {
        BtMsgGotoScene_Emit(BTSCN_Start);
        return;
    }
}

