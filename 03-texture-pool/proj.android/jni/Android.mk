LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../)
$(call import-add-path,$(LOCAL_PATH)/../../external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := ../main.cpp \
		../../Classes/AppDelegate.cpp \
		../../Classes/Scenes/AppStartScene.cpp \
		../../Classes/Scenes/BtSceneUtil.cpp \
		../../Classes/Scenes/BtTestScene.cpp \
		../../Classes/Scenes/BtWorldScene.cpp \
		../../Classes/Scenes/BtTownScene.cpp \
        ../../Classes/MsgHandling/BtMsgDispatcher.cpp \
        ../../Classes/TexturePool/Impl/TexturePoolGroup.cpp \
        ../../Classes/TexturePool/TexturePool.cpp \
        ../../Classes/TexturePool/TexturePoolTestScene.cpp \
		../../Classes/TexturePool/TexturePoolTestScene_Anim.cpp \
		../../Classes/Core/BtGuiUtil.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
