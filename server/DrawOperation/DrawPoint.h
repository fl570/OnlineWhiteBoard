/*************************************************************************
     ** File Name: DrawPoint.h
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_POINT_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_POINT_H_

#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawClass.h"
#include "../common.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawPoint : public DrawClass {
public:
    DrawPoint();
    void set(int color, int thinkness, int xa, int ya);
    void Draw(cv::Mat &m);
private:
#ifdef DEBUG
    FRIEND_TEST(DrawPointTest, set);
    FRIEND_TEST(DrawPointTest, Draw);
#endif
    int color_;
    int thinkness_;
    int xa_;
    int ya_;
};
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWPOINT_H_
