/*************************************************************************
     ** File Name: DrawRect.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <stdlib.h>
#include "../message.pb.h"
#include "./DrawOperation.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {

DEFINE_string(picture_store_address, "/root/picture/", "the address where pictures store");

DrawOperation::DrawOperation(std::string meeting_id) {
  meeting_id_ = meeting_id;
  a_ = 0;
  b_ = 0;
  color_ = 0;
  thinkness_ = 0;
  alpha_ = 0;
  xa_ = 0;
  ya_ = 0;
  xb_ = 0;
  yb_ = 0;
  serial_num_ = 0;
  picture_ = cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));
}

DrawOperation::~DrawOperation(){
  std::string str = "rm -f "+FLAGS_picture_store_address+"temp/*"+meeting_id_+"-*.bmp";
  system(str.c_str());
}


void DrawOperation::Draw(Operation op, unsigned int number) {
  Operation_OperationData* data;
  data = op.mutable_data();
  serial_num_ = number;
  switch (data->data_type()) {
    case Operation_OperationData_OperationDataType_ELLIPSE:
      a_ = data->a();
      b_ = data->b();
      color_ = data->color();
      thinkness_ = data->thinkness();
      xa_ = data->mutable_center()->x();
      ya_ = data->mutable_center()->y();
      if (a_ == b_) {
        if (data->fill()) { 
          alpha_ = data->alpha();
          transCircle.set(color_, thinkness_, xa_, ya_, a_, alpha_);
	  boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
          transCircle.Draw(picture_);
        } else {
          circle.set(color_, thinkness_, xa_, ya_, a_);
	  boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
          circle.Draw(picture_);
        }
      } else {
        if (data->fill()) {
          alpha_  = data->alpha();
          transEclipse.set(color_, thinkness_, xa_, ya_, a_, b_, alpha_);
	  boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
          transEclipse.Draw(picture_);
        } else {
          eclipse.set(color_, thinkness_, xa_, ya_, a_, b_);
	  boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
          eclipse.Draw(picture_);
        }	
      }
      break;
    case Operation_OperationData_OperationDataType_LINE: {
      color_ = data->color();
      thinkness_ = data->thinkness();
      xa_ = data->mutable_start_point()->x();
      ya_ = data->mutable_start_point()->y();
      xb_ = data->mutable_end_point()->x();
      yb_ = data->mutable_end_point()->y();
      line.set(color_, thinkness_, xa_, ya_, xb_, yb_);
      boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
      line.Draw(picture_);
      lock.unlock();
      lock.release();
      break;
    }
    case Operation_OperationData_OperationDataType_RECTANGE: {
      color_ = data->color();
      thinkness_ = data->thinkness();
      xa_ = data->mutable_top_left_corner()->x();
      ya_ = data->mutable_top_left_corner()->y();
      xb_ = data->mutable_bottom_right_corner()->x();
      yb_ = data->mutable_bottom_right_corner()->y();
      if (data->fill()) {
        alpha_ = data->alpha();
        transRect.set(color_, thinkness_, xa_, ya_, xb_, yb_, alpha_);
	boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
        transRect.Draw(picture_);
      } else {
        rect.set(color_, thinkness_, xa_, ya_, xb_, yb_);
	boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
        rect.Draw(picture_);
      }
      break;
    }
    case Operation_OperationData_OperationDataType_POINT: {
      color_ = data->color();
      thinkness_ = data->thinkness();
      xa_ = data->mutable_position()->x();
      ya_ = data->mutable_position()->y();
      point.set(color_, thinkness_, xa_, ya_);
      boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
      point.Draw(picture_);
      lock.unlock();
      lock.release();
      break;
    }
    case Operation_OperationData_OperationDataType_ERASER: {
      thinkness_ = data->thinkness();
      xa_ = data->mutable_position()->x();
      ya_ = data->mutable_position()->y();
      eraser.set(thinkness_, xa_, ya_);
      boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
      eraser.Draw(picture_);
      lock.unlock();
      lock.release();
      break;
    }
  }
}

void DrawOperation::Show() {
  cv:: imshow("display", picture_);
  cv::waitKey(0);
}

bool DrawOperation::Load(const std::string& path) {
  boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
  IplImage img = IplImage(picture_);
  IplImage* pImage = &img;
  pImage = cvLoadImage(path.c_str(), 1);
  cvReleaseImage(&pImage);
  return true;
}



PathInfo DrawOperation::SaveAsBmp(int i) {
  boost::shared_lock<boost::shared_mutex> lock(draw_mutex_);
  time_t now = time(NULL);
  std::ostringstream bufstr;
  bufstr << now;
  std::string now_time = bufstr.str();
  std::string path;
  if (i == 1) {
    path = FLAGS_picture_store_address+meeting_id_+"-"+now_time+".bmp";
  } else if (i == 2) {
    path = FLAGS_picture_store_address+"/temp"+meeting_id_+"-"+now_time+".bmp";
  }
  bool temp = cv::imwrite(path, picture_);
  PathInfo path_info;
  path_info.number = serial_num_;
  path_info.path = path;
  if(!temp)
    path_info.path = "";
  return path_info;
}


}  // DrawOperation
}  // ServerOperation op2;
}  // OnlineWhiteBoard
}  // Kingslanding
/*
int main() {
  Kingslanding::OnlineWhiteBoard::Server::DrawOperation::DrawOperation* d = new  Kingslanding::OnlineWhiteBoard::Server::DrawOperation::DrawOperation("fl570") ;
  d->Load("a.bmp");
     Operation  op;
    op.set_serial_number(1);
   Operation_OperationData*  data;
   data=op.mutable_data();
   data->set_data_type(Operation_OperationData_OperationDataType_ELLIPSE);
   data->set_thinkness(1);
   data->set_color(2);
   data->set_a(100);
   data->set_b(50);
   data->set_fill(true);
   data->set_alpha(0.7);
  Operation_OperationData_Point* point1;
   point1=data->mutable_center();
   point1->set_x(100);
   point1->set_y(100);
   d->Draw(op, 1);
   Operation op1;
   op1.set_serial_number(2);
   Operation_OperationData* data1;
   data1=op1.mutable_data();
   data1->set_data_type(Operation_OperationData_OperationDataType_ELLIPSE);
   data1->set_thinkness(2);
   data1->set_color(4);
   data1->set_a(100);
   data1->set_b(100);
   data1->set_fill(true);
   data1->set_alpha(0);
   Operation_OperationData_Point* point3;
   point3=data1->mutable_center();
   point3->set_x(100);
   point3->set_y(100);
   d->Draw(op1, 2);
   Operation op2;
   op2.set_serial_number(3);
   Operation_OperationData *data2;
   data2=op2.mutable_data();
   data2->set_data_type(Operation_OperationData_OperationDataType_RECTANGE);
   data2->set_thinkness(1);
   data2->set_color(3);
   data2->set_fill(true);
   Operation_OperationData_Point*  start_point;
   Operation_OperationData_Point*  end_point;
   start_point=data2->mutable_top_left_corner();
   end_point=data2->mutable_bottom_right_corner();
   start_point->set_x(100);
   start_point->set_y(100);
   end_point->set_x(250);
   end_point->set_y(250);
   data2->set_alpha(0.9);
   d->Draw(op2, 3);
    
 Operation op3;
   op3.set_serial_number(4);
  Operation_OperationData * data3;
   data3=op3.mutable_data();
   data3->set_data_type(Operation_OperationData_OperationDataType_POINT);
   data3->set_thinkness(5);
   data3->set_color(1);
   Operation_OperationData_Point*  position;
   position=data3->mutable_position();
 position->set_x(100);
 position->set_y(50);
   d->Draw(op3, 5);
   Operation op4;
   op4.set_serial_number(5);
   Operation_OperationData* data4;
   data4=op4.mutable_data();
   data4->set_data_type(Operation_OperationData_OperationDataType_ERASER);
   data4->set_thinkness(1);
   Operation_OperationData_Point* position1;
   position1=data4->mutable_position();
   position1->set_x(90);
   position1->set_y(100);
   d->Draw(op4, 4);
   d->Show();
   d->SaveAsBmp();
   return 0;
}*/
