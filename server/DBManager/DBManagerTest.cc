/*************************************************************************
     ** File Name :  DBManagerTest.cc
    ** Author :  fl570
    ** Mail :  cqfl570@gmail.com
    ** Created Time :  Wed Apr 10 16 : 46 : 51 2013
    **Copyright [2013]  <Copyright fl570>   [legal/copyright]
 ************************************************************************/

#include <stdio.h>
#include <time.h>
#include "./DBManager.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DBManager  {

DEFINE_string(test_path, "/root/Kingslanding/OnlineWhiteBoard/server\
/DBManager/", "the path of the test sql file");

class DBManagerTest: public ::testing::Test {
protected:
  static DBManager *db_instance ;
  MYSQL* conn_;
  static void SetUpTestCase() {
    google::InitGoogleLogging("LOG");
    FLAGS_log_dir = "../LOG";
  }
  static void TearDownTestCase() {
    google::ShutdownGoogleLogging();
  }
  virtual void SetUp() {
    char sql[200];
    conn_ = db_instance -> InitConnection();
    snprintf(sql, sizeof(sql), "load data local infile  '%sMeeting.txt' \
    into table Meeting", FLAGS_test_path.c_str());
    if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
    }
    snprintf(sql, sizeof(sql), "load data local infile  '%sMeetingUser.txt'\
    into table MeetingUser", FLAGS_test_path.c_str());
    if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
    }
    snprintf(sql, sizeof(sql), "load data local infile  '%sMeetingDocument.txt'\
    into table MeetingDocument", FLAGS_test_path.c_str());
    if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
    }
  }
  virtual void TearDown() {
    char sql[500];
    strncpy(sql, "delete from Meeting", sizeof(sql));
    if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
    }
    strncpy(sql, "delete from MeetingUser", sizeof(sql));
    if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
    }
    strncpy(sql, "delete from MeetingDocument", sizeof(sql));
    if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
    }
    db_instance -> DestoryConnection(conn_);
  }
};

DBManager* DBManagerTest::db_instance = DBManager::GetInstance();

TEST_F(DBManagerTest, AddMeeting) {
  EXPECT_LT(70, db_instance->AddMeeting());
}

TEST_F(DBManagerTest, SetMeetingID) {
  int id = db_instance->AddMeeting();
  EXPECT_TRUE(db_instance->SetMeetingID("abcd", id));
  char sql[500];
  char result[50];
  snprintf(sql, sizeof(sql), "select MeetingID  from Meeting \
  where id = %d", id);
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
      MYSQL_ROW row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%s", result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(0, strcmp(result, "abcd"));
}

TEST_F(DBManagerTest, SetMeetingState) {
  EXPECT_TRUE(db_instance->SetMeetingState(
  "072B030BA126B2F4B2374F342BE9ED44", 1));
  char sql[500];
  int result = 0;
  snprintf(sql, sizeof(sql), "select state  from Meeting where id = 60");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
      MYSQL_ROW row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%d", &result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(1, result);
}

TEST_F(DBManagerTest, AddMeetingPort) {
  EXPECT_TRUE(db_instance->AddMeetingPort(
  "072B030BA126B2F4B2374F342BE9ED44", 1));
  char sql[500];
  int result = 0;
  snprintf(sql, sizeof(sql), "select Port  from Meeting where id = 60");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
      MYSQL_ROW row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%d", &result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(1, result);
}

TEST_F(DBManagerTest, GetMeetingPort) {
  EXPECT_EQ(8080, db_instance->GetMeetingPort(
  "7F39F8317FBDB1988EF4C628EBA02591"));
}

TEST_F(DBManagerTest, SetDataRef) {
  EXPECT_TRUE(db_instance->SetDataRef(
  "072B030BA126B2F4B2374F342BE9ED44", 1));
  char sql[500];
  int result = 0;
  snprintf(sql, sizeof(sql), "select DataRef  from Meeting where id = 60");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
      MYSQL_ROW row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%d", &result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(1, result);
}

TEST_F(DBManagerTest, GetDataRef) {
  EXPECT_EQ(1234567, db_instance->GetDataRef(
  "7F39F8317FBDB1988EF4C628EBA02591"));
}

TEST_F(DBManagerTest, AddDocuemnt) {
  EXPECT_TRUE(db_instance->AddDocument(
  "072B030BA126B2F4B2374F342BE9ED44", "abcd"));
  char sql[500];
  char result[100];
  snprintf(sql, sizeof(sql), "select path  from MeetingDocument\
  where DocumentID > 12 \
  and MeetingID = '072B030BA126B2F4B2374F342BE9ED44'");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
      MYSQL_ROW row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%s", result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(0, strcmp(result, "abcd"));
}

TEST_F(DBManagerTest, GetDocument) {
  DocumentInfo res = db_instance->GetDocument(
  "14BFA6BB14875E45BBA028A21ED38046", 5);
  EXPECT_EQ(0, strcmp(res.path, "567890"));
  EXPECT_EQ(5, res.serial_number);
}

TEST_F(DBManagerTest, GetCurrentDocument) {
  DocumentInfo res = db_instance->GetCurrentDocument(
  "14BFA6BB14875E45BBA028A21ED38046");
  EXPECT_EQ(0, strcmp(res.path, "123dasf"));
  EXPECT_EQ(12, res.serial_number);
}

TEST_F(DBManagerTest, GetHistoryDocuments) {
  int size;
  DocumentInfo *info = db_instance->GetHistoryDocuments(
  "A3F390D88E4C41F2747BFA2F1B5F87DB", size);
  EXPECT_EQ(6, size);
  EXPECT_EQ(0, strcmp(info[0].path, "asdasfd"));
  EXPECT_EQ(0, strcmp(info[1].path, "12afdas"));
  EXPECT_EQ(0, strcmp(info[2].path, "dfasrqrea"));
  EXPECT_EQ(0, strcmp(info[3].path, "asfdaeq"));
  EXPECT_EQ(0, strcmp(info[4].path, "adsferq"));
  EXPECT_EQ(0, strcmp(info[5].path, "1adsfasdfa"));
  EXPECT_EQ(6, info[0].serial_number);
  EXPECT_EQ(7, info[1].serial_number);
  EXPECT_EQ(8, info[2].serial_number);
  EXPECT_EQ(9, info[3].serial_number);
  EXPECT_EQ(10, info[4].serial_number);
  EXPECT_EQ(11, info[5].serial_number);
}

TEST_F(DBManagerTest, AddMeetingUser) {
  EXPECT_EQ(1, db_instance->AddMeetingUser(
  "072B030BA126B2F4B2374F342BE9ED44", "abcd", 3));
  char sql[500];
  int result = 0;
  snprintf(sql, sizeof(sql), "select state  from Meeting  where id = 60");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
      MYSQL_ROW row_ = mysql_fetch_row(res_);
      sscanf(row_[0], "%d", &result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(1, result);
  EXPECT_EQ(3, db_instance->AddMeetingUser(
  "44F683A84163B3523AFE57C2E008BC8C", "abcd", 3));
  EXPECT_EQ(2, db_instance->AddMeetingUser("123456", "abcd", 3));
}

TEST_F(DBManagerTest, GetUserState) {
  EXPECT_EQ(2, db_instance->GetUserState(
  "FC490CA45C00B1249BBE3554A4FDF6FB", "cd2" ));
}

TEST_F(DBManagerTest, UpdateUserState) {
  EXPECT_EQ(1, db_instance->UpdateUserState(
  "FC490CA45C00B1249BBE3554A4FDF6FB", "cd3", 1));
  char sql[500];
  int result = 0;
  snprintf(sql, sizeof(sql), "select State  from MeetingUser  where id = 16");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
     if (0 != mysql_num_rows(res_)) {
       MYSQL_ROW row_ = mysql_fetch_row(res_);
       sscanf(row_[0], "%d", &result);
     }
     mysql_free_result(res_);
  }
  EXPECT_EQ(1, result);
  EXPECT_EQ(1, db_instance->UpdateUserState(
  "FC490CA45C00B1249BBE3554A4FDF6FB", "cd2", 3));
  snprintf(sql, sizeof(sql), "select State  from MeetingUser  where id = 15");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
     if (0 != mysql_num_rows(res_)) {
       MYSQL_ROW row_ = mysql_fetch_row(res_);
       sscanf(row_[0], "%d", &result);
     }
     mysql_free_result(res_);
  }
  EXPECT_EQ(3, result);
  EXPECT_EQ(2, db_instance->UpdateUserState(
  "FC490CA45C00B1249BBE3554A4FDF6FB", "cd1", 3));
  snprintf(sql, sizeof(sql), "select State  from MeetingUser  where id = 14");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
     if (0 != mysql_num_rows(res_)) {
       MYSQL_ROW row_ = mysql_fetch_row(res_);
       sscanf(row_[0], "%d", &result);
     }
     mysql_free_result(res_);
  }
  EXPECT_EQ(1, result);
  EXPECT_EQ(4, db_instance->UpdateUserState(
  "FC490CA45C00B1249BBE3554A4FDF6FB", "", 4));
  snprintf(sql, sizeof(sql), "select State  from MeetingUser  where id = 16");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
     if (0 != mysql_num_rows(res_)) {
       MYSQL_ROW row_ = mysql_fetch_row(res_);
       sscanf(row_[0], "%d", &result);
     }
     mysql_free_result(res_);
  }
  EXPECT_EQ(1, result);
}

TEST_F(DBManagerTest, GetUserList) {
  int size;
  UserInfo *info = db_instance->GetUserList(
  "FC490CA45C00B1249BBE3554A4FDF6FB", size);
  EXPECT_EQ(3, size);
  EXPECT_EQ(0, strcmp(info[0].user_name, "cd1"));
  EXPECT_EQ(0, strcmp(info[1].user_name, "cd2"));
  EXPECT_EQ(0, strcmp(info[2].user_name, "cd3"));
  EXPECT_EQ(1, info[0].state);
  EXPECT_EQ(2, info[1].state);
  EXPECT_EQ(3, info[2].state);
}

TEST_F(DBManagerTest, UpdateUserTime) {
  time_t now = time(NULL);
  EXPECT_TRUE(db_instance->UpdateUserTime(
  "FC490CA45C00B1249BBE3554A4FDF6FB", "cd1"));
  char sql[500];
  int64_t result = 0;
  snprintf(sql, sizeof(sql), "select HBTime  from MeetingUser  where id = 14");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
     if (0 != mysql_num_rows(res_)) {
       MYSQL_ROW row_ = mysql_fetch_row(res_);
       sscanf(row_[0], "%ld", &result);
     }
     mysql_free_result(res_);
  }
  EXPECT_LE(now, result);
}

TEST_F(DBManagerTest, DeleteDeadUser) {
  EXPECT_TRUE(db_instance->DeleteDeadUser());
  char sql[500];
  snprintf(sql, sizeof(sql), "select *  from MeetingUser  where HBTime < 1000000");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
     if (0 != mysql_num_rows(res_)) {
        EXPECT_TRUE(false);
     }
     mysql_free_result(res_);
  }
}

TEST_F(DBManagerTest, GetDeadMeeting) {
  int size;
  std::string *res = db_instance->GetDeadMeeting(size);
  EXPECT_EQ(4, size);
  EXPECT_EQ(0, strcmp(res[0].c_str(), "3295C76ACBF4CAAED33C36B1B5FC2CB1"));
  EXPECT_EQ(0, strcmp(res[1].c_str(), "735B90B4568125ED6C3F678819B6E058"));
  EXPECT_EQ(0, strcmp(res[2].c_str(), "A3F390D88E4C41F2747BFA2F1B5F87DB"));
  EXPECT_EQ(0, strcmp(res[3].c_str(), "14BFA6BB14875E45BBA028A21ED38046"));
  
  char sql[500];
  char result[10][50];
  snprintf(sql, sizeof(sql), "select MeetingID  from Meeting  where state = 2 \
  order by id");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
     MYSQL_RES *res_ = mysql_store_result(conn_);
     MYSQL_ROW row_ ;
     int num = mysql_num_rows(res_);
     EXPECT_EQ(5, num);
     if (0 != num) {
         for(int i=0; i<num; i++) {
           row_ = mysql_fetch_row(res_);
           sscanf(row_[0], "%s", result[i]);
         }
      EXPECT_EQ(0, strcmp(result[1], "3295C76ACBF4CAAED33C36B1B5FC2CB1"));
      EXPECT_EQ(0, strcmp(result[2], "735B90B4568125ED6C3F678819B6E058"));
      EXPECT_EQ(0, strcmp(result[3], "A3F390D88E4C41F2747BFA2F1B5F87DB"));
      EXPECT_EQ(0, strcmp(result[4], "14BFA6BB14875E45BBA028A21ED38046"));
     }
     mysql_free_result(res_);
  }
}

TEST_F(DBManagerTest, GetDeadHostMeeting) {
  int size;
  std::string *res = db_instance->GetDeadHostMeeting(size);
  EXPECT_EQ(4, size);
  EXPECT_EQ(0, strcmp(res[0].c_str(), "7F39F8317FBDB1988EF4C628EBA02591"));
  EXPECT_EQ(0, strcmp(res[1].c_str(), "44F683A84163B3523AFE57C2E008BC8C"));
  EXPECT_EQ(0, strcmp(res[2].c_str(), "03AFDBD66E7929B125F8597834FA83A4"));
  EXPECT_EQ(0, strcmp(res[3].c_str(), "FC490CA45C00B1249BBE3554A4FDF6FB"));
}

TEST_F(DBManagerTest, TransferAuth) {
  EXPECT_EQ(0, db_instance->TransferAuth(
  "7F39F8317FBDB1988EF4C628EBA02591"));
  char result[100];
  char sql[500];
  snprintf(sql, sizeof(sql), "select UserID  from MeetingUser  where State = 3\
  and MeetingID = '7F39F8317FBDB1988EF4C628EBA02591' ");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
      MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
        MYSQL_ROW row_ = mysql_fetch_row(res_);
        sscanf(row_[0], "%s", result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(0, strcmp(result, "van3"));
  EXPECT_EQ(1, db_instance->TransferAuth(
  "44F683A84163B3523AFE57C2E008BC8C"));
  snprintf(sql, sizeof(sql), "select UserID  from MeetingUser  where State = 3\
  and MeetingID = '44F683A84163B3523AFE57C2E008BC8C' ");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
      MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
        MYSQL_ROW row_ = mysql_fetch_row(res_);
        sscanf(row_[0], "%s", result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(0, strcmp(result, "fl1"));
  EXPECT_EQ(1, db_instance->TransferAuth(
  "03AFDBD66E7929B125F8597834FA83A4"));
  snprintf(sql, sizeof(sql), "select UserID  from MeetingUser  where State = 3\
  and MeetingID = '03AFDBD66E7929B125F8597834FA83A4' ");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
      MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
        MYSQL_ROW row_ = mysql_fetch_row(res_);
        sscanf(row_[0], "%s", result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(0, strcmp(result, "ab2"));
  EXPECT_EQ(1, db_instance->TransferAuth(
  "EA5D2F1C4608232E07D3AA3D998E5135"));
  snprintf(sql, sizeof(sql), "select UserID  from MeetingUser  where State = 3\
  and MeetingID = 'EA5D2F1C4608232E07D3AA3D998E5135' ");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
      MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
        MYSQL_ROW row_ = mysql_fetch_row(res_);
        sscanf(row_[0], "%s", result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(0, strcmp(result, "bc2"));
  EXPECT_EQ(1, db_instance->TransferAuth(
  "FC490CA45C00B1249BBE3554A4FDF6FB"));
  snprintf(sql, sizeof(sql), "select UserID  from MeetingUser  where State = 3\
  and MeetingID = 'FC490CA45C00B1249BBE3554A4FDF6FB' ");
  if (mysql_query(conn_, sql)) {
     LOG(ERROR) << mysql_error(conn_);
  } else {
      MYSQL_RES *res_ = mysql_store_result(conn_);
      if (0 != mysql_num_rows(res_)) {
        MYSQL_ROW row_ = mysql_fetch_row(res_);
        sscanf(row_[0], "%s", result);
      }
      mysql_free_result(res_);
  }
  EXPECT_EQ(0, strcmp(result, "cd2"));
}
}  // DBManager
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding

int main(int argc, char **argv) {
  std::cout << "Running main() from gtest_main.cc\n";
  google::ParseCommandLineFlags(&argc, &argv, true);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
