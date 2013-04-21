/*************************************************************************
     ** File Name :  DBManager.cc
    ** Author :  fl570
    ** Mail :  cqfl570@gmail.com
    ** Created Time :  Wed Apr 10 16 : 46 : 51 2013
    **Copyright [2013]  <Copyright fl570>   [legal/copyright]
 ************************************************************************/

#include "./DBManager.h"
#include <stdio.h>
#include <time.h>

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DBManager {

DEFINE_string(mysql_server_ip, "127.0.0.1", "the ip address of mysql server");
DEFINE_string(mysql_user, "root", "the user of mysql server");
DEFINE_string(mysql_password, "570", "the password of mysql server");
DEFINE_string(mysql_database, "OWB", "the database of mysql server");

DBManager* DBManager::db_manager_instance_ = NULL;

static char *server_args[] = {
  "this_program",       /* this string is not used */
  "--datadir=.",
  "--key_buffer_size=32M"
};
static char *server_groups[] = {
  "embedded",
  "server",
  "this_program_SERVER",
  reinterpret_cast<char *>(NULL)
};

MYSQL* DBManager::InitConnection() {
  if (mysql_server_init(sizeof(server_args) / sizeof(char *),
  server_args, server_groups)) {
    LOG(ERROR) << "cannot init mysql server";
    exit(1);
  }
  MYSQL *conn;
  conn = mysql_init(NULL);
  /* Connect to database */
  if (!mysql_real_connect(conn, FLAGS_mysql_server_ip.c_str(),
        FLAGS_mysql_user.c_str(), FLAGS_mysql_password.c_str(),
                     FLAGS_mysql_database.c_str(), 0, NULL, 0)) {
     LOG(ERROR) << mysql_error(conn);
     exit(1);
  }
  return conn;
}

bool DBManager::DestoryConnection(MYSQL *conn) {
  // close connection
  mysql_close(conn);
  // Use any MySQL API functions here
  mysql_server_end();
  return true;
}

DBManager* DBManager::GetInstance() {
    if (NULL == db_manager_instance_) {
      db_manager_instance_ = new DBManager();
    }
    return db_manager_instance_;
}

DBManager::DBManager::DBManager() {}
DBManager::~DBManager() {}

int DBManager::AddMeeting() {
  MYSQL *conn;
  conn = InitConnection();
  std::string sql = "insert into Meeting (state) values (0)";
  int res = -1;
  if (mysql_query(conn, sql.c_str())) {
    LOG(ERROR) << mysql_error(conn);
  } else {
      res = mysql_insert_id(conn);
  }
  DestoryConnection(conn);
  return res;
}

bool DBManager::SetMeetingID(const std::string& meeting_id, int id) {
  MYSQL *conn;
  bool result;
  char sql[500];
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "update Meeting set MeetingID = '%s' where \
  id = %d", meeting_id.c_str(), id);
  if (mysql_query(conn, sql)) {
    LOG(ERROR) << mysql_error(conn);
    result = false;
  } else {
    result = true;
  }
  DestoryConnection(conn);
  return result;
}

bool DBManager::AddMeetingPort(const std::string& meeting_id, int port) {
  char sql[500];
  bool result;
  MYSQL *conn;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "update Meeting set Port = %d where \
  MeetingID = '%s'", port, meeting_id.c_str());
  if (mysql_query(conn, sql)) {
    LOG(ERROR) << mysql_error(conn);
    result = false;
  } else {
    result = true;
  }
  DestoryConnection(conn);
  return result;
}

int DBManager::GetMeetingPort(const std::string& meeting_id) {
  char sql[500];
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  conn = InitConnection();
  int res = -1;
  snprintf(sql, sizeof(sql), "select Port from Meeting where \
  MeetingID = '%s'", meeting_id.c_str());
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
  } else {
    res_ = mysql_store_result(conn);
    if (0 != mysql_num_rows(res_)) {
      row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%d", &res);
    }
    mysql_free_result(res_);
  }
  DestoryConnection(conn);
  return res;
}

bool DBManager::SetMeetingState(const std::string& meeting_id, int state) {
  char sql[500];
  bool result;
  MYSQL *conn;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "update Meeting set state = %d where \
  MeetingID = '%s'", state, meeting_id.c_str());
  if (mysql_query(conn, sql)) {
    LOG(ERROR) << mysql_error(conn);
    result = false;
  } else {
    result = true;
  }
  DestoryConnection(conn);
  return result;
}

bool DBManager::SetDataRef(const std::string& meeting_id, int data_ref) {
  char sql[500];
  bool result;
  MYSQL *conn;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "update Meeting set DataRef = %d where \
  MeetingID = '%s'", data_ref, meeting_id.c_str());
  if (mysql_query(conn, sql)) {
    LOG(ERROR) << mysql_error(conn);
    result = false;
  } else {
    result = true;
  }
  DestoryConnection(conn);
  return result;
}

int DBManager::GetDataRef(const std::string& meeting_id) {
  char sql[500];
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  int res = -1;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "select DataRef from Meeting where \
  MeetingID = '%s'", meeting_id.c_str());
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
  } else {
    res_ = mysql_store_result(conn);
    if (0 != mysql_num_rows(res_)) {
      row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%d", &res);
    }
    mysql_free_result(res_);
  }
  DestoryConnection(conn);
  return res;
}

bool DBManager::AddDocument(const std::string& meeting_id,
                                     const std::string& path) {
  char sql[500];
  bool result;
  MYSQL *conn;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "insert into MeetingDocument (MeetingID, path) \
  values ('%s','%s')", meeting_id.c_str(), path.c_str());
  if (mysql_query(conn, sql)) {
    LOG(ERROR) << mysql_error(conn);
    result = false;
  } else {
    result = true;
  }
  DestoryConnection(conn);
  return result;
}

DocumentInfo DBManager::GetCurrentDocument(const std::string& meeting_id) {
  DocumentInfo res;
  char sql[500];
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "SELECT max(DocumentID), path FROM \
  MeetingDocument where MeetingID = '%s'", meeting_id.c_str());
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
     res.serial_number = -1;
  } else {
    res_ = mysql_store_result(conn);
    int num = mysql_num_rows(res_);
    if(0 == num) {
      res.serial_number = 0;
    } else {
      row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%d", &res.serial_number);
      sscanf(row_[1], "%s", res.path);
    }
    mysql_free_result(res_);
  }
  DestoryConnection(conn);
  return res;
}

DocumentInfo DBManager::GetDocument(const std::string& meeting_id,
                                                      int document_id) {
  DocumentInfo res;
  char sql[500];
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "SELECT path FROM MeetingDocument where MeetingID \
  = '%s' and DocumentID = %d", meeting_id.c_str(), document_id);
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
     res.serial_number = -1;
  } else {
    res_ = mysql_store_result(conn);
    int num = mysql_num_rows(res_);
    if(0 == num) {
      res.serial_number = 0;
    } else {
      row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%s", res.path);
      res.serial_number = document_id;
    }
    mysql_free_result(res_);
  }
  DestoryConnection(conn);
  return res;
}

DocumentInfo* DBManager::GetHistoryDocuments(const std::string& meeting_id, int& size) {
  DocumentInfo* res = NULL;
  char sql[500];
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "SELECT DocumentID,path FROM \
  MeetingDocument where \
  MeetingID = '%s' order by DocumentID", meeting_id.c_str());
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
     size = -1;
  } else {
    res_ = mysql_store_result(conn);
    size = mysql_num_rows(res_);
    if (0 != size) {
      res = new DocumentInfo[size];
      for (int i = 0; i < size; i++) {
        row_ = mysql_fetch_row(res_);
        sscanf(row_[0], "%d", &res[i].serial_number);
        sscanf(row_[1], "%s", res[i].path);
      }
    }
    mysql_free_result(res_);
  }
  DestoryConnection(conn);
  return res;
}

int DBManager::AddMeetingUser(const std::string& meeting_id,
                const std::string& user_id, int state) {
  // 1 for normal, 2 for require, 3 for host
  char sql[500];
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  int meeting_sta;
  int result;
  conn = InitConnection();
  time_t now = time(NULL);
  snprintf(sql, sizeof(sql), "select state from Meeting where \
  MeetingID = '%s'", meeting_id.c_str());
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
     result = 0;
  } else {
    res_ = mysql_store_result(conn);
    if (0 != mysql_num_rows(res_)) {
      row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%d", &meeting_sta);
      mysql_free_result(res_);
      switch (meeting_sta) {
        case 0:
          if (!SetMeetingState(meeting_id, 1)) {
            LOG(ERROR) << "failed to set Meeting state";
            result = 0;
            break;
          }
        case 1:
         snprintf(sql, sizeof(sql), "insert into MeetingUser(MeetingID, UserID,\
         State, HBTime)  values('%s', '%s', %d, %ld)",
         meeting_id.c_str(), user_id.c_str(), state, now);
          if (mysql_query(conn, sql)) {
            LOG(ERROR) << mysql_error(conn);
            result = 0;
          } else {
            result = 1;
          }
          break;
        default:
          LOG(INFO) << "the meeting is dead";
          result = 3;  //  the meeting is dead
          break;
      }
    } else {
      result = 2;  //  the meeting id doesn't exists
    }
  }
  DestoryConnection(conn);
  return result;
}

int DBManager::GetUserState(const std::string& meeting_id,
                                               const std::string& user_id) {
  char sql[500];
  int res = -1;
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "select State from MeetingUser where \
  MeetingID = '%s' and UserID = '%s'", meeting_id.c_str(), user_id.c_str());
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
  } else {
    res_ = mysql_store_result(conn);
    int num = mysql_num_rows(res_);
    if (0 == num) {
      res = 0;
    } else {
      row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%d", &res);
    }
    mysql_free_result(res_);
  }
  DestoryConnection(conn);
  return res;
}

int DBManager::UpdateUserState(const std::string& meeting_id,
                                 const std::string& user_id, int state) {
  char sql[500];
  time_t now = time(NULL);
  MYSQL *conn;
  MYSQL_RES *res_;
  now = now - 5;
  int result = 0;
  conn = InitConnection();
  if (3 == state) {
    snprintf(sql, sizeof(sql), "select * from MeetingUser where MeetingID = '%s'\
    and  UserID = '%s' and HBTime > %ld", meeting_id.c_str(), user_id.c_str(), now);
    if (mysql_query(conn, sql)) {
      LOG(ERROR) << mysql_error(conn);
      result = -1;
    } else {
      res_ = mysql_store_result(conn);
      int num = mysql_num_rows(res_);
      mysql_free_result(res_);
      if (0 == num) {
        result = 2;
      }
    }
  }
  if (result != -1 && result != 2) {
    snprintf(sql, sizeof(sql), "update MeetingUser set State = %d where \
    MeetingID = '%s' and UserID = '%s'", state,
    meeting_id.c_str(), user_id.c_str());
    if (mysql_query(conn, sql)) {
      LOG(ERROR) << mysql_error(conn);
      result = 0;
    } else {
      result = 1;
    }
  }
  DestoryConnection(conn);
  return result;
}

UserInfo* DBManager::GetUserList(const std::string& meeting_id, int& size) {
  UserInfo* res = NULL;
  char sql[500];
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "SELECT UserID, State FROM MeetingUser where \
  MeetingID = '%s' order by id", meeting_id.c_str());
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
     size = -1;
  } else {
    res_ = mysql_store_result(conn);
    size = mysql_num_rows(res_);
    if (0 != size) {
      res = new UserInfo[size];
      for (int i = 0; i < size; i++) {
        row_ = mysql_fetch_row(res_);
        sscanf(row_[0], "%s", res[i].user_name);
        sscanf(row_[1], "%d", &res[i].state);
      }
    }
    mysql_free_result(res_);
  }
  DestoryConnection(conn);
  return res;
}

bool DBManager::UpdateUserTime(const std::string& meeting_id,
                                      const std::string& user_id) {
  time_t now = time(NULL);
  char sql[500];
  MYSQL *conn;
  bool result;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "update MeetingUser set HBTime = %ld where \
  MeetingID = '%s' and UserID = '%s'",
  now, meeting_id.c_str(), user_id.c_str());
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
     result = false;
  } else {
     result = true;
  }
  DestoryConnection(conn);
  return result;
}

bool DBManager::DeleteDeadUser() {
  time_t now = time(NULL);
  now = now - 5*60;
  bool result;
  char sql[500];
  MYSQL *conn;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "delete from MeetingUser where \
  HBTime < %ld", now);
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
     result = false;
  } else {
     result = true;
  }
  DestoryConnection(conn);
  return result;
}

std::string* DBManager::GetDeadMeeting(int& size) {
  std::string* res = NULL;
  char sql[500];
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "select m.MeetingID from Meeting m where \
  m.state = 1 and m.MeetingID not in (select MeetingID from MeetingUser\
  group by MeetingID) order by id");
  if (mysql_query(conn, sql)) {
    LOG(ERROR) << mysql_error(conn);
    size = -1;
  } else {
    res_ = mysql_store_result(conn);
    size = mysql_num_rows(res_);
    if (0 != size) {
      res = new std::string[size];
      for (int i = 0; i < size; i++) {
        row_ = mysql_fetch_row(res_);
        res[i] = row_[0];
      }
    }
    mysql_free_result(res_);
    snprintf(sql, sizeof(sql), "update Meeting set state = 2 where \
    state = 1 and MeetingID not in (select MeetingID from MeetingUser\
    group by MeetingID)");
    if (mysql_query(conn, sql)) {
      LOG(ERROR) << mysql_error(conn);
      LOG(ERROR) << "delete dead meeting failed";
    }
  }
  DestoryConnection(conn);
  return res;
}

std::string* DBManager::GetDeadHostMeeting(int& size) {
  std::string* res = NULL;
  char sql[500];
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  time_t now = time(NULL);
  now = now - 5;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "select MeetingID from MeetingUser where \
  State = 3 and HBTime < %ld order by id", now);
  if (mysql_query(conn, sql)) {
     LOG(ERROR) << mysql_error(conn);
     size = -1;
  } else {
    res_ = mysql_store_result(conn);
    size = mysql_num_rows(res_);
    if (0 != size) {
      res = new std::string[size];
      for (int i = 0; i < size; i++) {
        row_ = mysql_fetch_row(res_);
        res[i] = row_[0];
  TransferAuth(res[i]);
      }
    }
    mysql_free_result(res_);
  }
  DestoryConnection(conn);
  return res;
}

bool DBManager::TransferAuth(const std::string& meeting_id) {
  char sql[500];
  bool result;
  char res[50];
  MYSQL *conn;
  MYSQL_RES *res_;
  MYSQL_ROW row_;
  int size;
  time_t now = time(NULL);
  now = now - 5;
  conn = InitConnection();
  snprintf(sql, sizeof(sql), "update MeetingUser set State = 1 where \
  State = 3 and MeetingID = '%s'", meeting_id.c_str());
  if (mysql_query(conn, sql)) {
    LOG(ERROR) << "transferAuth" << mysql_error(conn);
    result = false;
  } else {
    snprintf(sql, sizeof(sql), "select UserID from MeetingUser where \
    State = 2 and MeetingID = '%s' and HBTime > %ld", meeting_id.c_str(), now);
    if (mysql_query(conn, sql)) {
      LOG(ERROR) << mysql_error(conn);
      result = false;
    } else {
      res_ = mysql_store_result(conn);
      size = mysql_num_rows(res_);
      if (0 != size) {
        row_ = mysql_fetch_row(res_);
       sscanf(row_[0], "%s", res);
       snprintf(sql, sizeof(sql), "update MeetingUser set State = 3 where \
  UserID = '%s' and MeetingID = '%s'", res, meeting_id.c_str());
        if (mysql_query(conn, sql)) {
           LOG(ERROR) << "transferAuth failed" << mysql_error(conn);
           result = false;
        } else {
           result = true;
        }
      } else {
       snprintf(sql, sizeof(sql), "select UserID from MeetingUser where \
    State = 1 and MeetingID = '%s' and HBTime > %ld", meeting_id.c_str(), now);
        if (mysql_query(conn, sql)) {
           LOG(ERROR) << mysql_error(conn);
           result = false;
        } else {
           res_ = mysql_store_result(conn);
           size = mysql_num_rows(res_);
           if (0 != size) {
             row_ = mysql_fetch_row(res_);
            sscanf(row_[0], "%s", res);
            snprintf(sql, sizeof(sql), "update MeetingUser set State = 3 where \
  UserID='%s' and MeetingID= '%s'", res, meeting_id.c_str());
             if (mysql_query(conn, sql)) {
               LOG(ERROR) << "transferAuth failed" << mysql_error(conn);
               result = false;
             } else {
               result = true;
             }
           } else {
       result = true;
          }
        }
      }
      mysql_free_result(res_);
    }
  }
  DestoryConnection(conn);
  return result;
}
}  // DBManager
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
