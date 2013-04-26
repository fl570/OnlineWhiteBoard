/*************************************************************************
     ** File Name: Updater.cc
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#include "./Updater.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataUpdater {
UpdaterImp::UpdaterImp(MEMCACHE* mem_cache, DRAWOP* draw_op) {
  mem_cache_ = mem_cache;
  draw_op_ = draw_op;
}

UpdaterImp::~UpdaterImp() {
  delete mem_cache_;
  delete draw_op_;
}

bool UpdaterImp::WriteOperationToPool(const Operation& oper) {
 mem_cache_ -> AddOperation(oper); 
 draw_op_ ->  Draw(oper);
 return true;
}
}  // DataUpdater
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding