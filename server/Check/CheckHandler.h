/*************************************************************************
     ** File Name: CheckHandler.h
    ** Author: vanvick
    ** Mail: vanvick@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_CHECK_CHECKHANDLER_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_CHECK_CHECKHANDLER_H_

#define DBMANAGER Kingslanding::OnlineWhiteBoard::Server::DBManager

#include "./../Monitor//Handler.h"
#include "../common.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Check {

class CheckHandler : public Kingslanding::OnlineWhiteBoard::Server::Monitor::MsgHandler {
public:
    CheckHandler();
    virtual ~CheckHandler(){};
    std::string* CheckHost(int &);
    std::string* CheckUser(int &);
};
}  // Check
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_CHECK_CHECKHANDLER_H_