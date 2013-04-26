/*************************************************************************
     ** File Name: DrawTransRectTest.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#include "./DrawTransRect.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawTransRectTest: public ::testing::Test {
protected:
  DrawTransRect * transRect ;
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
    transRect = new DrawTransRect();
  }
  virtual void TearDown() {
    delete transRect;
  }
};

TEST_F(DrawTransRectTest, set) {
    transRect->set(1, 1, 50, 50, 200, 200, 0.5);
    EXPECT_EQ(1, transRect->color_);
    EXPECT_EQ(1, transRect->thinkness_);
    EXPECT_EQ(50, transRect->xa_);
    EXPECT_EQ(50, transRect->ya_);
    EXPECT_EQ(200, transRect->xb_);
    EXPECT_EQ(200, transRect->yb_);
    EXPECT_EQ(0.5, transRect->alpha_);
}

TEST_F(DrawTransRectTest, Draw) {
    DrawTransRect transRect;
    transRect.set(3, 1, 100, 100, 250, 250, 0.5);
    cv::Mat m=cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    transRect.Draw(m);
    IplImage img = IplImage(m);
    IplImage *pImage = &img;
    EXPECT_EQ(128, cvGet2D(pImage, 100, 100).val[0]);
    EXPECT_EQ(255, cvGet2D(pImage, 100, 100).val[1]);
    EXPECT_EQ(128, cvGet2D(pImage, 100, 100).val[2]);
    EXPECT_EQ(128, cvGet2D(pImage, 175, 175).val[0]);
    EXPECT_EQ(255, cvGet2D(pImage, 175, 175).val[1]);
    EXPECT_EQ(128, cvGet2D(pImage, 175, 175).val[2]);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
