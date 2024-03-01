#This makefile makes all the main book code with CppUTest test harness

include $(COMMON_CODE_DIR)/extern/makefile.include

#Set this to @ to keep the makefile quiet
SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = common_code
CPPUTEST_USE_EXTENSIONS = Y
CPPUTEST_USE_MEM_LEAK_DETECTION = N

#--- Target platform---
BOOL_OS_IS_LINUX="$(shell  uname -a |grep x86_64|wc -l )"

TARGET_PLATFORM=x86
ifeq ($(BOOL_OS_IS_LINUX), "0")
TARGET_PLATFORM=arm
endif

#--- Inputs ----#
#CPP_PLATFORM = gcc
PROJECT_HOME_DIR = .

SRC_DIRS = src/algorithm \
		   src/data_struct \
		   src/data_struct/ringbuffer \
		   src/application \
		   src/os \
		   src/network \
		   src/db \

#BIN_SRC_DIRS = tests \
#	old_test \
#	tools \

TEST_SRC_DIRS = \
	tests \
	
INCLUDE_DIRS =\
  .\
  src/algorithm \
  src/data_struct \
  src/data_struct/ringbuffer \
  src/application \
  src/os \
  src/network \
  src/db \
  $(CPPUTEST_HOME)/include \

#MOCKS_SRC_DIRS = \
#	mocks \
	
CPPUTEST_WARNINGFLAGS = -Wall -Wswitch-default -Werror
CPPUTEST_CFLAGS += -fPIC
#CPPUTEST_CFLAGS += -Wall -Wstrict-prototypes -pedantic
CPPUTEST_CPPFLAGS += -fPIC -fpermissive -w
LD_LIBRARIES = -lpthread -lrt $(LIB_CRC) -lcrypto
	
  
ifeq ($(CPPUTEST_HOME),)
$(info CPPUTEST_HOME not set! See README.txt)
else  
include $(CPPUTEST_HOME)/build/MakefileWorker.mk
endif
