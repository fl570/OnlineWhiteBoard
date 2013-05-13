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
    changed_ = false;
    capacity_ = capacity;
    changed_id_ = 0;
    switch_id_ = 0;
    front_[0] = 0;
    front_[1] = 0;
    rear_[0] = 0;
    rear_[1] = 0;
    size_[0] = 0;
    size_[1] = 0;
    front_id_[0] = 0;
    front_id_[1] = 0;
    rear_id_[0] = 0;
    rear_id_[1] = 0;

    if (capacity <= 0) {
        LOG(ERROR) << "ERROR: capacity <=0 !";
    } else {
        try {
            operation_[0] = new const Operation*[capacity_];
            operation_[1] = new const Operation*[capacity_];
        }
        catch (std::bad_alloc&) {
            LOG(ERROR) << "ERROR: cann't create operation_ set !";
        }
    }
    lock.unlock();;
}

MemoryCache::~MemoryCache() {
    boost:: unique_lock<boost::shared_mutex> lock(g_mutex);
    if (operation_[0] != NULL) {
        for (int i = 0; i < size_[0]; i++) {
            index_ = front_[0];
            delete operation_[0][index_];
            front_[0]++;
            front_[0] %= capacity_;
        }
        delete []operation_[0];
    }
    if (operation_[1] != NULL) {
        for (int i = 0; i < size_[1]; i++) {
            index_ = front_[1];
            delete operation_[1][index_];
            front_[1]++;
            front_[0] %= capacity_;
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
        changed_id_ = rear_id_[1];
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
        changed_id_ = rear_id_[0];
    }
    state_ = state_ ? false : true;
    changed_ = true;
    lock.unlock();
    return true;
}


bool MemoryCache::AddOperation(const Operation& oper,
                               unsigned int latest_id) {
    if (state_) {
        AddOperationToSet(1, oper, latest_id);
    } else {
        AddOperationToSet(0, oper, latest_id);
    }
    return true;
}

bool MemoryCache::AddOperationToSet(int set, const Operation& oper,
                                    unsigned int lastest_id) {
    boost:: unique_lock<boost::shared_mutex> lock(g_mutex);
    if (IsEmpty()) {
        rear_[set] = -1;
        front_id_[set] = lastest_id;
        rear_id_[set] = lastest_id;
    }
    if (IsFull()) {
        index_ = front_[set];
        rear_[set] = index_;
        delete operation_[set][index_];
        Operation *tmp = new Operation(oper);
        tmp -> set_serial_number(lastest_id);
        operation_[set][index_] = tmp;

        front_[set]++;
        front_[set] %= capacity_;
        index_ = front_[set];
        front_id_[set] = operation_[set][index_] -> serial_number();

        //LOG(ERROR) << tmp -> serial_number();
        //LOG(ERROR) << tmp -> data().data_type();
    } else {
        size_[set]++;
        rear_[set]++;
        rear_[set] %= capacity_;
        index_ = rear_[set];
        Operation *tmp = new Operation(oper);
        tmp -> set_serial_number(lastest_id);
        operation_[set][index_] = tmp;

        //LOG(ERROR) << tmp -> serial_number();
        //LOG(ERROR) << tmp -> data().data_type();
    }
    rear_id_[set] = lastest_id;
    return true;
}

Operations MemoryCache::GetOperationAfter(unsigned int operation_id) {
    Operations result;
    if (0 != switch_id_ && operation_id <= switch_id_) {
        LOG(ERROR) << "load document";
        result.set_operation_avaliable(Operations_OperationAvaliable_LOAD_DOCUMENT);
    } else {
        if (state_) {
            if (changed_ && size_[1] > QUEUESIZE/2) {
                changed_ = false;
            }
            if (changed_ && operation_id < changed_id_) {
                result = GetOperationFromSet(0, operation_id);
            } else {
                result = GetOperationFromSet(1, operation_id);
            }
        } else {
            if (changed_ && size_[0] > QUEUESIZE/2) {
                changed_ = false;
            }
            if (changed_ && operation_id < changed_id_) {
                result = GetOperationFromSet(1, operation_id);
            } else {
                result = GetOperationFromSet(0, operation_id);
            }
        }
    }
    //LOG(ERROR) << "operation_avaliable: " << result.operation_avaliable();
    return result;
}

Operations MemoryCache::GetOperationFromStoreAfter(unsigned int operation_id) {
    return state_ ? GetOperationFromSet(0, operation_id) :
           GetOperationFromSet(1, operation_id);
}


Operations MemoryCache::GetOperationFromSet(int set,
        unsigned int operation_id) {
    //LOG(ERROR) << "get operation from set:" << set;
    if ((state_ && set == 1) || (!state_ && set == 0))
        boost::shared_lock<boost::shared_mutex> lock(g_mutex);
    Operations opers;
    Operation *oper;
    if ((operation_id+1) < front_id_[set]) {
        opers.set_operation_avaliable(Operations_OperationAvaliable_NOT_AVALIABLE);
    } else {
        opers.set_operation_avaliable(Operations_OperationAvaliable_AVALIBLE);
        index_ = operation_id + 1 - front_id_[set] + front_[set];
        index_ %= capacity_;
        for (unsigned int i = 0; i < rear_id_[set]-operation_id; i++) {
            oper = opers.add_operations();
            *oper = *operation_[set][index_];
            index_++;
            index_ %= capacity_;
        }
    }
    //LOG(ERROR) << opers.operations_size();
    return opers;
}

bool MemoryCache::SetSwitch() {
    boost:: unique_lock<boost::shared_mutex> lock(g_mutex);
    switch_id_ = state_ ? rear_id_[1] : rear_id_[0];
    lock.unlock();
    return true;
}
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

