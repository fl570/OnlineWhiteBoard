/*************************************************************************
     ** File Name :  MemoryCacheTest.cc
    ** Author :  fl570
    ** Mail :  cqfl570@gmail.com
    ** Created Time :  Wed Apr 10 16 : 46 : 51 2013
    **Copyright [2013]  <Copyright fl570>   [legal/copyright]
 ************************************************************************/

#include "./MemoryCache.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataProvider {
class MemoryCacheTest: public ::testing::Test {
protected:
  MemoryCache *q0_ ;
  Operation oper[6];
  static void SetUpTestCase() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    google::InitGoogleLogging("LOG");
    FLAGS_log_dir = "./LOG";
  }
  static void TearDownTestCase() {
    google::protobuf::ShutdownProtobufLibrary();
    google::ShutdownGoogleLogging();
  }
  virtual void SetUp() {
    q0_ = new MemoryCache(5);
    Operation_OperationData *data;
    Operation_OperationData_Point *point;
    for (int i = 0; i < 6; i++) {
      oper[i].set_serial_number(i);
      data = oper[i].mutable_data();
      data -> set_data_type(Operation_OperationData_OperationDataType_LINE);
      data -> set_thinkness(30+i);
      data -> set_color(30+i);
      data -> set_a(30+i);
      data -> set_b(40+i);
      data -> set_fill(true);
      point = data -> mutable_start_point();
      point -> set_x(30+i);
      point -> set_y(40+i);
    }
    for (int i = 0; i < 4; i++) {
      q0_->AddOperation(oper[i]);
    }
  }
  virtual void TearDown() {
    delete q0_;
  }
};

TEST_F(MemoryCacheTest, Initial) {
    EXPECT_FALSE(q0_->state_);
    EXPECT_EQ(5, q0_->capacity_);
    EXPECT_EQ(0, q0_->front_[0]);
    EXPECT_EQ(0, q0_->front_[1]);
    EXPECT_EQ(3, q0_->rear_[0]);
    EXPECT_EQ(0, q0_->rear_[1]);
    EXPECT_EQ(0, q0_->front_id_[0]);
    EXPECT_EQ(3, q0_->rear_id_[0]);
    EXPECT_EQ(0, q0_->front_id_[1]);
    EXPECT_EQ(0, q0_->rear_id_[1]);
    EXPECT_EQ(0, q0_->size_[1]);
    EXPECT_EQ(4, q0_->size_[0]);
}

TEST_F(MemoryCacheTest, AddOperation) {
    q0_->AddOperation(oper[4]);
    EXPECT_FALSE(q0_->state_);
    EXPECT_EQ(5, q0_->capacity_);
    EXPECT_EQ(0, q0_->front_[0]);
    EXPECT_EQ(0, q0_->front_[1]);
    EXPECT_EQ(4, q0_->rear_[0]);
    EXPECT_EQ(0, q0_->rear_[1]);
    EXPECT_EQ(0, q0_->front_id_[0]);
    EXPECT_EQ(4, q0_->rear_id_[0]);
    EXPECT_EQ(0, q0_->front_id_[1]);
    EXPECT_EQ(0, q0_->rear_id_[1]);
    EXPECT_EQ(0, q0_->size_[1]);
    EXPECT_EQ(5, q0_->size_[0]);

    q0_->AddOperation(oper[5]);
    EXPECT_FALSE(q0_->state_);
    EXPECT_EQ(5, q0_->capacity_);
    EXPECT_EQ(1, q0_->front_[0]);
    EXPECT_EQ(0, q0_->front_[1]);
    EXPECT_EQ(0, q0_->rear_[0]);
    EXPECT_EQ(0, q0_->rear_[1]);
    EXPECT_EQ(1, q0_->front_id_[0]);
    EXPECT_EQ(5, q0_->rear_id_[0]);
    EXPECT_EQ(0, q0_->front_id_[1]);
    EXPECT_EQ(0, q0_->rear_id_[1]);
    EXPECT_EQ(0, q0_->size_[1]);
    EXPECT_EQ(5, q0_->size_[0]);
}

TEST_F(MemoryCacheTest, GetOperationAfter) {
    Operations opers = q0_->GetOperationAfter(-2);
    EXPECT_FALSE(opers.operation_avaliable());

    opers = q0_->GetOperationAfter(-1);
    EXPECT_EQ(4, opers.operations_size());
    EXPECT_EQ(true, opers.operation_avaliable());

    const Operation* oper;
    const Operation_OperationData *data;
    const Operation_OperationData_Point *point;

    for (int i = 0; i < opers.operations_size(); i++) {
      oper = &opers.operations(i);
      EXPECT_EQ(i, oper -> serial_number());
      data = &oper -> data();
      EXPECT_EQ(Operation_OperationData_OperationDataType_LINE,
              data -> data_type());
      point = &data -> start_point();
      EXPECT_EQ((i+30), point -> x());
      EXPECT_EQ((i+40), point -> y());
    }

    opers = q0_->GetOperationAfter(3);
    EXPECT_EQ(0, opers.operations_size());
    EXPECT_TRUE(opers.operation_avaliable());

    opers = q0_->GetOperationAfter(4);
    EXPECT_EQ(0, opers.operations_size());
    EXPECT_TRUE(opers.operation_avaliable());
}

TEST_F(MemoryCacheTest, GetOperationFromStoreAfter) {
    q0_->SetState();

    Operations opers = q0_->GetOperationFromStoreAfter(-2);
    EXPECT_FALSE(opers.operation_avaliable());

    opers = q0_->GetOperationFromStoreAfter(-1);
    EXPECT_EQ(4, opers.operations_size());
    EXPECT_TRUE(opers.operation_avaliable());

    const Operation* oper;
    const Operation_OperationData *data;
    const Operation_OperationData_Point *point;

    for (int i = 0; i < opers.operations_size(); i++) {
      oper = &opers.operations(i);
      EXPECT_EQ(i, oper -> serial_number());
      data = &oper -> data();
      EXPECT_EQ(Operation_OperationData_OperationDataType_LINE,
                      data -> data_type());
      point = &data -> start_point();
      EXPECT_EQ((i+30), point -> x());
      EXPECT_EQ((i+40), point -> y());
    }

    opers = q0_->GetOperationFromStoreAfter(3);
    EXPECT_EQ(0, opers.operations_size());
    EXPECT_TRUE(opers.operation_avaliable());

    opers = q0_->GetOperationFromStoreAfter(4);
    EXPECT_EQ(0, opers.operations_size());
    EXPECT_TRUE(opers.operation_avaliable());
}

TEST_F(MemoryCacheTest, SetState) {
    q0_->SetState();
    EXPECT_TRUE(q0_->state_);
    EXPECT_EQ(5, q0_->capacity_);
    EXPECT_EQ(0, q0_->front_[0]);
    EXPECT_EQ(0, q0_->front_[1]);
    EXPECT_EQ(3, q0_->rear_[0]);
    EXPECT_EQ(0, q0_->rear_[1]);
    EXPECT_EQ(0, q0_->front_id_[0]);
    EXPECT_EQ(3, q0_->rear_id_[0]);
    EXPECT_EQ(0, q0_->front_id_[1]);
    EXPECT_EQ(0, q0_->rear_id_[1]);
    EXPECT_EQ(0, q0_->size_[1]);
    EXPECT_EQ(4, q0_->size_[0]);

    for (int i = 4; i < 6; i++) {
      q0_->AddOperation(oper[i]);
    }

    EXPECT_TRUE(q0_->state_);
    EXPECT_EQ(5, q0_->capacity_);
    EXPECT_EQ(0, q0_->front_[0]);
    EXPECT_EQ(0, q0_->front_[1]);
    EXPECT_EQ(3, q0_->rear_[0]);
    EXPECT_EQ(1, q0_->rear_[1]);
    EXPECT_EQ(0, q0_->front_id_[0]);
    EXPECT_EQ(3, q0_->rear_id_[0]);
    EXPECT_EQ(4, q0_->front_id_[1]);
    EXPECT_EQ(5, q0_->rear_id_[1]);
    EXPECT_EQ(2, q0_->size_[1]);
    EXPECT_EQ(4, q0_->size_[0]);

    q0_->SetState();
    EXPECT_FALSE(q0_->state_);
    EXPECT_EQ(5, q0_->capacity_);
    EXPECT_EQ(0, q0_->front_[0]);
    EXPECT_EQ(0, q0_->front_[1]);
    EXPECT_EQ(0, q0_->rear_[0]);
    EXPECT_EQ(1, q0_->rear_[1]);
    EXPECT_EQ(0, q0_->front_id_[0]);
    EXPECT_EQ(0, q0_->rear_id_[0]);
    EXPECT_EQ(4, q0_->front_id_[1]);
    EXPECT_EQ(5, q0_->rear_id_[1]);
    EXPECT_EQ(2, q0_->size_[1]);
    EXPECT_EQ(0, q0_->size_[0]);
}
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

