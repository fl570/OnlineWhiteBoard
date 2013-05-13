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
    void set1(int thinkness, int xa, int ya, int xb, int yb, int xc, int yc);
    void Draw(cv::Mat &m);
    void DrawLine(cv::Mat &m);
private:
    CvPoint3D32f pointAdd(CvPoint3D32f p, CvPoint3D32f q);
    CvPoint3D32f pointTimes(float c, CvPoint3D32f p);
    CvPoint3D32f Bernstein(float u, CvPoint3D32f *p);
#ifdef DEBUG
    FRIEND_TEST(DrawEraserTest, set);
    FRIEND_TEST(DrawEraserTest, Draw);
#endif
      
    int thinkness_;
    int xa_;
    int ya_;
    int xb_;
    int yb_;
    int xc_;
    int yc_;
};
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWERASER_H_
