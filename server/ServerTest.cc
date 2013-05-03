/*************************************************************************
     ** File Name: ServerTest.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#include <iostream>
#include <pthread.h>
#include <stdlib.h>

#include "./Monitor/Monitor.h"
#include "./DataProvider/Provider.h"

#include "./DataProvider/DocumentHandler.h"
#include "./Check/CheckHandler.h"
#include "./common.h"

typedef Kingslanding::OnlineWhiteBoard::Server::Monitor::MeetingHandler MeetingHandler;
typedef Kingslanding::OnlineWhiteBoard::Server::Monitor::MonitorImp MonitorImp;
typedef Kingslanding::OnlineWhiteBoard::Server::DataProvider::ProviderImp DataProviderImp;
typedef Kingslanding::OnlineWhiteBoard::Server::DataProvider::DocumentHandler DocumentHandler;
typedef Kingslanding::OnlineWhiteBoard::Server::Check::CheckHandler CheckHandler;

bool alive = true;
void* host_check_thread(void* para) {
  CheckHandler* Check_Handler = new CheckHandler();
  MeetingHandler* Meeting_Handler = (MeetingHandler*)para;
  std::string* Meeting_ID;
  int size;
  while (alive) {
    sleep(10);
    Meeting_ID = Check_Handler->CheckHost(size);
    for (int i = 0; i < size; i++) {
      Meeting_Handler->TransferHostDraw(Meeting_ID[i]);
    }
  }
  delete Check_Handler;
  return ((void*)0);
}

void* user_check_thread(void* para) {
  CheckHandler* Check_Handler = new CheckHandler();
  MeetingHandler* Meeting_Handler = (MeetingHandler*)para;
  std::string* Meeting_ID;
  int size;
  while (alive) {
    sleep(300);
    Meeting_ID = Check_Handler->CheckUser(size);
    for (int i = 0; i < size; i++) {
      Meeting_Handler->DeleteMeeting(Meeting_ID[i]);
    }
  }
  delete Check_Handler;
  return ((void*)0);
}

class ServerTest: public ::testing::Test {
protected:
  static MeetingHandler *meeting_handler;
  static MonitorImp *monitor_imp;
  static RCF::RcfServer *monitor;
  static DocumentHandler *document_handler;
  static DataProviderImp *data_provider_imp;
  static RCF::RcfServer *data_provider;
  static pthread_t host_check_tid, user_check_tid;

  static void SetUpTestCase() {
    google::InitGoogleLogging("LOG");
    FLAGS_log_dir = "./LOG";
    meeting_handler = new MeetingHandler();
  
    monitor_imp = new MonitorImp(meeting_handler);
    monitor = new RCF::RcfServer( RCF::TcpEndpoint("0.0.0.0", 10500) );
    monitor->bind<Monitor>(*monitor_imp);
    monitor->start();

    document_handler =new DocumentHandler(meeting_handler);
    data_provider_imp = new DataProviderImp(document_handler);
    data_provider = new RCF::RcfServer( RCF::TcpEndpoint("0.0.0.0", 10501) );
    data_provider->bind<Provider>(*data_provider_imp);
    data_provider->start();

    int ret = pthread_create(&host_check_tid, NULL, host_check_thread, (void*)meeting_handler);
    if (ret != 0) {
      LOG(ERROR)<<"Cannot create thread !";
    }
    int ret1 = pthread_create(&user_check_tid, NULL, user_check_thread, (void*)meeting_handler);
    if (ret1 != 0) {
      LOG(ERROR)<<"Cannot create thread !";
    }
  }
  static void TearDownTestCase() {
    alive = false;
    monitor->stop();
    data_provider->stop();
    delete data_provider;
    delete monitor;
    delete monitor_imp;
    delete data_provider_imp;
    pthread_join(host_check_tid, NULL);
    pthread_join(user_check_tid, NULL);
    google::ShutdownGoogleLogging();
  }
  virtual void SetUp() {}
  virtual void TearDown() {}
};

MeetingHandler* ServerTest::meeting_handler = NULL;
MonitorImp * ServerTest::monitor_imp = NULL;
RCF::RcfServer * ServerTest::monitor = NULL;
DocumentHandler * ServerTest::document_handler = NULL;
DataProviderImp *ServerTest::data_provider_imp = NULL;
RCF::RcfServer * ServerTest::data_provider = NULL;
pthread_t ServerTest::host_check_tid;
pthread_t ServerTest::user_check_tid;
  
TEST_F(ServerTest, TransferAuth) {
  std::string id = meeting_handler-> CreateMeeting("bcd");
  JoinMeetingReturn msg = meeting_handler -> JoinMeeting(id, "cccc");
  EXPECT_EQ(SUCCESS, msg.join_state());
  msg = meeting_handler -> JoinMeeting(id, "bbbb");
  EXPECT_EQ(SUCCESS, msg.join_state());
  
  try {
    RcfClient<Monitor> client(RCF::TcpEndpoint("127.0.0.1", 10500));
    bool result = client.TransferAuth("bbbb", id);
    EXPECT_TRUE(result);
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
}

TEST_F(ServerTest, RequestAuth) {
  std::string id = meeting_handler-> CreateMeeting("bcd");
  JoinMeetingReturn msg = meeting_handler -> JoinMeeting(id, "cccc");
  EXPECT_EQ(SUCCESS, msg.join_state());
  msg = meeting_handler -> JoinMeeting(id, "bbbb");
  EXPECT_EQ(SUCCESS, msg.join_state());
  
  try {
    RcfClient<Monitor> client(RCF::TcpEndpoint("127.0.0.1", 10500));
    bool result = client.RequestAuth("bbbb", id);
    EXPECT_TRUE(result);
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
}

TEST_F(ServerTest, CreateMeeting) {
  std::string id ="";
  try {
    RcfClient<Monitor> client(RCF::TcpEndpoint("127.0.0.1",  10500));
    id = client.CreateMeeting("bbbb");
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
  EXPECT_FALSE(id == "");
}

TEST_F(ServerTest, HeartBeat) {
  std::string id = meeting_handler -> CreateMeeting("bcd");
  JoinMeetingReturn msg = meeting_handler -> JoinMeeting(id, "fl570");
  msg = meeting_handler -> JoinMeeting(id, "wyk");
  HeartBeatSendPackage hbp1;
  hbp1.set_meeting_id(id);
  hbp1.set_user_name("fl570");
  HeartBeatSendPackage hbp2;
  hbp2.set_meeting_id(id);
  hbp2.set_user_name("wyk");
  try {
    RcfClient<Monitor> client(RCF::TcpEndpoint("127.0.0.1", 10500));
    HeartReturnPackage rt = client.HeartBeat(hbp1);
    EXPECT_EQ(HOST, rt.identity());
    rt = client.HeartBeat(hbp2);
    EXPECT_EQ(PARTICIPANTS, rt.identity());
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
}

TEST_F(ServerTest, GetCurrentUserList) {
  std::string id = meeting_handler-> CreateMeeting("abc");
  JoinMeetingReturn msg = meeting_handler -> JoinMeeting(id, "fl570");
  msg = meeting_handler->JoinMeeting(id, "wyk");
  try {
    RcfClient<Monitor> client(RCF::TcpEndpoint("127.0.0.1", 10500));
    UserList list = client.GetCurrentUserList(id);
    User user1 = list.users(0);
    EXPECT_EQ("fl570", user1.user_name());
    EXPECT_EQ(HOST, user1.identity());
    User user2 = list.users(1);
    EXPECT_EQ("wyk", user2.user_name());
    EXPECT_EQ(PARTICIPANTS, user2.identity());
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
}

TEST_F(ServerTest, ResumeUpdater) {
  std::string id = meeting_handler-> CreateMeeting("abc");
  JoinMeetingReturn msg = meeting_handler -> JoinMeeting(id, "fl570");
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
    RcfClient<Monitor> client(RCF::TcpEndpoint("127.0.0.1", 10500));
    bool result = client.ResumeUpdater(id);
    RcfClient<Updater> updater_client(RCF::TcpEndpoint("127.0.0.1",
                                                                                               msg.server_info().port()));
    for (int i=0; i < 200; i++) {
      result = updater_client.WriteOperationToPool(oper);
      EXPECT_TRUE(result);
    }
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
}

TEST_F(ServerTest, WriteOperationToPool) {
  std::string id = meeting_handler-> CreateMeeting("bcd");
  JoinMeetingReturn msg = meeting_handler -> JoinMeeting(id, "cccc");
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
    RcfClient<Updater> client(RCF::TcpEndpoint(msg.server_info().server_ip(),
                                                                                               msg.server_info().port()));
    bool result = false;
    for (int i=0; i < 200; i++) {
      result = client.WriteOperationToPool(oper);
      EXPECT_TRUE(result);
    }
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
}

TEST_F(ServerTest, GetOperations) {
  std::string id = meeting_handler-> CreateMeeting("bcd");
  JoinMeetingReturn msg = meeting_handler -> JoinMeeting(id, "cccc");
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
    RcfClient<Updater> client(RCF::TcpEndpoint(msg.server_info().server_ip(),
                                                                                               msg.server_info().port()));
    bool result = false;
    for (int i=0; i < 200; i++) {
      oper.set_serial_number(i+1);
      result = client.WriteOperationToPool(oper);
      EXPECT_TRUE(result);
    }
    RcfClient<Provider> provider_client(RCF::TcpEndpoint("127.0.0.1", 10501));
    Operations ret = provider_client.GetOperations(id, 1);
    EXPECT_FALSE(ret.operation_avaliable());
    ret = provider_client.GetOperations(id,198);
    EXPECT_TRUE(ret.operation_avaliable());
    Operation op = ret.operations(0);
    EXPECT_EQ(199, op.serial_number());
    const Operation_OperationData data = op.data();
    EXPECT_EQ(Operation_OperationData_OperationDataType_LINE,
                      data.data_type());
    const Operation_OperationData_Point point = data.start_point();
    EXPECT_EQ((30), point.x());
    EXPECT_EQ((40), point.y());
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
}


TEST_F(ServerTest, JoinMeeting) {
  std::string id = meeting_handler-> CreateMeeting("bcd");
  JoinMeetingReturn msg;
  try {
    RcfClient<Monitor> client(RCF::TcpEndpoint("127.0.0.1", 10500));
    msg = client.JoinMeeting("bbbb", id);
    EXPECT_EQ(SUCCESS, msg.join_state());
    msg = client.JoinMeeting("cccc", id);
    EXPECT_EQ(SUCCESS, msg.join_state());
    msg = client.JoinMeeting("dddd", id);
    EXPECT_EQ(SUCCESS, msg.join_state());
    msg = client.JoinMeeting("dddd", "abcde");
    EXPECT_EQ(NOTAVAILABLE, msg.join_state());
    sleep(700);
    msg = client.JoinMeeting("eeee", id);
    EXPECT_EQ(DEAD, msg.join_state());
  } catch (const RCF::Exception & e) {
    LOG(ERROR) << e.getError().getErrorString();
  }
}

int main(int argc, char **argv) {
  std::cout << "Running main() from gtest_main.cc\n";
  RCF::RcfInitDeinit rcfInit;
  google::ParseCommandLineFlags(&argc, &argv, true);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
