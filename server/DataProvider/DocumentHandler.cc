/*************************************************************************
     ** File Name: DocumentHandler.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright fl570>  [legal/copyright]
 ************************************************************************/

#include "./DocumentHandler.h"
#include <math.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DataProvider {
DocumentHandler::DocumentHandler(MeetingHandler& Meeting_Handler): MsgHandler()
{
  meeting_handler = Meeting_Handler;
}

Document DocumentHandler::GetDocument(const std::string& meeting_id, int document_id)
{
  DBManager::DocumentInfo document;
  document = db_manager_->GetDocument(meeting_id, document_id);
  IplImage* img;
  img = cvLoadImage(document.path, 0);
  uchar* data = (uchar*)img->imageData;
  Document document1;
  document1.set_serial_number(document.serial_number);
  document1.set_data(data);
  cvReleaseImage(&img);
}

Document DocumentHandler::GetCurrentDocument(const std::string& meeting_id)
{
  DrawOperation* draw_operation = meeting_handler->getDrawOperation(meeting_id);
  std::string path = draw_operation->SaveAsBmp();
  IplImage* img;
  img = cvLoadImage(path, 0);
  uchar* data = (uchar*)img->imageData;
  Document document;
  document.set_serial_number(0);
  document.set_data(data);
  cvReleaseImage(&img);
}

DocumentList DocumentHandler::GetHistorySnapshots(const std::string& meeting_id)
{
  DocumentList list;
  int * size;
  DBManager::DocumentInfo * res = GetHistorySnapshots(meeting_id, size);
  for(int i = 0; i < *size; i++) {
    Document history_document = list.add_history_document();
    IplImage* img;
    img = cvLoadImage(res[i].path, 0);
    uchar* data = (uchar*)img->imageData;
    history_document.set_serial_number(res[i].serial_number);
    history_document.set_data(data);
    cvReleaseImage(&img);
  }
  return list;
}
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
