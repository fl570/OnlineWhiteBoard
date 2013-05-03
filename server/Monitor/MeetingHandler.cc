/*************************************************************************
     ** File Name: MeetingHandler.cc
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#include "./MeetingHandler.h"
#include <iostream>
#include "./md5.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor {
DEFINE_string(server_ip,"127.0.0.1","the ip of the server");

MeetingHandler::MeetingHandler() {
  boost:: unique_lock<boost::mutex> lock(m_stack_);
  monitor_updater_ = new UpdaterTable(163);
  for (int i = PORTMIN; i <= PORTMAX; i++) {
    port_.push(i);
  }
}

int MeetingHandler::AddMeetingPort(const std::string& meeting_id) {
  boost:: unique_lock<boost::mutex> lock(m_stack_);
  if (0 == port_.size()) {
    return 0;
  } else {
    const int ava = port_.top();
    db_manager_ -> AddMeetingPort(meeting_id, ava);
    port_.pop();
    return ava;
  }
  return 0;
}

bool MeetingHandler::LogIn(const User& user) {
  std::cerr << user.user_name() << " " << user.password() << std::endl;
  return true;
}

std::string MeetingHandler::CreateMeeting(const std::string& user_id) {
  int id = db_manager_->AddMeeting();
  std::string str_id;
  std::ostringstream ostr;
  ostr << id;
  str_id = ostr.str();
  str_id = MD5(str_id).toString();
  db_manager_->SetMeetingID(str_id, id);
  return str_id;
}

bool MeetingHandler::SetDataRef(const std::string& meeting_id, DataProvider::MemoryCache* mem_cache) {
  MEMCACHE *ref = mem_cache;
  db_manager_->SetDataRef(meeting_id, (int64_t)ref);
  return true;
}

DataProvider::MemoryCache* MeetingHandler::GetDataRef(const std::string& meeting_id) {
  int64_t ref = db_manager_->GetDataRef(meeting_id);
  if (ref != 0 || ref != -1)
    return (MEMCACHE*)ref;
  return NULL;
}

JoinMeetingReturn MeetingHandler::JoinMeeting(const std::string& meeting_id, const std::string& user_id) {
  bool has_meeting = false;
  UpdaterInfo* info = monitor_updater_->GetValue(has_meeting, meeting_id);
  int result;
  if (has_meeting) {
    result = db_manager_->AddMeetingUser(meeting_id, user_id, 1);
  } else {
    result = db_manager_->AddMeetingUser(meeting_id, user_id, 3);
  }

  if (result == 1) {
    if (!has_meeting) {
      MEMCACHE* mem =  new MEMCACHE();
      DRAWOP* draw_oper = new DRAWOP(meeting_id);
      UpdaterImpl *updater = new UpdaterImpl(mem, draw_oper);
      SetDataRef(meeting_id, mem);

      int port = AddMeetingPort(meeting_id);      
      RCF::RcfServer *server = new RCF::RcfServer( RCF::TcpEndpoint("0.0.0.0", port) );
      server->bind<Updater>(*updater);
      server->start();

      UpdaterInfo *updater_info = new UpdaterInfo;
      updater_info->server = server;
      updater_info->up_ref = updater;
      updater_info->draw_oper = draw_oper;
      monitor_updater_->Insert(meeting_id, updater_info);
    }
  }

  JoinMeetingReturn join_msg;
  switch (result) {
    case 1:
      join_msg.set_join_state(SUCCESS);
      break;
    case 0:
      join_msg.set_join_state(FAIL);
      break;
    case 2:
      join_msg.set_join_state(NOTAVAILABLE);
      break;
    case 3:
      join_msg.set_join_state(DEAD);
      break;
  }
  if (result == 1) {
    MeetingServerInfo* msg = join_msg.mutable_server_info();
    msg->set_server_ip(FLAGS_server_ip);
    msg->set_port(db_manager_->GetMeetingPort(meeting_id));
  }
  return join_msg;
}

bool MeetingHandler::DeleteMeeting(const std::string& meeting_id) {
  bool has_meeting = false;
  UpdaterInfo* info = monitor_updater_->GetValue(has_meeting, meeting_id);
  if (!has_meeting) {
    return false;
  }
  info -> server ->stop();
  int port = db_manager_->GetMeetingPort(meeting_id);
  if (port != -1 || port != 0) {
    boost:: unique_lock<boost::mutex> lock(m_stack_);
    port_.push(port);
    lock.unlock();
  }
  delete info->server;
  delete info->up_ref;
  delete info;
  monitor_updater_->Delete(meeting_id);
  return true;
}

bool MeetingHandler::ResumeUpdater(const std::string& meeting_id) {
  bool has_meeting = false;
  UpdaterInfo* info = monitor_updater_->GetValue(has_meeting, meeting_id);
  if (!has_meeting) {
    return false;
  }
  int port = db_manager_->GetMeetingPort(meeting_id);
  if (NULL == info->up_ref) {
    MEMCACHE* ref = GetDataRef(meeting_id);
    DRAWOP* draw_op = new DRAWOP(meeting_id);
    info->up_ref = new UpdaterImpl(ref, draw_op);
  }
  try {
    info -> server ->stop();
    delete info->server;
  } catch (...) {}
  info->server = new RCF::RcfServer( RCF::TcpEndpoint("0.0.0.0", port) );
  info->server -> bind<Updater>(*info -> up_ref);
  info->server -> start();
  return true;
}

bool MeetingHandler::TransferHostDraw(const std::string& meeting_id) {
  bool has_meeting = false;
  UpdaterInfo* info = monitor_updater_->GetValue(has_meeting, meeting_id);
  if (!has_meeting) {
    return false;
  }
  std::string path = info->draw_oper->SaveAsBmp();
  if(path == "")
    return false;
  db_manager_ -> AddDocument(meeting_id, path);
  return true;
}

DRAWOP* MeetingHandler::GetDrawOperation(const std::string& meeting_id) {
  bool has_meeting = false;
  UpdaterInfo* info = monitor_updater_->GetValue(has_meeting, meeting_id);
  if (!has_meeting) {
    return NULL;
  }
  return info -> draw_oper;
}


MeetingHandler::~MeetingHandler() {
  UpdaterTable::Iterator itr(monitor_updater_);
  while (itr.next()) {
    std::string key = itr.GetKey();
    DeleteMeeting(key);
  }
  delete monitor_updater_;
}

}  // Monitor
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding