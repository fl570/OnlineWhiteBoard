/*************************************************************************
     ** File Name: Handler.cc
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#include "./Handler.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor {
MsgHandler::MsgHandler() {
  db_manager_ = DBMANAGER::DBManager::GetInstance();
}
MsgHandler::~MsgHandler() {}
}  // Monitor
}  // server
}  // OnlineWhiteBoard
}  // Kingslanding

