LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#$(call import-add-path,$(LOCAL_PATH)/../../)
#$(call import-add-path,$(LOCAL_PATH)/../../external)
#$(call import-add-path,$(LOCAL_PATH)/../../cocos)

$(call import-add-path,$(LOCAL_PATH)/../../../../)
$(call import-add-path,$(LOCAL_PATH)/../../../../external)
$(call import-add-path,$(LOCAL_PATH)/../../../../cocos)
$(call import-add-path,$(LOCAL_PATH)/../../Dependencies/BtLua/dep)
$(call import-add-path,$(LOCAL_PATH)/../../Dependencies)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := ../main.cpp \
../../Classes/AppStartScene.cpp \
../../Classes/Core/BtGuiUtil.cpp \
../../Classes/Core/BtMsgDispatcher.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/Scenes/Scene_Bubble/BtBubbleScene.cpp \
../../Classes/Scenes/BtBaseScene.cpp \
../../Classes/Scenes/Scene_World/BtWorldScene.cpp \
../../Classes/Scenes/Scene_Town/BtTownSceneWidgets.cpp \
../../Classes/Scenes/Scene_Town/BtTownBuilding.cpp \
../../Classes/Scenes/Scene_Town/BtTiledMap.cpp \
../../Classes/Scenes/Scene_Town/BtTownScene.cpp \
../../Classes/Services/BtLuaService.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes $(LOCAL_PATH)/../../Dependencies/BtLua/dep/LuaBridge/Source/LuaBridge

LOCAL_STATIC_LIBRARIES := cocos2dx_static lua_static btlua_static

LOCAL_CPP_FEATURES := exceptions rtti

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,lua)
$(call import-module,BtLua)
