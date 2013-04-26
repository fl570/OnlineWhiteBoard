# The Makefile to build DBManager and it's test

CXX = g++

# System Header
SYSTEM_HEADER_DIR = /usr/include

# The root dir
ROOT_DIR = ../..

# The 3rdparty dir
THRIDPARTY_DIR = $(ROOT_DIR)/thridparty

# To find the gtest
GTEST_DIR = $(THRIDPARTY_DIR)/gtest-1.6.0

# To find the tbb
TBB_DIR = $(THRIDPARTY_DIR)/tbb41_20130314oss

# To find the RCF
RCF_DIR = $(THRIDPARTY_DIR)/RCF-2.0.0.2679

# To find the tbb lib
TBB_LIB = $(TBB_DIR)/build/linux_intel64_gcc_cc4.6.1_libc2.13_kernel3.0.0_release

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# Flags passed to the preprocessor
CPPFLAGS += -I$(GTEST_DIR)/include -I$(RCF_DIR)/include\
 	 -I$(TBB_DIR)/include -L$(TBB_LIB)

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# Flags passed to the C++ compiler
CXXFLAGS += -g -Wall -Wextra -O2 -DNDEBUG -m64

# Test
TESTS = ../test/MeetingHandler.test

# sources
DB_MANAGER_SRCS_ = ../DBManager/DBManager.cc
MEM_CACHE_SRCS_ = ../DataProvider/MemoryCache.cc
UPDATER_SRCS_ = ../DataUpdater/Updater.cc
MESSAGE_SRCS_ = ../message.pb.cc
MEETING_HANDLER_SRCS_ = Handler.cc MeetingHandler.cc md5.cc
RCF_SRCS_ = $(RCF_DIR)/src/RCF/RCF.cpp
MEETING_HANDLER_TEST_SRCS_ = MeetingHandlerTest.cc

all : $(TESTS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(TESTS) : Handler.o md5.o MeetingHandler.o Updater.o MemoryCache.o\
message.pb.o DBManager.o RCF.o MeetingHandlerTest.o gtest.a
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) Handler.o md5.o MeetingHandler.o Updater.o MemoryCache.o\
 message.pb.o DBManager.o RCF.o MeetingHandlerTest.o gtest.a -ldl -lboost_thread -lpthread -ltbb -lprotobuf -lglog -lgflags -lgtest -lmysqlclient -o $@

MeetingHandlerTest.o : $(MEETING_HANDLER_TEST_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(MEETING_HANDLER_TEST_SRCS_)

RCF.o : $(RCF_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(RCF_SRCS_)

DBManager.o : $(Memory_Cache_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(DB_MANAGER_SRCS_)

message.pb.o : $(MESSAGE_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(MESSAGE_SRCS_)

MemoryCache.o : $(MEM_CACHE_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(MEM_CACHE_SRCS_)

Updater.o : $(UPDATER_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(UPDATER_SRCS_)

Handler.o md5.o MeetingHandler.o : $(MEETING_HANDLER_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(MEETING_HANDLER_SRCS_)
clean :
	rm -rf $(TESTS) *.a *.o test/*.o 
