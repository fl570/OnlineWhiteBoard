/*************************************************************************
     ** File Name: DrawEraserTest.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#include "./DrawEraser.h"
#include "./DrawLine.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawEraserTest: public ::testing::Test {
protected:
  DrawEraser * eraser ;
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
    eraser = new DrawEraser();
  }
  virtual void TearDown() {
    delete eraser;
  }
};

TEST_F(DrawEraserTest, set) {
    eraser->set(1, 50, 50);
    EXPECT_EQ(1, eraser->thinkness_);
    EXPECT_EQ(50, eraser->xa_);
    EXPECT_EQ(50, eraser->ya_);
}

TEST_F(DrawEraserTest, Draw) {
    DrawLine line;
    line.set(1, 1, 100, 100, 250, 250);
    cv::Mat m=cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    line.Draw(m);
    eraser->set(1, 150, 150);
    eraser->Draw(m);
    IplImage img = IplImage(m);
    IplImage *pImage = &img;
    EXPECT_EQ(0, cvGet2D(pImage, 100, 100).val[0]);
    EXPECT_EQ(0, cvGet2D(pImage, 100, 100).val[1]);
    EXPECT_EQ(0, cvGet2D(pImage, 100, 100).val[2]);
    EXPECT_EQ(0, cvGet2D(pImage, 175, 175).val[0]);
    EXPECT_EQ(0, cvGet2D(pImage, 175, 175).val[1]);
    EXPECT_EQ(0, cvGet2D(pImage, 175, 175).val[2]);
    EXPECT_EQ(0, cvGet2D(pImage, 250, 250).val[0]);
    EXPECT_EQ(0, cvGet2D(pImage, 250, 250).val[1]);
    EXPECT_EQ(0, cvGet2D(pImage, 250, 250).val[2]);
    EXPECT_EQ(255, cvGet2D(pImage, 150, 150).val[0]);
    EXPECT_EQ(255, cvGet2D(pImage, 150, 150).val[1]);
    EXPECT_EQ(255, cvGet2D(pImage, 150, 150).val[2]);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

  