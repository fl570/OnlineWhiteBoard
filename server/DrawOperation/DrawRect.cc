/*************************************************************************
     ** File Name: DrawRect.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawRect.h"
#include "./DrawClass.h"


namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
DrawRect::DrawRect() {
  color_= 0;
  thinkness_ = 0;
  xa_ = 0;
  ya_ = 0;
  xb_ = 0;
  yb_ = 0;
}
void DrawRect::set(int color, int thinkness, int xa, int ya, int xb, int yb) {
  color_ = color;
  thinkness_ = thinkness;
  xa_ = xa;
  ya_ = ya;
  xb_ = xb;
  yb_ = yb;
}

void DrawRect::Draw(cv::Mat& m) {
  cv::Point top_left = cv::Point(xa_, ya_);
  cv::Point bottom_right = cv::Point(xb_, yb_);
  cv::Scalar c = SetColor(color_);
  cv::rectangle(m, top_left, bottom_right, c, thinkness_);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
