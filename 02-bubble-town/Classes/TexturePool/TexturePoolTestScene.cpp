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

#include "TexturePool/TexturePool.h"

static BtConstStr MI_Close      = "bt_close";

static BtConstStr MI_RenderImages      = "render images";
static BtConstStr MI_RenderPooled      = "render images (pooled)";
static BtConstStr MI_RenderDefragged   = "render images (defragged)";

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

    auto menuDefault = BtCreateDefaultUIElements(std::bind(&BtStdHandler_BackToMainMenu, std::placeholders::_1), BTSCN_TexturePool);
    if (menuDefault)
        addChild(menuDefault, 1);

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    //BtMenuBuilder mb;
    //mb.AddItems(s_menuItems, BT_ARRAY_SIZE(s_menuItems));
    //mb.SetItemAlign(BtMenuBuilder::Left);
    //mb.SetHandler(std::bind(&TexturePoolTestScene::OnMenuItem, this, std::placeholders::_1));
    //cocos2d::Menu* menuBuild = mb.Build();
    //if (menuBuild)
    //{
    //    cocos2d::Vec2 menuPos;
    //    menuPos.x = origin.x + 20;
    //    menuPos.y = origin.y + visibleSize.height - 150;
    //    menuBuild->setPosition(menuPos);
    //    addChild(menuBuild, 1);
    //}

    //_target = cocos2d::RenderTexture::create(100, 100, cocos2d::Texture2D::PixelFormat::RGBA8888);
    //_target->retain();
    //_target->setPosition(cocos2d::Vec2(250, 100));
    //_target->clear(0.0f, 0.0f, 0.0f, 0.0f);
    //addChild(_target, -1);

    //_target->begin();
    //sprite->visit();
    //_target->end();

    m_texturePool = new TexturePool;
    m_texturePool->Init(GDefaultTexturePoolInitParams, TexPool_MaxGroupCount);
    if (!m_texturePool)
        return false;

    BtConstStr resources[] = {
        "test_texture_pool/1419872819_box1.png",
        "test_texture_pool/1419872828_box3.png",
        "test_texture_pool/1419872831_box2.png",
        "test_texture_pool/1419872836_balloons.png",
        "test_texture_pool/1419872854_dog.png",
        "test_texture_pool/1419872856_rings.png",
        "test_texture_pool/1419872859_bouquet.png",
        "test_texture_pool/1419872860_rose.png",
        "test_texture_pool/1419872876_cake.png",
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

void TexturePoolTestScene::OnMenuItem(Ref* sender)
{
}

TexturePoolTestScene::~TexturePoolTestScene()
{
    //if (_target)
    //{
    //    _target->release();
    //    cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();
    //}

    BtDeletePointer(m_texturePool);
}
