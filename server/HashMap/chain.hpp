/*************************************************************************
     ** File Name: chain.hpp
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_HASHMAP_CHAIN_HPP_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_HASHMAP_CHAIN_HPP_

#include "../common.h"
#include "./Node.hpp"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace HashMap {
  
template <typename Value>
class Chain {
public:
  bool AddNode(const std::string&, const Value&);
  bool DeleteNode(const std::string&);
  Value GetNodeValue(bool& reuslt, const std::string&);
  bool ChangeNode(const std::string&, const Value&);
  Node<Value>* NodeList();
  Chain();
  ~Chain();
  
private:
  FORBIDDEN_EVIL_CONSTRUCTORS(Chain);

#ifdef DEBUG
  FRIEND_TEST(MemoryCacheTest, AddOperation);
#endif
  Node<Value>* node_list_;
  boost::shared_mutex g_mutex_;
};

template <typename Value>
bool Chain<Value>::AddNode(const std::string& key, const Value& value) {
  g_mutex_.lock();
  Node<Value>* tmp = new Node<Value>(key, value);
  if (node_list_ == NULL) {
    node_list_ = tmp;
  } else {
    tmp -> next = node_list_ -> next;
    node_list_ = tmp;
  }
  g_mutex_.unlock();
  return true;
}

template <typename Value>
bool Chain<Value>::DeleteNode(const std::string& key) {
  g_mutex_.lock_shared();
  Node<Value>* tmp = node_list_;
  if (tmp == NULL) {
    g_mutex_.unlock_shared();
    return false;
  }
  if (tmp -> KeyEqual(key)) {
    g_mutex_.unlock_shared();
    g_mutex_.lock();
    tmp -> Del();
    node_list_ = NULL;
    g_mutex_.unlock();
    return true;
  }
  while (tmp -> next != NULL && !tmp -> next -> KeyEqual(key) ) {
    tmp = tmp -> next;
  }
  if (tmp -> next == NULL) {
    g_mutex_.unlock_shared();
    return false;
  }
  g_mutex_.unlock_shared();
  g_mutex_.lock();
  Node<Value>* temp = tmp -> next;
  tmp -> next = temp -> next;
  g_mutex_.unlock();
  temp -> Del();
  return true;
}

template <typename Value>
bool Chain<Value>::ChangeNode(const std::string& key, const Value& value) {
  g_mutex_.lock_shared();
  Node<Value>* tmp = node_list_;
  if (tmp == NULL) {
    g_mutex_.unlock_shared();
    return false;
  }
  if (tmp -> KeyEqual(key)) {
    g_mutex_.unlock_shared();
    tmp -> Write(value);
    return true;
  }
  while (tmp -> next != NULL && !tmp -> next -> KeyEqual(key) ) {
    tmp = tmp -> next;
  }
  if (tmp -> next == NULL) {
    g_mutex_.unlock_shared();
    return false;
  }
  g_mutex_.unlock_shared();
  tmp -> Write(value);
  return true;
}

template <typename Value>
Value Chain<Value>::GetNodeValue(bool& result, const std::string& key) {
  g_mutex_.lock_shared();
  Node<Value>* tmp = node_list_;
  while (tmp  != NULL && !tmp -> KeyEqual(key) ) {
    tmp = tmp -> next;
  }
  if (tmp == NULL) {
    g_mutex_.unlock_shared();
    result = false;
    return (Value)NULL;
  }
  g_mutex_.unlock_shared();
  result = true;
  return tmp -> Read();
}

template <typename Value>
Node<Value>* Chain<Value>::NodeList() {
  return node_list_;
}

template <typename Value>
Chain<Value>::~Chain() {
  g_mutex_.lock();
  Node<Value>* tmp;
  while (node_list_  != NULL) {
    tmp = node_list_ -> next;
    node_list_ -> Del();
    node_list_ = tmp;
  }
  g_mutex_.unlock();
}

template <typename Value>
Chain<Value>::Chain() : node_list_(NULL) {}
}  // HashMap
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_HASHMAP_CHAIN_HPP_
