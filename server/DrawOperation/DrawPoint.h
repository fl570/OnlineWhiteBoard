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
    void set1(int color, int thinkness, int xa, int ya, int xb, int yb,int xc, int yc);
    void DrawLine(cv::Mat &m);
    void Draw(cv::Mat &m);
private:
    CvPoint3D32f pointAdd(CvPoint3D32f p, CvPoint3D32f q);
    CvPoint3D32f pointTimes(float c, CvPoint3D32f p);
    CvPoint3D32f Bernstein(float u, CvPoint3D32f *p);
#ifdef DEBUG
    FRIEND_TEST(DrawPointTest, set);
    FRIEND_TEST(DrawPointTest, Draw);
#endif
    int color_;
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
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWPOINT_H_
