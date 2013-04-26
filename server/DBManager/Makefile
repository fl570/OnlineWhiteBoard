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

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# Flags passed to the preprocessor
CPPFLAGS += -I$(GTEST_DIR)/include

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# Flags passed to the C++ compiler
CXXFLAGS += -g -Wall -Wextra

# Test
TESTS = ../test/DBManager.test

# MemoryCache sources
DB_MANAGER_SRCS_ = DBManager.cc
DB_MANAGER_TEST_SRCS_ = DBManagerTest.cc

all : $(TESTS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(TESTS) : DBManagerTest.o DBManager.o gtest.a
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) DBManagerTest.o \
	DBManager.o gtest.a -lglog -lgflags -lmysqlclient -o $@

DBManagerTest.o : $(DB_MANAGER_TEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DB_MANAGER_TEST_SRCS_)

DBManager.o : $(DB_MANAGER_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DB_MANAGER_SRCS_)

clean :
	rm -rf $(TESTS) *.a *.o test/*.o 
