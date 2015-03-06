#!/bin/sh

export NDK_ROOT=/home/android/android-ndk-r10d
export ANDROID_SDK_ROOT=/home/android/sdk
export PATH=/home/android/cocos2d/tools/cocos2d-console/bin:$PATH
export ANT_ROOT=/usr/share/ant/bin
export JAVA_HOME=/home/android/jdk1.8.0_20
cd /bubble_src/games/02-bubble-town
cocos run -p android
