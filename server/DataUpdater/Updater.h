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
#define DRAWOP Kingslanding::OnlineWhiteBoard::Server::DrawOperation::DrawOperation
#include "../message.pb.h"
#include "../DataProvider/MemoryCache.h"
#include "../DrawOperation/DrawOperation.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataUpdater {
class UpdaterImp {
public:
  UpdaterImp(MEMCACHE *, DRAWOP*);
  ~UpdaterImp();
  bool WriteOperationToPool(const Operation&);
private:
  MEMCACHE *mem_cache_;
  DRAWOP* draw_op_;
};
}  // DataUpdater
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAUPDATER_UPDATER_H_