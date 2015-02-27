/*!
 * \file BtGui_utouch.h
 * \date 2015/02/27 10:14
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtGui_utouch 
*/

#pragma once

//using atlasLut = std::map < std::string, cocos2d::SpriteFrame* >;

class BtValueMapUtil
{
public:
    static int GetPropertyAsInt(const cocos2d::ValueMap& vm, const std::string& propName);
    static std::string GetPropertyAsString(const cocos2d::ValueMap& vm, const std::string& propName);
};

using fnCreateNode = std::function < cocos2d::Node* () > ;
using creatorMap_t = std::map < std::string, fnCreateNode > ;

class BtUINodeBuilder
{
public:
    static cocos2d::Node* BuildNode(const cocos2d::ValueMap& layoutNode);

    static creatorMap_t s_creators;
};

class BtGui_utouch 
{
public: 
    static std::string LoadAtlas(const char* atlasLutFile, const char* atlasTextureFile);
    static cocos2d::Node* LoadLayout(const std::string& layoutFile);

private:
    static cocos2d::Node* LoadLayoutRecursively(const cocos2d::ValueMap& layoutNode);
};

