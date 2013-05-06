/*************************************************************************
     ** File Name: DocumentHandlerTest.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/
#include <string>
#include "./DocumentHandler.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataProvider {
class DocumentHandlerTest: public ::testing::Test {
protected:
  DocumentHandler* d ;
  Kingslanding::OnlineWhiteBoard::Server::Monitor::MeetingHandler* m;
//   static void SetUpTestCase() {
//     google::InitGoogleLogging("LOG");
//     FLAGS_log_dir = "./LOG";
//   }
//   static void TearDownTestCase() {
//     google::ShutdownGoogleLogging();
//   }
  virtual void SetUp() {
    m = new Kingslanding::OnlineWhiteBoard::Server::Monitor::MeetingHandler();
    d = new DocumentHandler(m);
  }
  virtual void TearDown() {
    delete d;
    delete m;
  }
};

TEST_F(DocumentHandlerTest, GetDocument) {
  std::string id = m-> CreateMeeting("a");
  JoinMeetingReturn msg = m -> JoinMeeting(id, "fl570");
  msg = m->JoinMeeting(id, "wyk");
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
 Kingslanding::OnlineWhiteBoard::Server::DrawOperation::DrawOperation* draw_operation = m->GetDrawOperation(id);
  const std::string path = draw_operation->SaveAsBmp(1);
  Kingslanding::OnlineWhiteBoard::Server::DBManager::DBManager* db_manager_ = Kingslanding::OnlineWhiteBoard::Server::DBManager::DBManager::GetInstance();
  db_manager_->AddDocument(id, path);
  DocumentList list = d->GetHistorySnapshots(id);
  Document document1 = list.history_document(0);
  int document_id = document1.serial_number();
  Document document =d->GetDocument(id, document_id);
  IplImage* img;
  img = cvLoadImage(path.c_str(), 1);
  uchar* data1 = (uchar*)img->imageData;
  std::strcmp((char*)data1, document.data().c_str());
}

TEST_F(DocumentHandlerTest, GetHistorySnapshots) {
  std::string id = m-> CreateMeeting("b");
  JoinMeetingReturn msg = m -> JoinMeeting(id, "jack");
  msg = m->JoinMeeting(id, "vanvick");
  Operation oper;
  Operation_OperationData *data;
  Operation_OperationData_Point *point;
  oper.set_serial_number(1);
  data = oper.mutable_data();
  data -> set_data_type(Operation_OperationData_OperationDataType_LINE);
  data -> set_thinkness(30);
  data -> set_color(2);
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
  Kingslanding::OnlineWhiteBoard::Server::DrawOperation::DrawOperation* draw_operation = m->GetDrawOperation(id);
  const std::string path = draw_operation->SaveAsBmp(1);
  Kingslanding::OnlineWhiteBoard::Server::DBManager::DBManager* db_manager_ = Kingslanding::OnlineWhiteBoard::Server::DBManager::DBManager::GetInstance();
  db_manager_->AddDocument(id, path);
  DocumentList list = d->GetHistorySnapshots(id);
  Document document1 = list.history_document(0);
  IplImage* img;
  img = cvLoadImage(path.c_str(), 1);
  IplImage* temp;
  temp = d->zoom(img, 200, 200);
  uchar* data1 = (uchar*)temp->imageData;
  std::strcmp((char*)data1, document1.data().c_str());
}

TEST_F(DocumentHandlerTest, GetCurrentDocument) {
  std::string id = m-> CreateMeeting("c");
  JoinMeetingReturn msg = m -> JoinMeeting(id, "messi");
  msg = m->JoinMeeting(id, "ronaldo");
  Operation oper;
  Operation_OperationData *data;
  Operation_OperationData_Point *point;
  oper.set_serial_number(1);
  data = oper.mutable_data();
  data -> set_data_type(Operation_OperationData_OperationDataType_LINE);
  data -> set_thinkness(30);
  data -> set_color(3);
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
  Document document = d->GetCurrentDocument(id);
  Kingslanding::OnlineWhiteBoard::Server::DrawOperation::DrawOperation* draw_operation = m->GetDrawOperation(id);
  const std::string path = draw_operation->SaveAsBmp(1);
  Kingslanding::OnlineWhiteBoard::Server::DBManager::DBManager* db_manager_ = Kingslanding::OnlineWhiteBoard::Server::DBManager::DBManager::GetInstance();
  db_manager_->AddDocument(id, path);
  DocumentList list = d->GetHistorySnapshots(id);
  Document document1 = list.history_document(0);
  int document_id = document1.serial_number();
  Document document2 =d->GetDocument(id, document_id);
  std::strcmp(document.data().c_str(), document2.data().c_str());
  
}
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

