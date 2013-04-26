/*************************************************************************
     ** File Name: Monitor.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_MONITOR_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_MONITOR_H_

#include "./Handler.h"
#include "./MeetingHandler.h"
#include "./HBHandler.h"
#include "./UserHandler.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor {
class MonitorImp {
public:
  MonitorImp(MeetingHandler*);
  virtual ~MonitorImp();
  bool Login(const User&);
  std::string CreatMeeting(const std::string&);
  JoinMeetingReturn JoinMeeting(const std::string&, const std::string&);
  int32_t TransferAuth(const std::string&, const std::string&);
  bool RequestAuth(const std::string&, const std::string&);
  UserList GetCurrentUserList(const std::string&);
  HeartReturnPackage HeartBeat(const HeartBeatSendPackage&);
  bool ResumeUpdater(const std::string&);
private:
  MeetingHandler *meeting_handler_;
  HbHandler hb_handler_;
  UserHandler user_handler_;
};
}  // Monitor
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_MONITOR_H_