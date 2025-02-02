MODULE_NAME = koo_resize_room

# DIRs
VOS_DRIVER_DIR = $(NVT_VOS_DIR)/drivers
HDAL_SAMPLE_DIR = $(NVT_HDAL_DIR)/samples
RTOS_OUTPUT_DIR = $(HDAL_SAMPLE_DIR)/output
# INCs
VOS_INC_PATH = $(VOS_DRIVER_DIR)/include
HDAL_INC_PATH = $(NVT_HDAL_DIR)/include
HDAL_LIB_PATH = $(NVT_HDAL_DIR)/output

INCLUDE_DIR = $(LINUX_BUILD_TOP)/code/lib/include

# INC FLAGs
EXTRA_INCLUDE += -I$(HDAL_INC_PATH) -I$(VOS_INC_PATH) -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/freetype2 -I$(NVT_HDAL_DIR)/vendor/media/include

.PHONY: all clean
###############################################################################
# Linux Makefile                                                              #
###############################################################################

#--------- ENVIRONMENT SETTING --------------------
WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes -Werror
COMPILE_OPTS	=  -I. -O2 -fPIC -ffunction-sections -fdata-sections -D__LINUX
C_CFLAGS	= $(PLATFORM_CFLAGS) $(COMPILE_OPTS) $(WARNING) $(EXTRA_INCLUDE)
LD_FLAGS	= -L$(HDAL_LIB_PATH) -L$(LIBRARY_DIR)/output -L$(NVT_HDAL_DIR)/vendor/output -lhdal -lpthread  -lvendor_media -lfreetype -lm
#--------- END OF ENVIRONMENT SETTING -------------
LIB_NAME = $(MODULE_NAME)
SRC = koo_resize_room.c

OBJ = $(SRC:.c=.o)

ifeq ("$(wildcard *.c */*.c)","")
all:
	@echo "nothing to be done for '$(OUTPUT_NAME)'"
clean:
	@echo "nothing to be done for '$(OUTPUT_NAME)'"
else
all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	@echo Creating $@...
	@$(CC) -o $@ $(OBJ) $(LD_FLAGS)
	@$(NM) -n $@ > $@.sym
	@$(STRIP) $@
	@$(OBJCOPY) -R .comment -R .note.ABI-tag -R .gnu.version $@

%.o: %.c
	@echo Compiling $<
	@$(CC) $(C_CFLAGS) -c $< -o $@

clean:
	@rm -f $(LIB_NAME) $(OBJ) $(LIB_NAME).sym *.o *.a *.so*
endif

install:
	@cp -avf $(LIB_NAME) $(ROOTFS_DIR)/rootfs/usr/bin

