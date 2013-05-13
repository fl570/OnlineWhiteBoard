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
      bool AddOperation(const Operation&, unsigned int);
      bool SetState();
      Operations GetOperationAfter(unsigned int);
      Operations GetOperationFromStoreAfter(unsigned int);
      bool SetSwitch();

private:
      bool IsEmpty() const;
      bool IsFull() const;
      bool GetCount() const;
      bool AddOperationToSet(int, const Operation&, unsigned int);
      Operations GetOperationFromSet(int, unsigned int);
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
      bool changed_;
      unsigned int changed_id_;
      int front_[2];
      int rear_[2];
      int size_[2];
      unsigned int front_id_[2];
      unsigned int rear_id_[2];
      unsigned int index_;
      unsigned int switch_id_;
      boost::shared_mutex g_mutex[2];
      const Operation** operation_[2];
};
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DATAPROVIDER_MEMORYCACHE_H_
