/*************************************************************************
     ** File Name: DrawClass.cc
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawClass.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {

cv::Scalar DrawClass::SetColor(int color) {
  switch (color) {
    case 1:
      return cv::Scalar(0, 0, 0);  // 颜色为黑色
      break;
    case 2:
      return cv::Scalar(255, 0, 0);  // 颜色为蓝色
      break;
    case 3:
      return cv::Scalar(0, 255, 0);  // 颜色为绿色
      break;
    case 4:
      return cv::Scalar(0, 0, 255);  // 颜色为红色
      break;
  }
}
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
