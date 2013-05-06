/*************************************************************************
     ** File Name: Updater.cc
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#include "./Updater.h"
#include "../DBManager/DBManager.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataUpdater {
UpdaterImp::UpdaterImp(MEMCACHE* mem_cache, DRAWOP* draw_op):lastest_id_(0) {
  mem_cache_ = mem_cache;
  draw_op_ = draw_op;
}

UpdaterImp::~UpdaterImp() {
  delete mem_cache_;
  delete draw_op_;
}

bool UpdaterImp::WriteOperationToPool(const Operation& oper) {
 lastest_id_++;
 mem_cache_ -> AddOperation(oper, lastest_id_); 
 draw_op_ ->  Draw(oper, lastest_id_);
 return true;
}

bool UpdaterImp::SetDocument(const std::string& meeting_id, 
                                         uint32_t document_id) {
  lastest_id_ = 0;
  mem_cache_ -> SetState();
  DBManager::DBManager *db_instance = DBManager::DBManager::GetInstance();
  DBManager::DocumentInfo info = db_instance->GetDocument(meeting_id, document_id);
  draw_op_ -> Load(info.path);
  return true;
}

}  // DataUpdater
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding