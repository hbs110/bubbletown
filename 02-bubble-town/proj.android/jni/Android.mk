LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../)
$(call import-add-path,$(LOCAL_PATH)/../../external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := ../main.cpp \
        ../../Classes/AppDelegate.cpp \
		../../Classes/AppStartScene.cpp \
        ../../Classes/Scenes/BtBaseScene.cpp \
		../../Classes/Scenes/Scene_Bubble/BtBubbleScene.cpp \
		../../Classes/Scenes/Scene_World/BtWorldScene.cpp \
		../../Classes/Scenes/Scene_Town/BtTownScene.cpp \
        ../../Classes/Core/BtMsgDispatcher.cpp \
		../../Classes/Core/BtGuiUtil.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
