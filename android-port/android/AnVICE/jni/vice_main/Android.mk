LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := vice_main

MY_PATH2 := ../vice/

CG_SUBDIRS := \
$(MY_PATH2)/src/raster \
$(MY_PATH2)/src/rtc \
$(MY_PATH2)/src/video \
$(MY_PATH2)/src/c64/cart \
$(MY_PATH2)/src/drive/iec \
$(MY_PATH2)/src/drive/iecieee \
$(MY_PATH2)/src/drive/iec/c64exp \
$(MY_PATH2)/src/drive/ieee \
$(MY_PATH2)/src/drive \
$(MY_PATH2)/src/iecbus \
$(MY_PATH2)/src/parallel \
$(MY_PATH2)/src/vdrive \
$(MY_PATH2)/src/sid \
$(MY_PATH2)/src/monitor \

# Add more subdirs here, like src/subdir1 src/subdir2

MY_PATH := $(LOCAL_PATH)

#Fix me
LOCAL_PATH := "C:/Projects/Eclipse/C64/AnVICE/jni/vice/"

CG_SRCDIR := $(LOCAL_PATH)
LOCAL_CFLAGS :=	-I$(LOCAL_PATH)/include \
				$(foreach D, $(CG_SUBDIRS), -I$(CG_SRCDIR)/$(D)) \
				-I$(LOCAL_PATH)/../sdl/include \
				-I$(LOCAL_PATH)/../locnet/include \
				-I$(LOCAL_PATH) \
				-I$(LOCAL_PATH)/src \
				-iquote$(LOCAL_PATH)/src/vicii \
				-iquote$(LOCAL_PATH)/src/c64 \
				-iquote$(LOCAL_PATH)/src/c64dtv \
				-iquote$(LOCAL_PATH)/src/imagecontents \
				-iquote$(LOCAL_PATH)/src/core \
				-iquote$(LOCAL_PATH)/src/tape \
				-iquote$(LOCAL_PATH)/src/arch/sdl \
				-iquote$(LOCAL_PATH)/src/arch \
				-iquote$(LOCAL_PATH)/src
				#-I$(LOCAL_PATH)/../sdl_mixer \
				#-I$(LOCAL_PATH)/../sdl/src/video/android \
				#-I$(LOCAL_PATH)/../sdl/src/events \
				#-I$(LOCAL_PATH)/../stlport/stlport \
				#-I$(SYSROOT)/usr/include
				
LOCAL_PATH := $(MY_PATH)

LOCAL_CPPFLAGS := $(LOCAL_CFLAGS)
LOCAL_CXXFLAGS := $(LOCAL_CFLAGS)

#Change C++ file extension as appropriate
LOCAL_CPP_EXTENSION := .cpp

#LOCAL_SRC_FILES := $(foreach F, $(CG_SUBDIRS), $(addprefix $(F)/,$(notdir $(wildcard $(LOCAL_PATH)/$(F)/*.cpp))))
# Uncomment to also add C sources
LOCAL_SRC_FILES += $(foreach F, $(CG_SUBDIRS), $(addprefix $(F)/,$(notdir $(wildcard $(LOCAL_PATH)/$(F)/*.c))))
LOCAL_SRC_FILES += \
	$(MY_PATH2)/src/alarm.c \
	$(MY_PATH2)/src/attach.c \
	$(MY_PATH2)/src/autostart.c \
	$(MY_PATH2)/src/autostart-prg.c \
	$(MY_PATH2)/src/charset.c \
	$(MY_PATH2)/src/clkguard.c \
	$(MY_PATH2)/src/clipboard.c \
	$(MY_PATH2)/src/cmdline.c \
	$(MY_PATH2)/src/cbmdos.c \
	$(MY_PATH2)/src/cbmimage.c \
	$(MY_PATH2)/src/color.c \
	$(MY_PATH2)/src/crc32.c \
	$(MY_PATH2)/src/datasette.c \
	$(MY_PATH2)/src/debug.c \
	$(MY_PATH2)/src/dma.c \
	$(MY_PATH2)/src/embedded.c \
	$(MY_PATH2)/src/event.c \
	$(MY_PATH2)/src/findpath.c \
	$(MY_PATH2)/src/fliplist.c \
	$(MY_PATH2)/src/gcr.c \
	$(MY_PATH2)/src/info.c \
	$(MY_PATH2)/src/init.c \
	$(MY_PATH2)/src/initcmdline.c \
	$(MY_PATH2)/src/interrupt.c \
	$(MY_PATH2)/src/ioutil.c \
	$(MY_PATH2)/src/joystick.c \
	$(MY_PATH2)/src/kbdbuf.c \
	$(MY_PATH2)/src/keyboard.c \
	$(MY_PATH2)/src/lib.c \
	$(MY_PATH2)/src/libm_math.c \
	$(MY_PATH2)/src/lightpen.c \
	$(MY_PATH2)/src/log.c \
	$(MY_PATH2)/src/machine-bus.c \
	$(MY_PATH2)/src/machine.c \
	$(MY_PATH2)/src/main.c \
	$(MY_PATH2)/src/network.c \
	$(MY_PATH2)/src/opencbmlib.c \
	$(MY_PATH2)/src/palette.c \
	$(MY_PATH2)/src/ram.c \
	$(MY_PATH2)/src/rawfile.c \
	$(MY_PATH2)/src/rawnet.c \
	$(MY_PATH2)/src/resources.c \
	$(MY_PATH2)/src/romset.c \
	$(MY_PATH2)/src/screenshot.c \
	$(MY_PATH2)/src/snapshot.c \
	$(MY_PATH2)/src/socket.c \
	$(MY_PATH2)/src/sound.c \
	$(MY_PATH2)/src/sysfile.c \
	$(MY_PATH2)/src/translate.c \
	$(MY_PATH2)/src/traps.c \
	$(MY_PATH2)/src/util.c \
	$(MY_PATH2)/src/vsync.c \
	$(MY_PATH2)/src/zfile.c \
	$(MY_PATH2)/src/zipcode.c \
	$(MY_PATH2)/src/mouse.c \
	$(MY_PATH2)/src/midi.c \
	$(MY_PATH2)/src/maincpu.c

LOCAL_PATH := $(MY_PATH)
	
# LOCAL_STATIC_LIBRARIES := sdl_mixer sdl tremor stlport
#LOCAL_STATIC_LIBRARIES := locnet_al

#LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lsupc++ -ljnigraphics

#LOCAL_LDLIBS := -ldl -lsupc++ -ljnigraphics

LOCAL_ARM_MODE := arm

include $(BUILD_STATIC_LIBRARY)

