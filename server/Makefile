# The Makefile to build MemoryCache and it's test

CXX = g++

# System Header
SYSTEM_HEADER_DIR = /usr/include

# The root dir
ROOT_DIR = ..

# The 3rdparty dir
THRIDPARTY_DIR = $(ROOT_DIR)/thridparty

# To find the gtest
GTEST_DIR = $(THRIDPARTY_DIR)/gtest-1.6.0

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# Flags passed to the preprocessor
CPPFLAGS += -I$(GTEST_DIR)/include

# Flags passed to the C++ compiler
CXXFLAGS += -g -Wall -Wextra

# Test
TESTS = test/MemoryCache.test

# Message
MESSAGE = message.pb.cc

# MemoryCache sources
Memory_Cache_SRCS_ = DataProvider/MemoryCache.cc
Memory_Cache_TEST_SRCS_ = DataProvider/MemoryCacheTest.cc

all : $(TESTS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(TESTS) : MemoryCacheTest.o MemoryCache.o message.pb.o gtest.a
	$(CXX) MemoryCacheTest.o MemoryCache.o message.pb.o gtest.a -lglog -lgflags -lprotobuf -lboost_thread\
        $(CXXFLAGS) $(CPPFLAGS) -o test/MemoryCache.test

MemoryCacheTest.o : $(Memory_Cache_TEST_SRCS_)
	$(CXX) $(CXXFLAGS) -c $(Memory_Cache_TEST_SRCS_)

MemoryCache.o : $(Memory_Cache_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(Memory_Cache_SRCS_)

message.pb.o : $(MESSAGE) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(MESSAGE)

clean :
	rm -rf $(TESTS) *.a *.o test/*.o 
