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

bool MeetingHandler::LogIn(const User& ) {
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
  UpdaterTable::const_accessor read_acc;
  bool has_meeting = monitor_updater_.find(read_acc,meeting_id);
  read_acc.release();
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
      UpdaterTable::accessor write_acc;
      monitor_updater_.insert(write_acc, meeting_id);
      write_acc->second = updater_info;
      write_acc.release();
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
  UpdaterTable::accessor write_acc;
  bool has_meeting = monitor_updater_.find(write_acc, meeting_id);
  if (!has_meeting) {
    return false;
  }
  write_acc->second->server->stop();
  int port = db_manager_->GetMeetingPort(meeting_id);
  if (port != -1 || port != 0) {
    boost:: unique_lock<boost::mutex> lock(m_stack_);
    port_.push(port);
    lock.unlock();
  }
  delete write_acc->second->server;
  delete write_acc->second->up_ref;
  delete write_acc->second;
  monitor_updater_.erase(write_acc);
  return true;
}

bool MeetingHandler::ResumeUpdater(const std::string& meeting_id) {
  UpdaterTable::accessor write_acc;
  bool has_meeting = monitor_updater_.find(write_acc, meeting_id);
  LOG(INFO) << has_meeting;
  if (!has_meeting) {
    return false;
  }
  int port = db_manager_->GetMeetingPort(meeting_id);
  if (NULL == write_acc->second->up_ref) {
    MEMCACHE* ref = GetDataRef(meeting_id);
    DRAWOP* draw_op = new DRAWOP(meeting_id);
    write_acc->second->up_ref = new UpdaterImpl(ref, draw_op);
  }
  try {
    write_acc ->second -> server ->stop();
    delete write_acc->second->server;
  } catch (...) {}
  write_acc->second->server = new RCF::RcfServer( RCF::TcpEndpoint("0.0.0.0", port) );
  write_acc->second->server -> bind<Updater>(*write_acc -> second -> up_ref);
  write_acc->second->server -> start();
  return true;
}

bool MeetingHandler::TransferHostDraw(const std::string& meeting_id) {
  UpdaterTable::const_accessor read_acc;
  bool has_meeting = monitor_updater_.find(read_acc, meeting_id);
  if (!has_meeting) {
    return false;
  }
  std::string path = read_acc->second->draw_oper->SaveAsBmp();
  read_acc.release();
  if(path == "")
    return false;
  db_manager_ -> AddDocument(meeting_id, path);
  return true;
}

DRAWOP* MeetingHandler::GetDrawOperation(const std::string& meeting_id) {
  UpdaterTable::const_accessor read_acc;
  bool has_meeting = monitor_updater_.find(read_acc, meeting_id);
  if (!has_meeting) {
    return NULL;
  }
  return read_acc -> second -> draw_oper;
}


MeetingHandler::~MeetingHandler() {
  UpdaterTable::iterator iter;
  iter =  monitor_updater_.begin();
  for (; iter != monitor_updater_.end(); iter++) {
    DeleteMeeting(iter -> first);
  }
}

}  // Monitor
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
