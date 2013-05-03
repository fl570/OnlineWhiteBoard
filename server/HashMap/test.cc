#include "./segment.hpp"
#include <iostream>
#include <pthread.h>
#include <unistd.h>

void* fun(void* para){
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *n =
  (Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *) para;
  std::string id="";
  bool result;
  for (int i =0; i<100000 ; i++) {
    std::ostringstream str;
    str << i;
    id = str.str();
    n->GetValue(result,id);
    n->Insert(id,i);
    n->GetValue(result,id);
    usleep(2); 
    if(i > 0 && i % 50 == 0){
	for(int index = i-50; index < i; index++) {
 	   str << index;
           id = str.str();
 	   n->Delete(id);
	}
    }
    
    //std::cout<<id<<std::endl;
  }
  std::cout<< "complete1" <<std::endl;
  return ((void*) 0);
}
void* read(void* para){
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *n =
  (Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *) para;
  std::string id="";
  bool result;
  for (int i =0; i<100000 ; i++) {
    std::ostringstream str;
    str << i;
    id = str.str();
    n->GetValue(result,id);
    usleep(6); 
  }
  std::cout<< "read complete1" <<std::endl;
  return ((void*) 0);
}
void* fun1(void* para){
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *n =
  (Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *) para;
  std::string id="";
  bool result;
  for (int i =100000; i<200000 ; i++) {
    std::ostringstream str;
    str << i;
    id = str.str();
    n->GetValue(result,id);
    n->Insert(id,i);
    n->GetValue(result,id);
    usleep(2); 
    if(i > 100000 && i % 50 == 0){
	for(int index = i-50; index < i; index++) {
 	   str << index;
           id = str.str();
 	   n->Delete(id);
	}
    }
    
    //std::cout<<id<<std::endl;
  }
  std::cout<< "complete2" <<std::endl;
  return ((void*) 0);
}
void* read1(void* para){
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *n =
  (Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *) para;
  std::string id="";
  bool result;
  for (int i =100000; i<200000 ; i++) {
    std::ostringstream str;
    str << i;
    id = str.str();
    n->GetValue(result,id);
    usleep(6); 
  }
  std::cout<< "read complete2" <<std::endl;
  return ((void*) 0);
}
void* fun2(void* para){
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *n =
  (Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *) para;
  std::string id="";
  bool result;
  for (int i =200000; i<300000 ; i++) {
    std::ostringstream str;
    str << i;
    id = str.str();
    n->GetValue(result,id);
    n->Insert(id,i);
    n->GetValue(result,id);
    usleep(2); 
    if(i > 200000 && i % 50 == 0){
	for(int index = i-50; index < i; index++) {
 	   str << index;
           id = str.str();
 	   n->Delete(id);
	}
    }
    //std::cout<<id<<std::endl;
  }
  std::cout<< "complete3" <<std::endl;
  return ((void*) 0);
}
void* read2(void* para){
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *n =
  (Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *) para;
  std::string id="";
  bool result;
  for (int i =200000; i<300000 ; i++) {
    std::ostringstream str;
    str << i;
    id = str.str();
    n->GetValue(result,id);
    usleep(6); 
  }
  std::cout<< "read complete3" <<std::endl;
  return ((void*) 0);
}
void* fun3(void* para){
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *n =
  (Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *) para;
  std::string id="";
  bool result;
  for (int i =300000; i<400000 ; i++) {
    std::ostringstream str;
    str << i;
    id = str.str();
    n->GetValue(result,id);
    n->Insert(id,i);
    n->GetValue(result,id);
    usleep(2); 
    if(i > 300000 && i % 50 == 0){
	for(int index = i-50; index < i; index++) {
 	   str << index;
           id = str.str();
 	   n->Delete(id);
	}
    }
    
    //std::cout<<id<<std::endl;
  }
  std::cout<< "complete4" <<std::endl;
  return ((void*) 0);
}
void* read3(void* para){
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *n =
  (Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *) para;
  std::string id="";
  bool result;
  for (int i =300000; i<400000 ; i++) {
    std::ostringstream str;
    str << i;
    id = str.str();
    n->GetValue(result,id);
    usleep(6); 
  }
  std::cout<< "read complete4" <<std::endl;
  return ((void*) 0);
}

int main() {
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int> *n = new 
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int>(163);
  pthread_t p1,p2,p3,p4,r1,r2,r3,r4;
  
  pthread_create(&p1,NULL,fun,n);
  pthread_create(&p2,NULL,fun1,n);
  pthread_create(&p3,NULL,fun2,n);
  pthread_create(&p4,NULL,fun3,n);
  pthread_create(&r1,NULL,read,n);
  pthread_create(&r2,NULL,read1,n);
  pthread_create(&r3,NULL,read2,n);
  pthread_create(&r4,NULL,read3,n);
  pthread_join(p1,NULL);
  pthread_join(p2,NULL);
  pthread_join(p3,NULL);
  pthread_join(p4,NULL);
  pthread_join(r1,NULL);
  pthread_join(r2,NULL);
  pthread_join(r3,NULL);
  pthread_join(r4,NULL);
  Kingslanding::OnlineWhiteBoard::Server::HashMap::HashTable<int>::Iterator itr(n);
  bool result = itr.next();
  result = itr.next();
  delete n;
}
