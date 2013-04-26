/*************************************************************************
     ** File Name: DrawCircleTest.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#include "./DrawCircle.h"


namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawCircleTest: public ::testing::Test {
protected:
  DrawCircle * circle ;
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
    circle = new DrawCircle();
  }
  virtual void TearDown() {
    delete circle;
  }
};

TEST_F(DrawCircleTest, set) {
    circle->set(2, 1, 50, 50, 50);
    EXPECT_EQ(2, circle->color_);
    EXPECT_EQ(1, circle->thinkness_);
    EXPECT_EQ(50, circle->xa_);
    EXPECT_EQ(50, circle->ya_);
    EXPECT_EQ(50, circle->r_);
}

TEST_F(DrawCircleTest, Draw) {
    circle->set(4, 2, 100, 100, 100);
    cv::Mat m=cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    circle->Draw(m);
    IplImage img = IplImage(m);
    IplImage *pImage = &img;
    EXPECT_EQ(0, cvGet2D(pImage, 100, 0).val[0]);
    EXPECT_EQ(0, cvGet2D(pImage, 100, 0).val[1]);
    EXPECT_EQ(255, cvGet2D(pImage, 100, 0).val[2]);
    EXPECT_EQ(255, cvGet2D(pImage, 100, 100).val[0]);
    EXPECT_EQ(255, cvGet2D(pImage, 100, 100).val[1]);
    EXPECT_EQ(255, cvGet2D(pImage, 100, 100).val[2]);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

  