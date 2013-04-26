/*************************************************************************
     ** File Name: Monitor.cc
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#include "./Monitor.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor {
MonitorImp::MonitorImp(MeetingHandler* meeting_handler) {
  meeting_handler_ = meeting_handler;
}

MonitorImp::~MonitorImp() {
  delete meeting_handler_;
}

std::string MonitorImp::CreatMeeting(const std::string& meeting_id) {
  return meeting_handler_ -> CreateMeeting(meeting_id);
}

JoinMeetingReturn MonitorImp::JoinMeeting(const std::string& user_id,
                                                         const std::string& meeting_id) {
  return meeting_handler_ -> JoinMeeting(meeting_id, user_id);
}

bool MonitorImp::Login(const User& user) {
  return meeting_handler_ -> LogIn(user);
}

bool MonitorImp::ResumeUpdater(const std::string& meeting_id) {
  return meeting_handler_ -> ResumeUpdater(meeting_id);
}

bool MonitorImp::RequestAuth(const std::string& user_id, const std::string& meeting_id) {
  return user_handler_.RequestAuth(meeting_id, user_id);
}

int32_t MonitorImp::TransferAuth(const std::string& user_id, const std::string& meeting_id) {
  return user_handler_.TransferAuth(meeting_id, user_id);
}

HeartReturnPackage MonitorImp::HeartBeat(const HeartBeatSendPackage& msg) {
  return hb_handler_.GetUserState(msg);
}

UserList MonitorImp::GetCurrentUserList(const std::string& meeting_id) {
  return user_handler_.GetCurrentUserList(meeting_id);
}
}  // Monitor
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
