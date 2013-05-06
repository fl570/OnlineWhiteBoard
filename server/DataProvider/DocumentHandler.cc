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
DocumentHandler::DocumentHandler(Kingslanding::OnlineWhiteBoard::Server::Monitor::MeetingHandler* Meeting_Handler): MsgHandler()
{
  meeting_handler = Meeting_Handler;
}

Document DocumentHandler::GetDocument(const std::string& meeting_id, int document_id)
{
  DBManager::DocumentInfo document;
  document = db_manager_->GetDocument(meeting_id, document_id);
  IplImage* img;
  img = cvLoadImage(document.path, 1);
  uchar* data = (uchar*)img->imageData;
  Document document1;
  document1.set_serial_number(document.serial_number);
  document1.set_data((char*)data);
  cvReleaseImage(&img);
  return document1;
}

Document DocumentHandler::GetCurrentDocument(const std::string& meeting_id) {
  Kingslanding::OnlineWhiteBoard::Server::DrawOperation::DrawOperation* draw_operation =
  meeting_handler->GetDrawOperation(meeting_id);
  DrawOperation::PathInfo path_info = draw_operation->SaveAsBmp(2);
  IplImage* img;
  img = cvLoadImage(path_info.path.c_str(), 1);
  uchar* data =(uchar*) img->imageData;
  Document document;
  document.set_serial_number(path_info.number);
  document.set_data((char*)data);
  cvReleaseImage(&img);
  return document;
}

DocumentList DocumentHandler::GetHistorySnapshots(const std::string& meeting_id)
{
  DocumentList list;
  int size;
  IplImage* img;
  IplImage* history_img;
  DBManager::DocumentInfo *res = db_manager_->GetHistoryDocuments(meeting_id, size);
  for(int i = 0; i < size; i++) {
    Document* history_document = list.add_history_document();
    img = cvLoadImage(res[i].path, 1);
    history_img = zoom(img, 200, 200);
    uchar* data =(uchar*) history_img->imageData;
    history_document->set_serial_number(res[i].serial_number);
    history_document->set_data((char*)data);
  }
  if (res != NULL) {
    delete []res;
  }
  cvReleaseImage(&img);
  cvReleaseImage(&history_img);
  return list;
}

IplImage* DocumentHandler::zoom(IplImage* image, int rows , int cols ){
  IplImage* pImage = cvCreateImage(cvSize(cols,rows),image->depth,image->nChannels);
  float scaleFactorRow = ((float)rows)/image->height;
  float scaleFactorCol = ((float)cols)/image->width;
  int i_out, j_out, i_in, j_in;
  float i_in_f, j_in_f;
  float u, v;
  float pixel,pixel1,pixel2;
  u=v=0.0;
  for (i_out = 0; i_out<rows;++i_out) {
    for (j_out =0;j_out<cols;++j_out) {
      CvScalar sa =cvGet2D(pImage, i_out, j_out);
      i_in_f=((float)i_out)/scaleFactorRow;
      j_in_f=((float)j_out)/scaleFactorCol;
      u=i_in_f-cvFloor(i_in_f);
      v=j_in_f-cvFloor(j_in_f);
      i_in =cvFloor(i_in_f);
      j_in=cvFloor(j_in_f);
      CvScalar s = cvGet2D(image,i_in,j_in);
      CvScalar s1= cvGet2D(image,i_in,j_in+1);
      CvScalar s2=cvGet2D(image, i_in+1, j_in);
      CvScalar s3 =cvGet2D(image,i_in+1,j_in+1);
      if(image->height==i_in ||image->width ==j_in || 0==i_in ||0 ==j_in)
        continue;
       pixel=(1-u)*(1-v)*CV_IMAGE_ELEM(image,uchar,i_in,3*j_in)+
       (1-u)*v*CV_IMAGE_ELEM(image,uchar,i_in,3*(j_in+1))+
       u*(1-v)*CV_IMAGE_ELEM(image,uchar,i_in+1,3*j_in)+
       u*v*CV_IMAGE_ELEM(image, uchar, i_in+1,3*(j_in+1));
       pixel1=(1-u)*(1-v)*CV_IMAGE_ELEM(image,uchar,i_in,3*j_in+1)+
       (1-u)*v*CV_IMAGE_ELEM(image,uchar,i_in,3*(j_in+1)+1)+
       u*(1-v)*CV_IMAGE_ELEM(image,uchar,i_in+1,3*j_in+1)+
       u*v*CV_IMAGE_ELEM(image, uchar, i_in+1,3*(j_in+1)+1);
       pixel2=(1-u)*(1-v)*CV_IMAGE_ELEM(image,uchar,i_in,3*j_in+2)+
       (1-u)*v*CV_IMAGE_ELEM(image,uchar,i_in,3*(j_in+1)+2)+
       u*(1-v)*CV_IMAGE_ELEM(image,uchar,i_in+1,3*j_in+2)+
       u*v*CV_IMAGE_ELEM(image, uchar, i_in+1,3*(j_in+1)+2);
       CV_IMAGE_ELEM(pImage,uchar,i_out,3*j_out)=cvFloor(pixel);
       CV_IMAGE_ELEM(pImage,uchar,i_out,3*j_out+1)=cvFloor(pixel1);
       CV_IMAGE_ELEM(pImage,uchar,i_out,3*j_out+2)=cvFloor(pixel2);
    }
  }
  return pImage;
}
}  // DataProvider
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
