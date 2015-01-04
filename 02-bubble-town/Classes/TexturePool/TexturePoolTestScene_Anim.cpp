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
#include "Core/BtGuiUtil.h"

#include "Scenes/BtSceneUtil.h"

#include "TexturePool/TexturePool.h"

static BtConstStr MI_Close      = "bt_close";

static BtConstStr BtRes_TestImagePath = "__test_texture_pool__/map/";

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


    //BtTextMenuBuilder mb;
    //mb.AddItem("+");
    //mb.AddItem("-");
    //mb.SetItemAlign(BtTextMenuBuilder::Left);
    //mb.SetHandler(std::bind(&TexturePoolTestScene_Anim::OnMenuItem, this, std::placeholders::_1));
    //cocos2d::Menu* menuBuild = mb.Build();
    //if (menuBuild)
    //{
    //    cocos2d::Vec2 menuPos;
    //    menuPos.x = origin.x + 50;
    //    menuPos.y = origin.y + visibleSize.height - 100;
    //    menuBuild->setPosition(menuPos);

    //    addChild(menuBuild, 1);
    //}

    initArmatrueName();

    m_texturePool = new TexturePool;
    m_texturePool->Init(GDefaultTexturePoolInitParams, TexPool_MaxGroupCount);
    if (!m_texturePool)
        return false;

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

    auto text = dynamic_cast<cocos2d::MenuItemLabel*>(sender);
    if (text)
    {
        auto label = dynamic_cast<cocos2d::LabelProtocol*>(text->getLabel());
        if (label)
        {
            if (label->getString() == "+")
            {
                if( m_vArmature.size() < m_vArmatrueName.size() ) 
                {
                    std::string strArmatrueName = m_vArmatrueName[m_vArmature.size()];

                    std::string sPng = BtRes_TestImagePath + strArmatrueName + ".png";
                    std::string sPlist = BtRes_TestImagePath + strArmatrueName + ".plist";
                    std::string sXml = BtRes_TestImagePath + strArmatrueName + ".xml";

                    cocostudio::DataReaderHelper::setPositionReadScale(0.25f);
                    cocostudio::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo( sPng.c_str(), sPlist.c_str(), sXml.c_str() );

                    cocostudio::Armature *pArmature = cocostudio::Armature::create();
                    pArmature->init(strArmatrueName.c_str());
                    pArmature->setScale(2.0f);
                    pArmature->getAnimation()->setAnimationScale(0.2f);
                    pArmature->getAnimation()->playByIndex(1, -1, -1/*, 1, 10000*/);
                    pArmature->setPosition(rand() % 300, rand() % 300);
                    this->addChild(pArmature);

                    m_vArmature.push_back(pArmature);
                }
            }
            else if (label->getString() == "-")
            {
                if( !m_vArmature.empty() ) 
                {
                    cocostudio::Armature *pCCArmature = m_vArmature[m_vArmature.size() - 1];
                    m_vArmature.pop_back();
                    pCCArmature->removeFromParentAndCleanup(true);
                }

            }
        }
    }
}

void TexturePoolTestScene_Anim::initArmatrueName()
{
    m_vArmatrueName.push_back("DefenderN");
    m_vArmatrueName.push_back("SpearmenN");
    m_vArmatrueName.push_back("ArcherN");
    m_vArmatrueName.push_back("ShieldMaster");
    m_vArmatrueName.push_back("Defender");
    m_vArmatrueName.push_back("Archer");
    m_vArmatrueName.push_back("Spearmen");
    m_vArmatrueName.push_back("Witch");
    m_vArmatrueName.push_back("Catapult");
    m_vArmatrueName.push_back("Artillery");
    m_vArmatrueName.push_back("Cavalry");
    m_vArmatrueName.push_back("ElephantSoldier");
    m_vArmatrueName.push_back("Hoplite");
    m_vArmatrueName.push_back("Priest");
    m_vArmatrueName.push_back("ArmorCavalry");
    m_vArmatrueName.push_back("Assasin");
    m_vArmatrueName.push_back("Berserker");
    m_vArmatrueName.push_back("IceWitch");
    m_vArmatrueName.push_back("Pastor");
    m_vArmatrueName.push_back("WitchDoctor");
    m_vArmatrueName.push_back("CaoZhi");
    m_vArmatrueName.push_back("DiaoChan");
    m_vArmatrueName.push_back("DongZhuo");
    m_vArmatrueName.push_back("DongZhuoEvil");
    m_vArmatrueName.push_back("FaZheng");
    m_vArmatrueName.push_back("GongSunZan");
    m_vArmatrueName.push_back("GuYong");
    m_vArmatrueName.push_back("GuanYu");
    m_vArmatrueName.push_back("HuaXiong");
    m_vArmatrueName.push_back("HuangYueYing");
    m_vArmatrueName.push_back("JiangWei");
    m_vArmatrueName.push_back("LingTong");
    m_vArmatrueName.push_back("LiuBei");
    m_vArmatrueName.push_back("LuSu");
    m_vArmatrueName.push_back("LuXun");
    m_vArmatrueName.push_back("LvBu");
    m_vArmatrueName.push_back("LvBuEvil");
    m_vArmatrueName.push_back("MaDai");
    m_vArmatrueName.push_back("PanFeng");
    m_vArmatrueName.push_back("PoJun");
    m_vArmatrueName.push_back("SunCe");
    m_vArmatrueName.push_back("SunJian");
    m_vArmatrueName.push_back("SunLuBan");
    m_vArmatrueName.push_back("XiaoQiao");
    m_vArmatrueName.push_back("XinXianYing");
    m_vArmatrueName.push_back("XuHuang");
    m_vArmatrueName.push_back("XuShu");
    m_vArmatrueName.push_back("XuHuang");
    m_vArmatrueName.push_back("YanLiangWenChou");
    m_vArmatrueName.push_back("YuJin");
    m_vArmatrueName.push_back("YuanShao");
    m_vArmatrueName.push_back("YuanShu");
    m_vArmatrueName.push_back("ZhangFei");
    m_vArmatrueName.push_back("ZhangJiao");
    m_vArmatrueName.push_back("ZhangJiaoEvil");
    m_vArmatrueName.push_back("PangDe");
    m_vArmatrueName.push_back("ZhaoYun");
    m_vArmatrueName.push_back("ZhouCang");
    m_vArmatrueName.push_back("ZuoCi");
    m_vArmatrueName.push_back("HellDog");
    m_vArmatrueName.push_back("MaYuanYi");
    m_vArmatrueName.push_back("PengTuo");
    m_vArmatrueName.push_back("Ooze");
    m_vArmatrueName.push_back("TenAttendants");
    m_vArmatrueName.push_back("Ghoul");
    m_vArmatrueName.push_back("Butcher");
    m_vArmatrueName.push_back("ZhangLiangEvil");
    m_vArmatrueName.push_back("ZhangLiangBao");
    m_vArmatrueName.push_back("ZhangManCheng");
    m_vArmatrueName.push_back("ThunderDragon");
    m_vArmatrueName.push_back("TaoTie");

    for (auto strArmatrueName : m_vArmatrueName)
    {
        std::string sPng = BtRes_TestImagePath + strArmatrueName + ".png";
        std::string sPlist = BtRes_TestImagePath + strArmatrueName + ".plist";
        std::string sXml = BtRes_TestImagePath + strArmatrueName + ".xml";

        cocostudio::DataReaderHelper::setPositionReadScale(0.25f);
        cocostudio::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo( sPng.c_str(), sPlist.c_str(), sXml.c_str() );

        cocostudio::Armature *pArmature = cocostudio::Armature::create();
        pArmature->init(strArmatrueName.c_str());
        pArmature->setScale(2.0f);
        pArmature->getAnimation()->setAnimationScale(0.2f);
        pArmature->getAnimation()->playByIndex(1, -1, -1/*, 1, 10000*/);
        pArmature->setPosition(rand() % 300, rand() % 300);
        this->addChild(pArmature);

        m_vArmature.push_back(pArmature);
    }
}

