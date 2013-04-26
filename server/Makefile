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

# To find the tbb
TBB_DIR = $(THRIDPARTY_DIR)/tbb41_20130314oss

# To find the RCF
RCF_DIR = $(THRIDPARTY_DIR)/RCF-2.0.0.2679

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# To find the tbb lib
TBB_LIB = $(TBB_DIR)/build/linux_intel64_gcc_cc4.6.1_libc2.13_kernel3.0.0_release

# Flags passed to the preprocessor
CPPFLAGS += -I$(GTEST_DIR)/include -I$(TBB_DIR)/include -L$(TBB_LIB) -I$(RCF_DIR)/include

# Flags passed to the C++ compiler
CXXFLAGS += -g -Wall -Wextra -O2 -DNDEBUG -m64

# Test
TESTS = test/all.test

# Message
MESSAGE = message.pb.cc

# DrawOperation sources
DRAW_OPERATION_SRCS_ = DrawOperation/DrawOperation.cc
DRAW_OPERATION_TEST_SRCS_ = DrawOperation/DrawOperationTest.cc
DRAW_CLASS_SRCS_ = DrawOperation/DrawClass.cc
DRAW_CIRCLE_SRCS_ = DrawOperation/DrawCircle.cc
DRAW_TRANSCIRCLE_SRCS_ = DrawOperation/DrawTransCircle.cc
DRAW_RECT_SRCS_ = DrawOperation/DrawRect.cc
DRAW_TRANSRECT_SRCS_ = DrawOperation/DrawTransRect.cc
DRAW_ECLIPSE_SRCS_ = DrawOperation/DrawEclipse.cc
DRAW_TRANSECLIPSE_SRCS_ =DrawOperation/DrawTransEclipse.cc
DRAW_LINE_SRCS_ = DrawOperation/DrawLine.cc
DRAW_POINT_SRCS_ = DrawOperation/DrawPoint.cc
DRAW_ERASER_SRCS_ = DrawOperation/DrawEraser.cc
DRAW_CIRCLE_TEST_SRCS_ = DrawOperation/DrawCircleTest.cc
DRAW_TRANSCIRCLE_TEST_SRCS_ = DrawOperation/DrawTransCircleTest.cc
DRAW_TRANSRECT_TEST_SRCS_ = DrawOperation/DrawTransRectTest.cc
DRAW_RECT_TEST_SRCS_ = DrawOperation/DrawRectTest.cc
DRAW_LINE_TEST_SRCS_ = DrawOperation/DrawLineTest.cc
DRAW_POINT_TEST_SRCS_ = DrawOperation/DrawPointTest.cc
DRAW_ERASER_TEST_SRCS_ = DrawOperation/DrawEraserTest.cc

# MemoryCache sources
Memory_Cache_SRCS_ = DataProvider/MemoryCache.cc
Memory_Cache_TEST_SRCS_ = DataProvider/MemoryCacheTest.cc
DB_MANAGER_SRCS_ = DBManager/DBManager.cc
DB_MANAGER_TEST_SRCS_ = DBManager/DBManagerTest.cc
MEETING_HANDLER_SRCS_ = Monitor/Handler.cc Monitor/MeetingHandler.cc Monitor/md5.cc
MEETING_HANDLER_TEST_SRCS_ = Monitor/MeetingHandlerTest.cc
UPDATER_SRCS_ = DataUpdater/Updater.cc
RCF_SRCS_ = $(RCF_DIR)/src/RCF/RCF.cpp

all : $(TESTS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(TESTS) : MemoryCacheTest.o MemoryCache.o message.pb.o gtest.a DBManagerTest.o DBManager.o \
	Handler.o md5.o MeetingHandler.o Updater.o \
	RCF.o MeetingHandlerTest.o gtest.a DrawOperationTest.o DrawCircleTest.o DrawTransCircleTest.o \
        DrawRectTest.o DrawTransRectTest.o DrawLineTest.o \
        DrawPointTest.o DrawEraserTest.o DrawOperation.o DrawClass.o \
        DrawLine.o DrawPoint.o DrawEraser.o DrawRect.o \
        DrawTransRect.o DrawCircle.o DrawTransCircle.o \
	DrawEclipse.o DrawTransEclipse.o
	$(CXX) MemoryCacheTest.o MemoryCache.o message.pb.o gtest.a DBManagerTest.o DBManager.o \
	Handler.o md5.o MeetingHandler.o Updater.o \
	RCF.o MeetingHandlerTest.o gtest.a DrawOperationTest.o DrawCircleTest.o DrawTransCircleTest.o \
	DrawRectTest.o DrawTransRectTest.o DrawLineTest.o \
	DrawPointTest.o DrawEraserTest.o DrawOperation.o DrawClass.o \
	DrawLine.o DrawPoint.o DrawEraser.o DrawRect.o \
	DrawTransRect.o DrawCircle.o DrawTransCircle.o \
	DrawEclipse.o DrawTransEclipse.o \
	-ldl -lpthread -ltbb `pkg-config --cflags opencv`  `pkg-config --libs opencv` \
	-lglog -lgflags -lprotobuf -lboost_thread -lmysqlclient -lgtest \
	$(CXXFLAGS) $(CPPFLAGS) -o $@

MeetingHandlerTest.o : $(MEETING_HANDLER_TEST_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(MEETING_HANDLER_TEST_SRCS_)

RCF.o : $(RCF_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(RCF_SRCS_)

Updater.o : $(UPDATER_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(UPDATER_SRCS_)

Handler.o md5.o MeetingHandler.o : $(MEETING_HANDLER_SRCS_)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(MEETING_HANDLER_SRCS_)

MemoryCacheTest.o : $(Memory_Cache_TEST_SRCS_)
	$(CXX) $(CXXFLAGS) -c $(Memory_Cache_TEST_SRCS_)

MemoryCache.o : $(Memory_Cache_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(Memory_Cache_SRCS_)

message.pb.o : $(MESSAGE) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(MESSAGE)

DBManagerTest.o : $(DB_MANAGER_TEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DB_MANAGER_TEST_SRCS_)

DBManager.o : $(DB_MANAGER_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DB_MANAGER_SRCS_)

DrawOperationTest.o : $(DRAW_OPERATION_TEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_OPERATION_TEST_SRCS_)

DrawOperation.o : $(DRAW_OPERATION_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_OPERATION_SRCS_)

DrawCircle.o : $(DRAW_CIRCLE_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_CIRCLE_SRCS_)

DrawCircleTest.o : $(DRAW_CIRCLE_TEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_CIRCLE_TEST_SRCS_)

DrawTransCircle.o : $(DRAW_TRANSCIRCLE_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_TRANSCIRCLE_SRCS_)

DrawTransCircleTest.o : $(DRAW_TRANSCIRCLE_TEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_TRANSCIRCLE_TEST_SRCS_)

DrawRect.o : $(DRAW_RECT_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_RECT_SRCS_)

DrawRectTest.o : $(DRAW_RECT_TEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_RECT_TEST_SRCS_)

DrawTransRect.o : $(DRAW_TRANSRECT_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_TRANSRECT_SRCS_)

DrawTransRectTest.o : $(DRAW_TRANSRECT_TEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_TRANSRECT_TEST_SRCS_)

DrawEclipse.o : $(DRAW_ECLIPSE_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_ECLIPSE_SRCS_)

DrawTransEclipse.o : $(DRAW_TRANSECLIPSE_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_TRANSECLIPSE_SRCS_)

DrawLine.o : $(DRAW_LINE_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_LINE_SRCS_)

DrawLineTest.o : $(DRAW_LINE_TEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_LINE_TEST_SRCS_)

DrawPoint.o : $(DRAW_POINT_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_POINT_SRCS_)

DrawPointTest.o : $(DRAW_POINT_TEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_POINT_TEST_SRCS_)

DrawEraser.o : $(DRAW_ERASER_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_ERASER_SRCS_)

DrawEraserTest.o : $(DRAW_ERASER_TEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_ERASER_TEST_SRCS_)

DrawClass.o : $(DRAW_CLASS_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(DRAW_CLASS_SRCS_)

clean :
	rm -rf $(TESTS) *.a *.o test/*.o 
