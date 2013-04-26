/*************************************************************************
     ** File Name: DrawRectTest.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#include "./DrawRect.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawRectTest: public ::testing::Test {
protected:
  DrawRect * rect ;
  static void SetUpTestCase() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    google::InitGoogleLogging("LOG");
    FLAGS_log_dir = "../LOG";
  }
  static void TearDownTestCase() {
    google::protobuf::ShutdownProtobufLibrary();
    google::ShutdownGoogleLogging();
  }
  virtual void SetUp() {
    rect = new DrawRect();
  }
  virtual void TearDown() {
    delete rect;
  }
};

TEST_F(DrawRectTest, set) {
    rect->set(1, 1, 50, 50, 200, 200);
    EXPECT_EQ(1, rect->color_);
    EXPECT_EQ(1, rect->thinkness_);
    EXPECT_EQ(50, rect->xa_);
    EXPECT_EQ(50, rect->ya_);
    EXPECT_EQ(200, rect->xb_);
    EXPECT_EQ(200, rect->yb_);
}

TEST_F(DrawRectTest, Draw) {
    DrawRect rect;
    rect.set(3, 1, 100, 100, 250, 250);
    cv::Mat m=cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    rect.Draw(m);
    IplImage img = IplImage(m);
    IplImage *pImage = &img;
    EXPECT_EQ(0, cvGet2D(pImage, 100, 100).val[0]);
    EXPECT_EQ(255, cvGet2D(pImage, 100, 100).val[1]);
    EXPECT_EQ(0, cvGet2D(pImage, 100, 100).val[2]);
    EXPECT_EQ(0, cvGet2D(pImage, 250, 250).val[0]);
    EXPECT_EQ(255, cvGet2D(pImage, 250, 250).val[1]);
    EXPECT_EQ(0, cvGet2D(pImage, 250, 250).val[2]);
    EXPECT_EQ(255, cvGet2D(pImage, 175, 175).val[0]);
    EXPECT_EQ(255, cvGet2D(pImage, 175, 175).val[1]);
    EXPECT_EQ(255, cvGet2D(pImage, 175, 175).val[2]);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

  