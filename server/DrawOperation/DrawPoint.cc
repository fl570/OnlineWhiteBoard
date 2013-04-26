/*************************************************************************
     ** File Name: DrawPoint.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawPoint.h"
#include "./DrawClass.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
DrawPoint::DrawPoint() {
  color_= 0;
  thinkness_ = 0;
  xa_ = 0;
  ya_ = 0;
}
void DrawPoint::set(int color, int thinkness, int xa, int ya) {
  color_ = color;
  thinkness_ = thinkness;
  xa_ = xa;
  ya_ = ya;
}

void DrawPoint::Draw(cv::Mat& m) {
  cv::Point start_point = cv::Point(xa_, ya_);
  cv::Point end_point = cv::Point(xa_, ya_);
  cv::Scalar c = SetColor(color_);
  cv::line(m, start_point, end_point, c, thinkness_);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
