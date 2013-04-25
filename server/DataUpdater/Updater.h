/*************************************************************************
     ** File Name: Updater.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAUPDATER_UPDATER_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAUPDATER_UPDATER_H_

#define MEMCACHE Kingslanding::OnlineWhiteBoard::Server::DataProvider::MemoryCache
#include "../message.pb.h"
#include "../DataProvider/MemoryCache.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataUpdater {
class UpdaterImp {
public:
  UpdaterImp(MEMCACHE *);
  ~UpdaterImp();
  bool WriteOperationToPool(const Operation&);
private:
  MEMCACHE *mem_cache_;
  //DrawOp draw_op;
};
}  // DataUpdater
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAUPDATER_UPDATER_H_