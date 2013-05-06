/*************************************************************************
     ** File Name: Server.cc
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include "./DataProvider/Provider.h"
#include "./Monitor/Monitor.h"
#include "./Monitor/MeetingHandler.h"
#include "./DataProvider/DocumentHandler.h"
#include "./Check/CheckHandler.h"

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
    if (Meeting_ID != NULL)
      delete []Meeting_ID;
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
    if (Meeting_ID != NULL)
      delete []Meeting_ID;
  }
  delete Check_Handler;
  return ((void*)0);
}

int main(int argc, char **argv) {
  RCF::RcfInitDeinit rcfInit;
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging("LOG");
  FLAGS_log_dir = "./LOG";
  MeetingHandler *meeting_handler = new MeetingHandler();
  
  MonitorImp *monitor_imp = new MonitorImp(meeting_handler);
  RCF::RcfServer *monitor = new RCF::RcfServer( RCF::TcpEndpoint("0.0.0.0", 10500) );
  monitor->bind<Monitor>(*monitor_imp);
  RCF::ThreadPoolPtr monitor_tp_ptr( new RCF::ThreadPool(100) );
  monitor->setThreadPool(monitor_tp_ptr);
  monitor->start();

  DocumentHandler *document_handler =new DocumentHandler(meeting_handler);
  DataProviderImp *data_provider_imp = new DataProviderImp(document_handler);
  RCF::RcfServer *data_provider = new RCF::RcfServer( RCF::TcpEndpoint("0.0.0.0", 10501) );
  data_provider->bind<Provider>(*data_provider_imp);
  RCF::ThreadPoolPtr updater_tp_ptr( new RCF::ThreadPool(50) );
  monitor->setThreadPool(updater_tp_ptr);
  data_provider->start();

  pthread_t host_check_tid, user_check_tid;
  int ret = pthread_create(&host_check_tid, NULL, host_check_thread, (void*)meeting_handler);
  if (ret != 0) {
    LOG(ERROR)<<"Cannot create thread !";
  }
  int ret1 = pthread_create(&user_check_tid, NULL, user_check_thread, (void*)meeting_handler);
   if (ret1 != 0) {
    LOG(ERROR)<<"Cannot create thread !";
  }
  
  std::cout << "server start"<< std::endl;
  
  std::string in;
  while (1) {
    std::cin >> in;
    if ("quit" == in) {
      break;
    }
  }

  alive = false;
  monitor->stop();
  data_provider->stop();
  pthread_join(host_check_tid, NULL);
  pthread_join(user_check_tid, NULL);
  delete data_provider;
  delete monitor;
  delete monitor_imp;
  delete data_provider_imp;
  std::cout << "server stoped"<<std::endl;
  google::ShutdownGoogleLogging();
  return 0;
}