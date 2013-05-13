/*************************************************************************
     ** File Name: DrawTransRect.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawTransRect.h"
#include "./DrawClass.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
DrawTransRect::DrawTransRect() {
  color_= 0;
  thinkness_ = 0;
  xa_ = 0;
  ya_ = 0;
  xb_ = 0;
  yb_ = 0;
  alpha_ = 0;
}
void DrawTransRect::set
    (int color, int thinkness, int xa, int ya, int xb, int yb, float alpha) {
  color_ = color;
  thinkness_ = thinkness;
  xa_ = xa;
  ya_ = ya;
  xb_ = xb;
  yb_ = yb;
  alpha_ = alpha;
}

void DrawTransRect::Draw(cv::Mat& m) {
  IplImage img = IplImage(m);
  IplImage * pImage = &img;
  IplImage * pTemp = (IplImage*)cvClone(pImage);
  int width = xb_-xa_;
  int height = yb_-ya_;
  cv::Scalar c = SetColor(color_);
  cv::Point left_corner = cv::Point(xa_, ya_);
  cv::Point right_corner = cv::Point(xb_, yb_);
  cvRectangle(pTemp, left_corner, right_corner, c, -1 );
  //cvAddWeighted(pImage, alpha_, pTemp, 1-alpha_, 0.0, pImage);
  cvAddWeighted(pImage, 1-alpha_, pTemp, alpha_, 0.0, pImage);
  cvReleaseImage(&pTemp);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
