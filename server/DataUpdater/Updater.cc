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
UpdaterImp::UpdaterImp(MEMCACHE* mem_cache, DRAWOP* draw_op):lastest_id_(0),
                                                       new_operation_(false) {
  mem_cache_ = mem_cache;
  draw_op_ = draw_op;
}

UpdaterImp::~UpdaterImp() {
  delete mem_cache_;
  delete draw_op_;
}

bool UpdaterImp::WriteOperationToPool(const Operation& oper) {
 lastest_id_++;
 if (!new_operation_) {
   new_operation_ = true;
 }
 mem_cache_ -> AddOperation(oper, lastest_id_); 
 draw_op_ ->  Draw(oper, lastest_id_);
 return true;
}

bool UpdaterImp::SetDocument(const std::string& meeting_id, 
                                         uint32_t document_id) {
  LOG(ERROR) << "set document";
  lastest_id_++;
  DBManager::DBManager *db_instance = DBManager::DBManager::GetInstance();
  if (new_operation_) {
    DrawOperation::PathInfo info = draw_op_->SaveAsBmp(1);
    db_instance->AddDocument(meeting_id, info.path);
    new_operation_ = false;
  }
  mem_cache_ -> SetSwitch();
  DBManager::DocumentInfo info = db_instance->GetDocument(meeting_id, document_id);
  draw_op_ -> Load(info.path, lastest_id_);
  return true;
}

}  // DataUpdater
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
