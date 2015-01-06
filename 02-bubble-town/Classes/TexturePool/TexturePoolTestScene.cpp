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
{                           // (每一次绘制，都注明关键 drawcall 的次数)
    MI_RenderImages,        //  生成 9 个精灵，依次画在屏幕上
    MI_RenderPooled,        //  生成 9 个精灵，放到 pool 里，画在屏幕上
    MI_RenderDefragged,     //  生成 9 个精灵，放到 pool 里，整理一下纹理池，画在屏幕上
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
        CC_CALLBACK_1(TexturePoolTestScene::OnMenuItem,this));
    closeItem->setName(MI_Close);
    closeItem->setPosition(origin + cocos2d::Vec2(visibleSize) - cocos2d::Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = cocos2d::Menu::create(closeItem, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);

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

void TexturePoolTestScene::OnMenuItem(Ref* sender)
{
    auto image = dynamic_cast<cocos2d::MenuItemImage*>(sender);
    if (image && image->getName() == MI_Close)
    {
        BtMsgGotoScene_Emit(BTSCN_Start);
        return;
    }
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
