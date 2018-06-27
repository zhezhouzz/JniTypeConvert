LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := testjni
FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SRC_FILES := $(FILE_LIST)
LOCAL_LDLIBS := -llog
LOCAL_CFLAGS := -O3
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_CPPFLAGS += -frtti
include $(BUILD_SHARED_LIBRARY)