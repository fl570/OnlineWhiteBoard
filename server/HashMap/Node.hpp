/*************************************************************************
     ** File Name: Node.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_HASHMAP_NODE_HPP_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_HASHMAP_NODE_HPP_

#include <string>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

#include "../common.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace HashMap {
  
template<typename Value>
class Node {
public:
      explicit Node(const std::string&, const Value&);
      Value Read();
      std::string Key();
      void Del();
      bool KeyEqual(const std::string&);
      bool Write(const Value&);
      Node* next;

private:
      ~Node();
      FORBIDDEN_EVIL_CONSTRUCTORS(Node);

#ifdef DEBUG
      FRIEND_TEST(MemoryCacheTest, AddOperation);
#endif

      std::pair<const std::string, const Value> item_;
      boost::shared_mutex g_mutex_;
};

template <typename Value>
Node<Value>::Node(const std::string& key, const Value& value) : item_(key, value), next(NULL) {}


template <typename Value>
Value Node<Value>::Read() {
  g_mutex_.lock_shared();
  Value result = item_.second;
  g_mutex_.unlock_shared();
  return result;
}

template <typename Value>
bool Node<Value>::Write(const Value& value) {
  g_mutex_.lock();
  item_.second = value;
  g_mutex_.unlock();
  return true;
}

template <typename Value>
void Node<Value>::Del() {
  delete this;
}

template <typename Value>
bool Node<Value>::KeyEqual(const std::string& key) {
  return item_.first == key;
}

template <typename Value>
std::string Node<Value>::Key() {
  return item_.first;
}

template <typename Value>
Node<Value>::~Node() {
  g_mutex_.lock();
  g_mutex_.unlock();
}
}  // HashMap
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_HASHMAP_NODE_HPP_
