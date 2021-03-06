 /*!
  * \file BtCoreDef.h
  * \date 2014/12/23 10:04
  * \author Gu Lu (gulu@kingsoft.com)
  *
  * \brief Definition of BtCoreDef 
 */
 
 #pragma once

#include "cocos2d.h"

#include <vector>
#include <string>

// C-style string constant
typedef const char* const   BtConstStr;

// flags
typedef unsigned int btflags;

// flag operations
template <typename T> inline bool BtHasFlag(btflags flags, T targetFlags) { return (flags & (btflags) targetFlags) != 0; }
template <typename T> inline void BtMarkFlag(btflags& flags, T targetFlags) { flags |= (btflags) targetFlags; }
template <typename T> inline void BtClearFlag(btflags& flags, T targetFlags) { flags &= ~((btflags) targetFlags); }

// array size
#define BT_ARRAY_SIZE(arr)  ( sizeof(arr) / sizeof(arr[0]) )

// invalid id
const int BT_INVALID_ID = -1;

// invalid float
const float BT_INVALID_FLOAT = -100007.0f;

// invalid id
const cocos2d::Vec2 BT_INVALID_VEC2 = cocos2d::Vec2(-1000123.0f, -1000123.0f);

inline bool BtIsValid(int v) { return v != BT_INVALID_ID; }
inline bool BtIsValid(float v) { return v != BT_INVALID_FLOAT; }
inline bool BtIsValid(const cocos2d::Vec2& v) { return v != BT_INVALID_VEC2; }

template <typename T> void BtDeletePointer(T& ptr)  {if (ptr)    { delete ptr; ptr = nullptr; } }
template <typename T> void BtDeleteArray(T& pArray) {if (pArray) { delete [] pArray; pArray = nullptr; } }

template <typename ELEM, typename CONT>
bool BtIsInContainer(ELEM e, CONT c) 
{
    return std::find(c.begin(), c.end(), e) != c.end(); 
}

//=============================================================================
//  self & base defining
//=============================================================================
#define BT_POLYM_DEF(derivedClassName, baseClassName) \
    static derivedClassName* Create() { return new derivedClassName; } \
    typedef derivedClassName    self_type; \
    typedef baseClassName       base_type;

//=============================================================================
//  singleton defining
//=============================================================================
/**
 *	Singleton Definition 
 *
 *  Remarks:
 *  1) using CreateInst() & DestroyInst() to enable/disable the singleton
 *  2) using Get() to access the singleton pointer
 *  3) the ctor & dtor are set to be private to prevent being exposed to outside
 */
#define BT_SINGLETON_DEF(className) \
    public: \
    static className* Get() { return BT_SGT_INST(className); } \
    static className* CreateInst() \
        { \
        DestroyInst(); \
        BT_SGT_INST(className) = new className;\
        return BT_SGT_INST(className); \
        } \
        static void DestroyInst() \
        { \
        BtDeletePointer(BT_SGT_INST(className)); \
        } \
    private: \
    static className* BT_SGT_INST(className); \
    template <typename T> friend void BtDeletePointer(T& ptr); \
    className(); \
    virtual ~className(); // the dtor should be virtual in case it's in a class hierarchy


/**
 *	Singleton Implementation
 */
#define BT_SINGLETON_IMPL(className) \
    className* className::BT_SGT_INST(className) = NULL;

/**
 *	Singleton Object 
 */
#define BT_SGT_INST(className)  s_##className

/* 
    Round to the nearest integer
*/
template <typename T>
T BtRound(T value)
{
    T base = floor(value);
    if (value < base + T(0.5f))
    {
        return base;
    } 
    else
    {
        return base + T(1.0f);
    }
}

const float BT_EPSILON = 0.00001f;
template <typename T>
T BtIsZero(T value)
{
    return value < (T)BT_EPSILON && value > - (T)BT_EPSILON;
}

typedef cocos2d::experimental::TMXTiledMap tileMap_t;
typedef cocos2d::experimental::TMXLayer tileLayer_t;

inline float BtGetVisibleWidth()
{
    return cocos2d::Director::getInstance()->getVisibleSize().width;
}

inline float BtGetVisibleHeight()
{
    return cocos2d::Director::getInstance()->getVisibleSize().height;
}

inline float BtGetVisibleTop()
{
    return cocos2d::Director::getInstance()->getVisibleOrigin().y + cocos2d::Director::getInstance()->getVisibleSize().height;
}

