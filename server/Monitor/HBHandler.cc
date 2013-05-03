/*************************************************************************
     ** File Name: HBHandler.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#include "./HBHandler.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor {
HbHandler::HbHandler() : MsgHandler() {
}

HbHandler::~HbHandler() {
}

HeartReturnPackage HbHandler::GetUserState(const HeartBeatSendPackage& h)
{
  std:: string user_name = h.user_name();
  std:: string meeting_id = h.meeting_id();
  bool temp = db_manager_->UpdateUserTime(meeting_id, user_name);
  int state = 0;
  state = db_manager_->GetUserState(meeting_id, user_name);
  HeartReturnPackage heart_return_package;
  switch(state) {
    case -1:
      LOG(ERROR)<<" Failed to GetUserState !";
    case 1:
      heart_return_package.set_identity(PARTICIPANTS);
      break;
    case 2:
      heart_return_package.set_identity(CANDIDATE);
      break;
    case 3:
      heart_return_package.set_identity(HOST);
  }
  return heart_return_package;
}
}  // Monitor
}  // server
}  // OnlineWhiteBoard
}  // Kingslanding

/*int main() {
  Kingslanding::OnlineWhiteBoard::Server::Monitor::HbHandler hbHandler;
  HeartBeatSendPackage* h = new HeartBeatSendPackage();
  h->set_meeting_id("405");
  h->set_user_name("abc");
  HeartReturnPackage heart_retrun_package =hbHandler.GetUserState(*h);
  std::cout<<heart_retrun_package.identity();
  return 0;
}*/
