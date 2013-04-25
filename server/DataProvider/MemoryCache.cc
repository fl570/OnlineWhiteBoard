/*************************************************************************
     ** File Name :  MemoryCache.cc
    ** Author :  fl570
    ** Mail :  cqfl570@gmail.com
    ** Created Time :  Wed Apr 10 16 : 46 : 51 2013
    **Copyright [2013]  <Copyright fl570>   [legal/copyright]
 ************************************************************************/

#include "./MemoryCache.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataProvider {

MemoryCache::MemoryCache(int capacity) {
  boost:: unique_lock<boost::shared_mutex> lock(g_mutex);
  state_ = false;
  capacity_ = capacity;
  front_[0]=0;
  front_[1]=0;
  rear_[0]=0;
  rear_[1]=0;
  size_[0]=0;
  size_[1]=0;
  front_id_[0]=0;
  front_id_[1]=0;
  rear_id_[0]=0;
  rear_id_[1]=0;

  if (capacity <= 0) {
    LOG(ERROR) << "ERROR: capacity <=0 !";
  } else {
    try {
      operation_[0] = new const Operation*[capacity_];
      operation_[1] = new const Operation*[capacity_];
    }
    catch(std::bad_alloc&) {
      LOG(ERROR) << "ERROR: cann't create operation_ set !";
    }
  }
  lock.unlock();;
}

MemoryCache::~MemoryCache() {
  boost:: unique_lock<boost::shared_mutex> lock(g_mutex);
  if (operation_[0]) {
    for (int i = 0; i < size_[0]; i++) {
      index_ = front_[0];
      delete operation_[0][index_];
      front_[0]++;
    }
    delete []operation_[0];
  }
  if (operation_[1]) {
    for (int i = 0; i < size_[1]; i++) {
      index_ = front_[1];
      delete operation_[1][index_];
      front_[1]++;
    }
    delete []operation_[1];
  }
  lock.unlock();
}

bool MemoryCache::IsEmpty() const {
    return 0 == (state_ ? size_[1] : size_[0]);
}

bool MemoryCache::GetCount() const {
    return state_ ? size_[1] : size_[0];
}

bool MemoryCache::IsFull() const {
    return capacity_ == (state_ ? size_[1] : size_[0]);
}

bool MemoryCache::SetState() {
    boost:: unique_lock<boost::shared_mutex> lock(g_mutex);
    if (state_) {
      for (int i = 0; i < size_[0]; i++) {
        index_ = front_[0];
        delete operation_[0][index_];
        front_[0]++;
      }
      rear_[0] = 0;
      front_[0] = 0;
      size_[0] = 0;
      front_id_[0] = 0;
      rear_id_[0] = 0;
    } else {
      for (int i = 0; i < size_[1]; i++) {
        index_ = front_[1];
        delete operation_[1][index_];
        front_[1]++;
      }
      rear_[1] = 0;
      front_[1] = 0;
      size_[1] = 0;
      front_id_[1] = 0;
      rear_id_[1] = 0;
    }
    state_ = state_ ? false : true;
    lock.unlock();
    return true;
}


bool MemoryCache::AddOperation(const Operation& oper) {
  if (state_) {
    AddOperationToSet(1, oper);
  } else {
    AddOperationToSet(0, oper);
  }

  return true;
}

bool MemoryCache::AddOperationToSet(int set, const Operation& oper) {
    boost:: unique_lock<boost::shared_mutex> lock(g_mutex);
    if (IsEmpty()) {
      rear_[set] = -1;
      front_id_[set] = oper.serial_number();
      rear_id_[set] = oper.serial_number();
    }
    if (IsFull()) {
      index_ = front_[set];
      rear_[set] = index_;
      delete operation_[set][index_];
      operation_[set][index_] = new Operation(oper);

      front_[set]++;
      front_[set] %= capacity_;
      index_ = front_[set];
      front_id_[set] = operation_[set][index_] -> serial_number();
    } else {
      size_[set]++;
      rear_[set]++;
      rear_[set] %= capacity_;
      index_ = rear_[set];
      operation_[set][index_] = new Operation(oper);
    }

    rear_id_[set] = oper.serial_number();
    lock.unlock();
    return true;
}

Operations MemoryCache::GetOperationAfter(int operation_id) {
  return state_ ? GetOperationFromSet(1, operation_id) :
                              GetOperationFromSet(0, operation_id);
}

Operations MemoryCache::GetOperationFromStoreAfter(int operation_id) {
  return state_ ? GetOperationFromSet(0, operation_id) :
                              GetOperationFromSet(1, operation_id);
}


Operations MemoryCache::GetOperationFromSet(int set, int operation_id) {
  boost:: unique_lock<boost::shared_mutex> lock(g_mutex);
  Operations opers;
  Operation *oper;
  if ((operation_id+1) < front_id_[set]) {
    opers.set_operation_avaliable(false);
  } else {
    opers.set_operation_avaliable(true);
    index_ = operation_id+1;
    for (int i = 0; i < rear_id_[set]-operation_id; i++) {
      oper = opers.add_operations();
      *oper = *operation_[set][index_];
      index_++;
    }
  }
  lock.unlock();
  return opers;
}
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

