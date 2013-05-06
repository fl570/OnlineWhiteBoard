/*************************************************************************
     ** File Name: DrawOperationTest.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include "./DrawOperation.h"
#include "time.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawOperationTest: public ::testing::Test {
protected:
  DrawOperation * d ;
//   static void SetUpTestCase() {
//     GOOGLE_PROTOBUF_VERIFY_VERSION;
//     google::InitGoogleLogging("LOG");
//     FLAGS_log_dir = "../LOG";
//   }
//   static void TearDownTestCase() {
//     google::protobuf::ShutdownProtobufLibrary();
//     google::ShutdownGoogleLogging();
//   }
  virtual void SetUp() {
    d = new DrawOperation("fl570");
  }
  virtual void TearDown() {
    delete d;
  }
};

TEST_F(DrawOperationTest, set) {
  Operation  op;
  op.set_serial_number(1);
  Operation_OperationData*  data;
  data=op.mutable_data();
  data->set_data_type(Operation_OperationData_OperationDataType_ELLIPSE);
  data->set_thinkness(1);
  data->set_color(2);
  data->set_a(300);
  data->set_b(50);
  data->set_fill(true);
  data->set_alpha(0.7);
  Operation_OperationData_Point* point1;
  point1=data->mutable_center();
  point1->set_x(100);
  point1->set_y(100);
  d->Draw(op);
  std::string path = d->SaveAsBmp(1);
  time_t now = time(NULL);
  std::ostringstream bufstr;
  bufstr << now;
  std::string now_time = bufstr.str();
  EXPECT_EQ("/root/picture/fl570-" +now_time+ ".bmp", path);
}

}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

