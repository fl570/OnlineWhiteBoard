/*************************************************************************
     ** File Name: DataProvider.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAPROVIDER_DATAPROVIDER_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAPROVIDER_DATAPROVIDER_H_

#include "./MemoryCache.h"
#include "./DocumentHandler.h"
#include "../DBManager/DBManager.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataProvider {
class DataProviderImp {
public:
  DataProviderImp();
  virtual ~DataProviderImp(DocumentHandler*);
  Operations GetOperations(const std::string&, int32_t);
  Document GetLastestDocument(const std::string&);
  DocumentList GetHistorySnapshots(const std::string&);
  Document GetDocument(const std::string&, int32_t);
private:
  typedef Kingslanding::OnlineWhiteBoard::Server::DBManager::DBManager DBManager;
  DocumentHandler *document_handler_;
  DBManager* db_instance_;
};
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAPROVIDER_DATAPROVIDER_H_