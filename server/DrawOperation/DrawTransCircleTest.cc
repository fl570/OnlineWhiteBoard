/*************************************************************************
     ** File Name: DrawTransCircleTest.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#include "./DrawTransCircle.h"


namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawTransCircleTest: public ::testing::Test {
protected:
  DrawTransCircle * transCircle ;
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
    transCircle = new DrawTransCircle();
  }
  virtual void TearDown() {
    delete transCircle;
  }
};

TEST_F(DrawTransCircleTest, set) {
    transCircle->set(2, 1, 50, 50, 50,0.5);
    EXPECT_EQ(2, transCircle->color_);
    EXPECT_EQ(1, transCircle->thinkness_);
    EXPECT_EQ(50, transCircle->xa_);
    EXPECT_EQ(50, transCircle->ya_);
    EXPECT_EQ(50, transCircle->r_);
    EXPECT_EQ(0.5, transCircle->alpha_);
}

TEST_F(DrawTransCircleTest, Draw) {
    transCircle->set(4, 2, 100, 100, 100,0.5);
    cv::Mat m=cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    transCircle->Draw(m);
    IplImage img = IplImage(m);
    IplImage *pImage = &img;
    EXPECT_EQ(128, cvGet2D(pImage, 100, 0).val[0]);
    EXPECT_EQ(128, cvGet2D(pImage, 100, 0).val[1]);
    EXPECT_EQ(255, cvGet2D(pImage, 100, 0).val[2]);
    EXPECT_EQ(128, cvGet2D(pImage, 100, 100).val[0]);
    EXPECT_EQ(128, cvGet2D(pImage, 100, 100).val[1]);
    EXPECT_EQ(255, cvGet2D(pImage, 100, 100).val[2]);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

  