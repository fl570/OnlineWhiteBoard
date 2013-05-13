/*************************************************************************
     ** File Name: UserHandler.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/

#include "./UserHandler.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace Monitor {
UserHandler::UserHandler(MeetingHandler* meeting_handler): MsgHandler()
{
Meeting_Handler = meeting_handler;
}


bool UserHandler::TransferAuth(const std::string& meeting_id, const std::string& user_name)
{
  int temp;
  bool result = true;
  temp = db_manager_->UpdateUserState(meeting_id, user_name, 4);
  if(temp != 4) {
    std::cout<<"1";
    result = false;
  }
  temp = db_manager_->UpdateUserState(meeting_id, user_name, 3);
  if(temp != 1) {
    result = false;
  }
  Meeting_Handler->TransferHostDraw(meeting_id);
  return result;
}

bool UserHandler::RequestAuth(const std::string& meeting_id, const std::string& user_name)
{
  int temp;
  bool result = true;
  temp=db_manager_->UpdateUserState(meeting_id, user_name, 2);
  if(temp == 0) {
    result =false;
  }
  return result;
}

UserList UserHandler::GetCurrentUserList(const std::string& meeting_id)
{
  UserList list;
  int  size;
  DBManager::UserInfo * res = db_manager_->GetUserList(meeting_id, size);
  list = UserListFactory(res, size);
  //LOG(ERROR) << "user list size: "<< size;
  if (res != NULL && size != 0) {
    try {
      delete []res;
    } catch (...) {
      LOG(ERROR) << "user handler delete failed";
    }
  }
  return list;
}

UserList UserHandler::UserListFactory(DBManager::UserInfo* res , int& size )
{
  UserList list;
  for(int i = 0; i < size; i++) {
    User*  usr = list.add_users();
    int state = res[i].state;
    switch(state){
      case 1:
    usr->set_identity(PARTICIPANTS);
    break;
      case 2:
    usr->set_identity(CANDIDATE);
    break;
      case 3:
    usr->set_identity(HOST);
    break;
    }
    usr->set_user_name(res[i].user_name);
  }
  return list;
}

}  // Monitor
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

/*int main() {
Kingslanding::OnlineWhiteBoard::Server::Monitor::MeetingHandler* m = new Kingslanding::OnlineWhiteBoard::Server::Monitor::MeetingHandler();
Kingslanding::OnlineWhiteBoard::Server::Monitor::UserHandler* u = new Kingslanding::OnlineWhiteBoard::Server::Monitor::UserHandler(m);
std::string meeting_id = "405";
std::string user_name = " abc";
bool temp = u->TransferAuth(meeting_id, user_name);
std::cout<<temp;
return 0;

}*/
