/*************************************************************************
     ** File Name: DrawLineTest.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#include "./DrawLine.h"


namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawLineTest: public ::testing::Test {
protected:
  DrawLine * line ;
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
    line = new DrawLine();
  }
  virtual void TearDown() {
    delete line;
  }
};

TEST_F(DrawLineTest, set) {
    line->set(2, 1, 50, 50, 100, 100);
    EXPECT_EQ(2, line->color_);
    EXPECT_EQ(1, line->thinkness_);
    EXPECT_EQ(50, line->xa_);
    EXPECT_EQ(50, line->ya_);
    EXPECT_EQ(100, line->xb_);
    EXPECT_EQ(100, line->yb_);
}

TEST_F(DrawLineTest, Draw) {
    line->set(1, 1, 100, 100, 250, 250);
    cv::Mat m=cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
    line->Draw(m);
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
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

int main(int argc, char **argv) {
  std::cout << "Running main() from gtest_main.cc\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
  