/*************************************************************************
     ** File Name: UserHandler.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_USERHANDLER_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_USERHANDLER_H_

#define DBMANAGER Kingslanding::OnlineWhiteBoard::Server::DBManager

#include "./Handler.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor {

class UserHandler : public MsgHandler {
public:
    UserHandler();
    virtual ~UserHandler();
    bool TransferAuth(const std::string&, const std::string&);
    bool RequestAuth(const std::string&, const std::string&);
    UserList GetCurrentUserList(const std::string&);
private:
    UserList UserListFactory(DBMANAGER::UserInfo*, int&);
};
}  // Monitor
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_USERHANDLER_H_
