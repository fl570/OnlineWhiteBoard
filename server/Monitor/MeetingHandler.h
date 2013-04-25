/*************************************************************************
     ** File Name: MeetingHandler.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_MEETINGHANDLER_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_MEETINGHANDLER_H_

#define DBMANAGER Kingslanding::OnlineWhiteBoard::Server::DBManager
#define MEMCACHE Kingslanding::OnlineWhiteBoard::Server::DataProvider::MemoryCache
#define PORTMIN 10000
#define PORTMAX 10250

#include <stack>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include "tbb/concurrent_hash_map.h"
#include "./Handler.h"
#include "../DataProvider/MemoryCache.h"
#include "../RcfDefine.h"
#include "../DataUpdater/Updater.h"


namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor {
class MeetingHandler : public MsgHandler {
public:
  MeetingHandler();
  std::string CreateMeeting(const std::string&);
  JoinMeetingReturn JoinMeeting(const std::string&, const std::string&);
  bool DeleteMeeting(const std::string&);
  bool ResumeUpdater(const std::string&);
  bool TransferHostDraw(const std::string&);
  bool LogIn(const User&);
  //DrawOperation* GetDrawOperation(const std::string&);
  virtual ~MeetingHandler();
private:
  int AddMeetingPort(const std::string&);
  bool SetDataRef(const std::string&, MEMCACHE*);
  MEMCACHE* GetDataRef(const std::string&);

#ifdef DEBUG
  friend class MeetingHandlerTest;
  FRIEND_TEST(MeetingHandlerTest, AddMeetingPort);
  FRIEND_TEST(MeetingHandlerTest, SetDataRef);
  FRIEND_TEST(MeetingHandlerTest, GetDataRef);
#endif

  typedef Kingslanding::OnlineWhiteBoard::Server::DataUpdater::UpdaterImp UpdaterImpl;

  struct UpdaterInfo {
    RCF::RcfServer* server;
    UpdaterImpl* up_ref;
  };

  typedef tbb::concurrent_hash_map<std::string, UpdaterInfo*> UpdaterTable;

  boost::mutex m_stack_;
  UpdaterTable monitor_updater_;
  std::stack<int> port_;
};
}  // Monitor
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_MONITOR_MEETINGHANDLER_H_
