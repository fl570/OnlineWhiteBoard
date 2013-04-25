/*************************************************************************
     ** File Name: HBHandler.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_HBHANDLER_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_HBHANDLER_H_

#define DBMANAGER Kingslanding::OnlineWhiteBoard::Server::DBManager

#include "./Handler.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor {

class HbHandler : public MsgHandler {
public:
    HbHandler();
    HeartReturnPackage GetUserState(HeartBeatSendPackage&);
    virtual ~HbHandler();
};
}  // Monitor
}  // server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_HBHANDLER_H_
