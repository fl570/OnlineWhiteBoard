/*************************************************************************
     ** File Name: DataProvider.cc
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#include "./DataProvider.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataProvider {
DataProviderImp::DataProviderImp(DocumentHandler* document_handler) {
  db_instance_ = DBManager::GetInstance();
  document_handler_ = document_handler;
}

Operations DataProviderImp::GetOperations(const std::string& meeting_id,
                                                                                                                 int32_t latest_id) {
  MemoryCache *mem = (MemoryCache*) db_instance_ ->GetDataRef(meeting_id);
  return mem->GetOperationAfter(latest_id);
}

Document DataProviderImp::GetDocument(const std::string& meeting_id,
                                                                                                   int32_t document_id) {
  return document_handler_->GetDocument(meeting_id, document_id);
}

DocumentList DataProviderImp::GetHistorySnapshots(const std::string& meeting_id) {
  return document_handler_->GetHistorySnapshots(meeting_id);
}

Document DataProviderImp::GetLastestDocument(const std::string& meeting_id) {
  return document_handler_->GetCurrentDocument(meeting_id);
}

DataProviderImp::~DataProviderImp() {
  delete document_handler_;
}
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
