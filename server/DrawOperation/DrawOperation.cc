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
DEFINE_int32(size_of_screen, 3, "the size of screen");

DrawOperation::DrawOperation(std::string meeting_id) {
  meeting_id_ = meeting_id;
  is_start_ = true;
  a_ = 0;
  b_ = 0;
  color_ = 0;
  thinkness_ = 0;
  alpha_ = 0;
  temp_xa_ = 0;
  temp_ya_ = 0;
  temp_xb_ = 0;
  temp_yb_ = 0;
  temp_xc_ = 0;
  temp_yc_ = 0;
  xa_ = 0;
  ya_ = 0;
  xb_ = 0;
  yb_ = 0;
  serial_num_ = 0;
  width_ = 1024*FLAGS_size_of_screen;
  height_ =728*FLAGS_size_of_screen;
  picture_ = cv::Mat(height_, width_, CV_8UC3, cv::Scalar(255, 255, 255));
}

DrawOperation::~DrawOperation(){
  std::string str = "rm -f "+FLAGS_picture_store_address+"temp/*"+meeting_id_+"-*.jpg";
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
	  LOG(ERROR)<<"a"<<data->fill();
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
      LOG(ERROR)<<"data->fill"<<data->fill();
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
      is_start_ = data->is_start();
      if (is_start_) {
        color_ = data->color();
        thinkness_ = data->thinkness();
        xa_ = data->mutable_position()->x();
        ya_ = data->mutable_position()->y();
        point.set(color_, thinkness_, xa_, ya_);
        temp_xa_ = xa_;
        temp_ya_ = ya_;
	temp_xb_ = xa_;
	temp_yb_ = ya_;
	temp_xc_ = xa_;
	temp_yc_ = ya_;
        boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
        point.Draw(picture_);
        lock.unlock();
        lock.release();
      }else {
	temp_xa_ = temp_xb_;
	temp_ya_ = temp_yb_;
	temp_xb_ = temp_xc_;
	temp_yb_ = temp_yc_;
        color_ = data->color();
	thinkness_ = data->thinkness();
	temp_xc_ = data->mutable_position()->x();
	temp_yc_ = data->mutable_position()->y();
	point.set1(color_, thinkness_, temp_xa_, temp_ya_, temp_xb_, temp_yb_, temp_xc_, temp_yc_);
	boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
	point.DrawLine(picture_);
	lock.unlock();
	lock.release();
      }
      break;
    }
    case Operation_OperationData_OperationDataType_ERASER: {
      is_start_ = data->is_start();
      if(is_start_) {
      thinkness_ = data->thinkness();
      xa_ = data->mutable_position()->x();
      ya_ = data->mutable_position()->y();
      eraser.set(thinkness_, xa_, ya_);
      temp_xa_ = xa_;
      temp_ya_ = ya_;
      temp_xb_ = xa_;
      temp_yb_ = ya_;
      temp_xc_ = xa_;
      temp_yc_ = ya_;
      boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
      eraser.Draw(picture_);
      lock.unlock();
      lock.release();
      }else {
	temp_xa_ = temp_xb_;
	temp_ya_ = temp_yb_;
	temp_xb_ = temp_xc_;
	temp_yb_ = temp_yc_;
	thinkness_ = data->thinkness();
	temp_xc_ = data->mutable_position()->x();
	temp_yc_ = data->mutable_position()->y();
	eraser.set1(thinkness_, temp_xa_, temp_ya_, temp_xb_, temp_yb_, temp_xc_, temp_yc_);
	boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
	eraser.DrawLine(picture_);
	lock.unlock();
	lock.release();
      }
      break;
    }
  }
}

IplImage* DrawOperation::zoom(IplImage* image, int rows , int cols ){
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

void DrawOperation::Show() {
  cv:: imshow("display", picture_);
  cv::waitKey(0);
}

bool DrawOperation::Load(const std::string& path, unsigned int serial_num) {
  boost::unique_lock<boost::shared_mutex> lock(draw_mutex_);
  serial_num_ = serial_num;
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
  IplImage  img = IplImage(picture_);
  IplImage *pImage = &img;
  if (i == 1) {
    path = FLAGS_picture_store_address+meeting_id_+"-"+now_time+".jpg";
    IplImage *history_image = zoom(pImage, 90, 120);
    cvSaveImage((path+".jpg").c_str(), history_image);
  } else if (i == 2) {
    path = FLAGS_picture_store_address+"/temp/"+meeting_id_+"-"+now_time+".jpg";
  }
  cvSaveImage(path.c_str(), pImage);
  //cvReleaseImage(&pImage);
  PathInfo path_info;
  path_info.number = serial_num_;
  path_info.path = path;
  //if(!temp)
    //path_info.path = "";
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
