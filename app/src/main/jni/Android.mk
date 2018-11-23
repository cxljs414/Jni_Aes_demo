LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_LDLIBS :=-llog
LOCAL_MODULE := JniUtil
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_SRC_FILES := \
JniUtil.c \
aes256.c \
base64.c

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_PROGUARD_ENABLED:= disabled

include $(BUILD_SHARED_LIBRARY)
