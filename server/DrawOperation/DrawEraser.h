/*************************************************************************
     ** File Name: DrawEraser.h
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_ERASER_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_ERASER_H_

#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawClass.h"
#include "../common.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawEraser : public DrawClass {
public:
    DrawEraser();
    void set(int thinkness, int xa, int ya);
    void Draw(cv::Mat &m);
private:
#ifdef DEBUG
    FRIEND_TEST(DrawEraserTest, set);
    FRIEND_TEST(DrawEraserTest, Draw);
#endif
      
    int thinkness_;
    int xa_;
    int ya_;
};
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWERASER_H_
