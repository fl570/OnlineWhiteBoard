/*************************************************************************
     ** File Name: DocumentHandler.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAPROVIDER_DOCUMENTHANDLER_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAPROVIDER_DOCUMENTHANDLER_H_

#define DBMANAGER Kingslanding::OnlineWhiteBoard::Server::DBManager

#include "../DBManager/DBManager.h"
#include "../message.pb.h"
#include "../Monitor/Handler.h"
#include "../Monitor/MeetingHandler.h"
#include "../DrawOperation/DrawOperation.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataProvider {

class DocumentHandler : public Kingslanding::OnlineWhiteBoard::Server::Monitor::MsgHandler{
public:
    DocumentHandler(MeetingHandler&);
    Document GetCurrentDocument(const std::string&);
    DocumentList GetHistorySnapshots(const std::string&);
    Document GetDocument(const std::string&, int);
    virtual ~DocumentHandler();
private:
    MeetingHandler* meeting_handler;
};
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAPROVIDER_DOCUMENTHANDLER_H_
