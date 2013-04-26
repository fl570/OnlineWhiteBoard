/*************************************************************************
     ** File Name: DBManager.h
    ** Author: fl570
    ** Mail: cqfl570@gmail.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_DBMANAGER_DBMANAGER_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_DBMANAGER_DBMANAGER_H_

#include <mysql/mysql.h>
#include <string>

#include "../common.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DBManager {

struct UserInfo {
  char user_name[50];
  int state;
};

struct DocumentInfo {
  char path[100];
  unsigned int serial_number;
};

class DBManager {
public:
    static DBManager* GetInstance();
    bool AddDocument(const std::string&, const std::string&);
    bool SetMeetingID(const std::string&, int);
    int AddMeeting();
    bool SetMeetingState(const std::string&, int);
    int AddMeetingUser(const std::string&, const std::string&, int);  // int
    // 代表当前用户的状态是否为host
    bool AddMeetingPort(const std::string&, int);
    bool DeleteDeadUser();
    int UpdateUserState(const std::string&, const std::string&, int);
    int TransferAuth(const std::string&);
    bool SetDataRef(const std::string&, int64_t);
    int GetUserState(const std::string&, const std::string&);
    bool UpdateUserTime(const std::string&, const std::string&);
    DocumentInfo GetCurrentDocument(const std::string&);
    int64_t GetDataRef(const std::string&);
    int GetMeetingPort(const std::string&);
    std::string* GetDeadMeeting(int&);  // 返回死亡的Meeting 的id
    // 并删除数据库中死亡的Meeting id
    std::string* GetDeadHostMeeting(int&);  // 返回host死亡的Meeting 的id
    DocumentInfo* GetHistoryDocuments(const std::string&, int&);
    DocumentInfo GetDocument(const std::string&, int);
    UserInfo* GetUserList(const std::string&, int&);

private:
    DBManager();
    ~DBManager();
    MYSQL* InitConnection();
    bool DestoryConnection(MYSQL *conn);

#ifdef DEBUG
    friend class DBManagerTest;
#endif

    static DBManager* db_manager_instance_;

    class Garbo {
    public:
        ~Garbo() {
            if (DBManager::db_manager_instance_ != NULL) {
                delete DBManager::db_manager_instance_;
            }
        }
    };
    static Garbo garbo_;
};
}  // DBManager
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DBMANAGER_DBMANAGER_H_
