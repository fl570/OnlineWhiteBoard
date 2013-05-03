/*************************************************************************
     ** File Name: segment.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_HASHMAP_SEGMENT_HPP_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_HASHMAP_SEGMENT_HPP_

#include "../common.h"
#include "./chain.hpp"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace HashMap {
  
template<typename Value>
class HashTable {
public:
  explicit HashTable(int size);
  ~HashTable();
  Value GetValue(bool&, const std::string&);
  bool Insert(const std::string&, const Value&);
  bool Change(const std::string&, const Value&);
  bool Delete(const std::string&);

  class Iterator {
  public:
    Iterator(HashTable<Value>*);
    bool next();
    Value GetValue();
    std::string GetKey();
  private:
    Node<Value>* current_;
    Node<Value>* next_;
    int index_;
    HashTable<Value>* table_;
  };

private:
  FORBIDDEN_EVIL_CONSTRUCTORS(HashTable);
  uint64_t HashCode(const std::string&);
#ifdef DEBUG
  FRIEND_TEST(MemoryCacheTest, AddOperation);
#endif
  Chain<Value> *array_;
  int capacity_;
};

template<typename Value>
HashTable<Value>::HashTable(int size) : capacity_(size) {
  array_ = new Chain<Value>[size];
}

template<typename Value>
HashTable<Value>::~HashTable() {
  delete []array_;
}

template<typename Value>
bool HashTable<Value>::Insert(const std::string& key, const Value& value) {
  int64_t index = HashCode(key);
  return array_[index].AddNode(key, value);
}

template<typename Value>
bool HashTable<Value>::Change(const std::string& key, const Value& value) {
  uint64_t index = HashCode(key);
  return array_[index].ChangeNode(key, value);
}

template<typename Value>
bool HashTable<Value>::Delete(const std::string& key) {
  uint64_t index = HashCode(key);
  return array_[index].DeleteNode(key);
}

template<typename Value>
Value HashTable<Value>::GetValue(bool& result, const std::string& key) {
  uint64_t index = HashCode(key);
  return array_[index].GetNodeValue(result, key);
}

template<typename Value>
uint64_t HashTable<Value>::HashCode(const std::string& key) {
  const char *tmp = key.c_str();
  uint64_t hashcode = 0;
  for (uint i =0; i < key.size(); i++) {
    hashcode = (uint64_t ) (31*hashcode + *tmp);
    tmp++;
  }
  return hashcode % capacity_;
}

template<typename Value>
Value HashTable<Value>::Iterator::GetValue() {
  return current_->Read();
}

template<typename Value>
std::string HashTable<Value>::Iterator::GetKey() {
  return current_->Key();
}

template<typename Value>
HashTable<Value>::Iterator::Iterator(HashTable<Value>* hash_table) :
index_(0), table_(hash_table), current_(hash_table->array_[0].NodeList()), next_(NULL) {}

template<typename Value>
bool HashTable<Value>::Iterator::next() {
  if (current_ != NULL && next_ != NULL) {
    current_ = next_;
    next_ = current_ -> next;
    return true;
  }
  if (current_ != NULL && next_ == NULL) {
    current_ = next_;
  }
  while (index_ < table_->capacity_-1  && current_ == NULL) {
    index_++;
    current_ = table_->array_[index_].NodeList();
  }
  if (current_ == NULL) {
    return false;
  } else {
    next_ = current_ -> next;
  }
  return true;
}
}  // HashMap
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_HASHMAP_SEGMENT_HPP_
