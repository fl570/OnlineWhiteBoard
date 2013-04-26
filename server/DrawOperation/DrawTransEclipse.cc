/*************************************************************************
     ** File Name: DrawTransEclipse.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawTransEclipse.h"
#include "./DrawClass.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
DrawTransEclipse::DrawTransEclipse() {
  color_= 0;
  thinkness_ = 0;
  xa_ = 0;
  ya_ = 0;
  a_ = 0;
  b_ = 0;
  alpha_ = 0;
}
void DrawTransEclipse::set
    (int color, int thinkness, int xa, int ya, int a, int b, float alpha) {
  color_ = color;
  thinkness_ = thinkness;
  xa_ = xa;
  ya_ = ya;
  a_ = a;
  b_ = b;
  alpha_ = alpha;
}

void DrawTransEclipse::Draw(cv::Mat& m) {
  IplImage img = IplImage(m);
  IplImage * pImage = &img;
  IplImage * pTemp = (IplImage*)cvClone(pImage);
  cv::Scalar c = SetColor(color_);
  cv::Point center = cv::Point(xa_, ya_);
  cvEllipse(pTemp, center, cv::Size(a_, b_), 0, 0, 360, c, -1);
  cvAddWeighted(pImage, alpha_, pTemp, 1-alpha_, 0.0, pImage);
  cvReleaseImage(&pTemp);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
