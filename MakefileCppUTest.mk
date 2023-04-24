#This makefile makes all the main book code with CppUTest test harness

include $(COMMON_CODE_DIR)/extern/makefile.include

#Set this to @ to keep the makefile quiet
SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = common_code
CPPUTEST_USE_EXTENSIONS = Y
CPPUTEST_USE_MEM_LEAK_DETECTION = N

#--- Inputs ----#
#CPP_PLATFORM = gcc
PROJECT_HOME_DIR = .

SRC_DIRS = src/algorithm \
		   src/data_struct \

#BIN_SRC_DIRS = tests \
#	old_test \
#	tools \

TEST_SRC_DIRS = \
	tests \
	
INCLUDE_DIRS =\
  .\
  src/algorithm \
  src/data_struct \
  $(CPPUTEST_HOME)/include \

#MOCKS_SRC_DIRS = \
#	mocks \
	
CPPUTEST_WARNINGFLAGS = -Wall -Wswitch-default -Werror 
CPPUTEST_CFLAGS += -fPIC 
#CPPUTEST_CFLAGS += -Wall -Wstrict-prototypes -pedantic
LD_LIBRARIES = -lpthread -lrt $(LIB_CRC)
	
  
ifeq ($(CPPUTEST_HOME),)
$(info CPPUTEST_HOME not set! See README.txt)
else  
include $(CPPUTEST_HOME)/build/MakefileWorker.mk
endif
