/*************************************************************************
     ** File Name: UserHandlerTest.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#include "./UserHandler.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor {
class UserHandlerTest: public ::testing::Test {
protected:
  UserHandler *u ;
  MeetingHandler *m;
//   static void SetUpTestCase() {
//     GOOGLE_PROTOBUF_VERIFY_VERSION;
//     google::InitGoogleLogging("LOG");
//     FLAGS_log_dir = "./LOG";
//   }
//   static void TearDownTestCase() {
//     google::protobuf::ShutdownProtobufLibrary();
//     google::ShutdownGoogleLogging();
//   }
  virtual void SetUp() {
    m = new MeetingHandler();
    u = new UserHandler(m);
  }
  virtual void TearDown() {
    delete u;
    delete m;
  }
};

TEST_F(UserHandlerTest, TransferAuth) {
  std::string id = m-> CreateMeeting("bcd");
  JoinMeetingReturn msg = m -> JoinMeeting(id, "cccc");
 msg = m->JoinMeeting(id, "bbbb");
 
  Operation oper;
  Operation_OperationData *data;
  Operation_OperationData_Point *point;
  oper.set_serial_number(1);
  data = oper.mutable_data();
  data -> set_data_type(Operation_OperationData_OperationDataType_LINE);
  data -> set_thinkness(30);
  data -> set_color(30);
  data -> set_a(30);
  data -> set_b(40);
  data -> set_fill(true);
  point = data -> mutable_start_point();
  point -> set_x(30);
  point -> set_y(40);
  try {
    RcfClient<Updater> client(RCF::TcpEndpoint("127.0.0.1", 10250));
    bool result = client.WriteOperationToPool(oper);
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
  bool temp = u->TransferAuth(id, "bbbb");
  EXPECT_EQ(true, temp);
}

TEST_F(UserHandlerTest, RequestAuth) {
  std::string id = m-> CreateMeeting("bcd");
  JoinMeetingReturn msg = m -> JoinMeeting(id, "cccc");
  msg = m->JoinMeeting(id, "bbbb");
  bool temp = u->RequestAuth(id, "bbbb");
  EXPECT_EQ(true, temp);
}

TEST_F(UserHandlerTest, GetCurrentUserList) {
  std::string id = m-> CreateMeeting("abc");
  JoinMeetingReturn msg = m -> JoinMeeting(id, "fl570");
  msg = m->JoinMeeting(id, "wyk");
  UserList list = u->GetCurrentUserList(id);
  User user1 = list.users(0);
  EXPECT_EQ("fl570", user1.user_name());
  EXPECT_EQ(HOST, user1.identity());
  User user2 = list.users(1);
  EXPECT_EQ("wyk", user2.user_name());
  EXPECT_EQ(PARTICIPANTS, user2.identity());
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

