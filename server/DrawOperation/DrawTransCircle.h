/*************************************************************************
     ** File Name: DrawTransCircle.h
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWTRANSCIRCLE_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWTRANSCIRCLE_H_

#include<opencv/highgui.h>
#include<opencv2/core/core.hpp>
#include "../message.pb.h"
#include "./DrawClass.h"
#include "../common.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawTransCircle : public DrawClass {
public:
    DrawTransCircle();
    void set(int color, int thinkness, int xa, int ya, int r, float alpha);
    void Draw(cv::Mat &m);
private:
#ifdef DEBUG
    FRIEND_TEST(DrawTransCircleTest, set);
    FRIEND_TEST(DrawTransCircleTest, Draw);
#endif
    int color_;
    int thinkness_;
    int xa_;
    int ya_;
    int r_;
    float alpha_;
};
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWTRANSCIRCLE_H_
