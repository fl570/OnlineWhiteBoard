/*************************************************************************
     ** File Name: CheckHandler.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#include "./CheckHandler.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Check {
CheckHandler::CheckHandler() : MsgHandler() {
  int size;
  db_manager_->DeleteDeadUser();
  std::string* meeting = db_manager_->GetDeadMeeting(size);
  if (NULL != meeting && size != 0) {
    try {
      delete []meeting;
    } catch (...) {
      LOG(ERROR) << "check handler delete failed";
    }
  }
}

std::string* CheckHandler::CheckHost(int& size) {
  std::string* res = db_manager_->GetDeadHostMeeting(size);
  return res;
}

std::string* CheckHandler::CheckUser(int& size) {
  db_manager_->DeleteDeadUser();
  std::string* res = db_manager_->GetDeadMeeting(size);
  return res;
}


}  // Check
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding