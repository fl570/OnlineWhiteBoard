/*************************************************************************
     ** File Name: DrawEraser.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawEraser.h"
#include "./DrawClass.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
DrawEraser::DrawEraser() {
  thinkness_ = 0;
  xa_ = 0;
  ya_ = 0;
}
void DrawEraser::set(int thinkness, int xa, int ya) {
  thinkness_ = thinkness;
  xa_ = xa;
  ya_ = ya;
}

void DrawEraser::Draw(cv::Mat& m) {
  cv::Point start_point = cv::Point(xa_, ya_);
  cv::Point end_point = cv::Point(xa_, ya_);
  cv::line(m, start_point, end_point, cv::Scalar(255, 255, 255), thinkness_);
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
