/*************************************************************************
     ** File Name: DrawCircle.h
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWCIRCLE_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWCIRCLE_H_

#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawClass.h"
#include "../common.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawCircle : public DrawClass {
public:
    DrawCircle();
    void set(int color, int thinkness, int xa, int ya, int r);
    void Draw(cv::Mat &m);
private:
#ifdef DEBUG
    FRIEND_TEST(DrawCircleTest, set);
    FRIEND_TEST(DrawCircleTest, Draw);
#endif
    
    int color_;
    int thinkness_;
    int xa_;
    int ya_;
    int r_;
};
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWCIRCLE_H_
