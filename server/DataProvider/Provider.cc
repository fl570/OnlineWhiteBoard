/*************************************************************************
     ** File Name: DataProvider.cc
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#include "./Provider.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataProvider {
ProviderImp::ProviderImp(DocumentHandler* document_handler) {
  db_instance_ = DBManager::GetInstance();
  document_handler_ = document_handler;
}

Operations ProviderImp::GetOperations(const std::string& meeting_id,
                                                 uint32_t latest_id) {
  //LOG(ERROR) << "Get Operations:" << meeting_id;
  //LOG(ERROR) << "last id: "<< latest_id;
  int64_t ret= db_instance_ ->GetDataRef(meeting_id);
  if (ret == -1) {
    Operations opers;
    return opers;
  }
  MemoryCache *mem = (MemoryCache*) ret;
  return mem->GetOperationAfter(latest_id);
}

Document ProviderImp::GetDocument(const std::string& meeting_id,
                                               uint32_t document_id) {
  return document_handler_->GetDocument(meeting_id, document_id);
}

DocumentList ProviderImp::GetHistorySnapshots(const std::string& meeting_id) {
  return document_handler_->GetHistorySnapshots(meeting_id);
}

Document ProviderImp::GetLatestDocument(const std::string& meeting_id) {
  return document_handler_->GetCurrentDocument(meeting_id);
}

ProviderImp::~ProviderImp() {
  delete document_handler_;
}
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
