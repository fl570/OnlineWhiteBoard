/*************************************************************************
     ** File Name: DrawOperation.h
    ** Author: vanvick
    ** Mail: vanvick@163.com
    ** Created Time: Wed Apr 10 16:46:51 2013
    **Copyright [2013] <Copyright vanvick>  [legal/copyright]
 ************************************************************************/


#ifndef KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWOPERATION_H_
#define KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWOPERATION_H_

#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <string>
#include <time.h>
#include "../message.pb.h"
#include "./DrawClass.h"
#include "./DrawRect.h"
#include "./DrawTransRect.h"
#include "./DrawCircle.h"
#include "./DrawTransCircle.h"
#include "./DrawEclipse.h"
#include "./DrawTransEclipse.h"
#include "./DrawEraser.h"
#include "./DrawPoint.h"
#include "./DrawLine.h"

namespace Kingslanding {
namespace OnlineWhiteBoard {
namespace Server {
namespace DrawOperation {
class DrawOperation {
public:
    explicit DrawOperation(std::string meeting_id);
    // ~DrawOperation();
    void Draw(Operation op);
    std::string SaveAsBmp();
    void Show();

private:
    std::string meeting_id_;
    int a_;
    int b_;
    int color_;
    int thinkness_;
    float alpha_;
    int xa_;
    int xb_;
    int ya_;
    int yb_;
    cv::Mat picture_;
    DrawCircle circle;
    DrawTransCircle transCircle;
    DrawEclipse eclipse;
    DrawTransEclipse transEclipse;
    DrawRect rect;
    DrawTransRect transRect;
    DrawLine line;
    DrawPoint point;
    DrawEraser eraser;
};
}  // DrawOperation
}  // Server
}  // OnlineWhiteBoard
}  // Kingslanding
#endif  // KINGSLANDING_ONLINEWHITEBOARD_SERVER_DRAWOPERATION_DRAWOPERATION_H_
