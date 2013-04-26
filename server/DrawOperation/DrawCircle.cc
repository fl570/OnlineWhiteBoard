/*************************************************************************
     ** File Name: DrawCircle.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawCircle.h"
#include "./DrawClass.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
DrawCircle::DrawCircle() {
  color_= 0;
  thinkness_ = 0;
  xa_ = 0;
  ya_ = 0;
  r_ = 0;
}
void DrawCircle::set(int color, int thinkness, int xa, int ya, int r) {
  color_ = color;
  thinkness_ = thinkness;
  xa_ = xa;
  ya_ = ya;
  r_ = r;
}

void DrawCircle::Draw(cv::Mat& m) {
  cv::Point center = cv::Point(xa_, ya_);
  cv::Scalar c = SetColor(color_);
  cv::circle(m, center, r_, c, thinkness_);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
