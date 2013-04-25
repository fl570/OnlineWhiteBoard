/*************************************************************************
     ** File Name: ServerDelegate.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/


#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAPROVIDER_MEMORYCACHE_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAPROVIDER_MEMORYCACHE_H_

#define QUEUESIZE 128

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

#include "../message.pb.h"
#include "../common.h"


namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataProvider {

class MemoryCache {
public:
      explicit MemoryCache(int capacity = QUEUESIZE);
      ~MemoryCache();

public:
      bool AddOperation(const Operation& oper);
      bool SetState();
      Operations GetOperationAfter(int operation_id);
      Operations GetOperationFromStoreAfter(int operation_id);

private:
      bool IsEmpty() const;
      bool IsFull() const;
      bool GetCount() const;
      bool AddOperationToSet(int set, const Operation& oper);
      Operations GetOperationFromSet(int set, int operation_id);
      FORBIDDEN_EVIL_CONSTRUCTORS(MemoryCache);

#ifdef DEBUG
      FRIEND_TEST(MemoryCacheTest, AddOperation);
      FRIEND_TEST(MemoryCacheTest, SetState);
      FRIEND_TEST(MemoryCacheTest, GetOperationAfter);
      FRIEND_TEST(MemoryCacheTest, GetOperationFromStoreAfter);
      FRIEND_TEST(MemoryCacheTest, Initial);
#endif
      int capacity_;
      bool state_;
      int front_[2];
      int rear_[2];
      int size_[2];
      int front_id_[2];
      int rear_id_[2];
      int index_;
      boost::shared_mutex g_mutex;
      const Operation** operation_[2];
};
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAPROVIDER_MEMORYCACHE_H_
