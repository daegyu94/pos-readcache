TOP := $(abspath $(CURDIR)/../../..)

###################################################
MFS_TOP_DIR :=$(abspath ../../metafs)
include $(MFS_TOP_DIR)/mvm/Makefile.include

###################################################
export UT_TOP_DIR=$(MFS_TOP_DIR)/unit_test
MFS_MVM_DIR:=$(addprefix $(MFS_TOP_DIR)/, $(MFS_INCLUDE_PATH))

UT_INCLUDE =-I$(UT_TOP_DIR) -I$(MFS_MVM_DIR)
MFS_INCLUDE_PATH += \
                    $(UT_INCLUDE) \
                    $(UT_TOP_DIR) \
                    $(MFS_MVM_DIR) 

VPATH += $(MFS_INCLUDE_PATH)
###############################################################

SRCS += $(wildcard *.cpp) \
	file_descriptor_allocator.cpp \
	meta_volume.cpp \
	active_file_list.cpp \
	fd_inode_map.cpp \
	catalog.cpp \
	catalog_manager.cpp \
	extent_allocator.cpp \
	meta_file_inode.cpp \
	inode_table_header.cpp \
	inode_creator.cpp \
	inode_deleter.cpp \
	inode_manager.cpp \
	inode_table.cpp \
	on_volume_meta_region_mgr.cpp \
	ssd_meta_volume.cpp \
	nvram_meta_volume.cpp

#UT_SRCS +=\
#    mvm_test.cpp \
#    mvm_func_negative_test.cpp \
#    mvm_func_positive_test.cpp 


SRCS += $(UT_SRCS)

###############################################################
  
-include $(TOP)/Makefile.rules 
include Makefile.mvm
