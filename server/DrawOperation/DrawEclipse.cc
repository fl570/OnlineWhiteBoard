/*************************************************************************
     ** File Name: DrawEclipse.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawEclipse.h"
#include "./DrawClass.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
DrawEclipse::DrawEclipse() {
  color_= 0;
  thinkness_ = 0;
  xa_ = 0;
  ya_ = 0;
  a_ = 0;
  b_ = 0;
}
void DrawEclipse::set(int color, int thinkness, int xa, int ya, int a, int b) {
  color_ = color;
  thinkness_ = thinkness;
  xa_ = xa;
  ya_ = ya;
  a_ = a;
  b_ = b;
}

void DrawEclipse::Draw(cv::Mat& m) {
  cv::Point center = cv::Point(xa_, ya_);
  cv::Scalar c = SetColor(color_);
  cv::ellipse(m, center, cv::Size(a_, b_), 0, 0, 360, c, thinkness_);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
