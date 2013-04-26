/*************************************************************************
     ** File Name: DrawTransCircle.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawTransCircle.h"
#include "./DrawClass.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
DrawTransCircle::DrawTransCircle() {
  color_= 0;
  thinkness_ = 0;
  xa_ = 0;
  ya_ = 0;
  r_ = 0;
  alpha_ = 0;
}
void DrawTransCircle::set
              (int color, int thinkness, int xa, int ya, int r, float alpha) {
  color_ = color;
  thinkness_ = thinkness;
  xa_ = xa;
  ya_ = ya;
  r_ = r;
  alpha_ = alpha;
}

void DrawTransCircle::Draw(cv::Mat& m) {
  IplImage img = IplImage(m);
  IplImage *pImage = &img;
  IplImage * pTemp = (IplImage*)cvClone(pImage);
  cv::Scalar c = SetColor(color_);
  cv::Point center = cv::Point(xa_, ya_);
  cvCircle(pTemp, center, r_, c, -1);
  cvAddWeighted(pImage, alpha_, pTemp, 1-alpha_, 0.0, pImage);
  cvReleaseImage(&pTemp);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
