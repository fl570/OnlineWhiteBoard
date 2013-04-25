/*************************************************************************
     ** File Name :  MeetingHandlerTest.cc
    ** Author :  fl570
    ** Mail :  cqfl570@gmail.com
    ** Created Time :  Wed Apr 10 16 : 46 : 51 2013
    **Copyright [2013]  <Copyright fl570>   [legal/copyright]
 ************************************************************************/

#include <iostream>
#include <time.h>
#include "./MeetingHandler.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor  {

typedef Kingslanding::OnlineWhiteBoard::Server::DBManager::DBManager DBM;

class MeetingHandlerTest: public ::testing::Test {
protected:
  MeetingHandler *meeting_instance ;
  std::string str_id;
  static DBM *db_instance ;
  MEMCACHE* memory;
  static void SetUpTestCase() {
    google::InitGoogleLogging("LOG");
    FLAGS_log_dir = "../LOG";
  }
  static void TearDownTestCase() {
    google::ShutdownGoogleLogging();
  }
  virtual void SetUp() {
    db_instance ->DeleteDeadUser();
    int size;
    db_instance ->GetDeadMeeting(size);
    meeting_instance = new MeetingHandler();
    meeting_instance -> port_.pop();
    int id = db_instance -> AddMeeting();
    std::ostringstream str;
    str << id;
    str_id = str.str();
    db_instance -> SetMeetingID(str_id, id);
    MeetingHandler::UpdaterInfo *info = new MeetingHandler::UpdaterInfo;
    memory = new MEMCACHE();
    info -> server = new RCF::RcfServer(RCF::TcpEndpoint("0.0.0.0", 10250));
    info -> up_ref = new MeetingHandler::UpdaterImpl(memory);
    db_instance ->SetDataRef(str_id, (int64_t) memory);
    MeetingHandler::UpdaterTable::accessor write_acc;
    meeting_instance -> monitor_updater_.insert(write_acc, str_id);
    write_acc->second = info;
    write_acc.release();
    db_instance -> AddMeetingPort(str_id, 10250);
  }
  virtual void TearDown() {
    delete meeting_instance;
  }
};

DBM* MeetingHandlerTest::db_instance = DBM::GetInstance();

TEST_F(MeetingHandlerTest, AddMeetingPort) {
  EXPECT_EQ(10249, meeting_instance -> AddMeetingPort(str_id));
  EXPECT_EQ(10249, db_instance -> GetMeetingPort(str_id));
}

TEST_F(MeetingHandlerTest, SetDataRef) {
  MEMCACHE* mem = new MEMCACHE();
  EXPECT_TRUE(meeting_instance -> SetDataRef(str_id, mem));
  MEMCACHE* copy = (MEMCACHE*) db_instance -> GetDataRef(str_id);
  EXPECT_TRUE(copy == mem);
  delete mem;
}

TEST_F(MeetingHandlerTest, GetDataRef) {
  MEMCACHE* mem = meeting_instance ->GetDataRef(str_id);
  EXPECT_TRUE(memory == mem);
  EXPECT_TRUE(mem ->SetState());
}

TEST_F(MeetingHandlerTest, CreateMeeting) {
  std::string meeting_id = meeting_instance -> CreateMeeting("abd");
  EXPECT_EQ(0, db_instance -> GetMeetingPort(meeting_id));
}

TEST_F(MeetingHandlerTest, JoinMeeting) {
  JoinMeetingReturn msg = meeting_instance -> JoinMeeting(str_id, "abc");
  EXPECT_EQ(1, db_instance -> GetUserState(str_id, "abc"));
  EXPECT_EQ(SUCCESS, msg.join_state());
  EXPECT_EQ(0, strcmp("127.0.0.1", msg.server_info().server_ip().c_str()));
  EXPECT_EQ(10250, msg.server_info().port());

  msg = meeting_instance -> JoinMeeting("123", "abcd");
  EXPECT_EQ(0, db_instance -> GetUserState(str_id, "abcd"));
  EXPECT_EQ(NOTAVAILABLE, msg.join_state());
  
  std::string id = meeting_instance -> CreateMeeting("bcd");
  msg = meeting_instance -> JoinMeeting(id, "cccc");
  EXPECT_EQ(3, db_instance -> GetUserState(id, "cccc"));
  EXPECT_EQ(SUCCESS, msg.join_state());
  EXPECT_EQ(0, strcmp("127.0.0.1", msg.server_info().server_ip().c_str()));
  EXPECT_EQ(10249, msg.server_info().port());

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
   RcfClient<Updater> client(RCF::TcpEndpoint("127.0.0.1", 10249));
   bool result = client.WriteOperationToPool(oper);
   EXPECT_TRUE(result);
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
}

TEST_F(MeetingHandlerTest, DeleteMeeting) {
  EXPECT_TRUE(meeting_instance -> DeleteMeeting(str_id));
}

TEST_F(MeetingHandlerTest, ResumeUpdater) {
  EXPECT_TRUE(meeting_instance -> ResumeUpdater(str_id));
  LOG(INFO) << "UpdaterResume Complete";
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
    RcfClient< Updater > client(RCF::TcpEndpoint("127.0.0.1", 10250));
    EXPECT_TRUE(client.WriteOperationToPool(oper));
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
}
}  // Monitor
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

int main(int argc, char **argv) {
  std::cout << "Running main() from gtest_main.cc\n";
  RCF::RcfInitDeinit rcfInit;
  google::ParseCommandLineFlags(&argc, &argv, true);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
