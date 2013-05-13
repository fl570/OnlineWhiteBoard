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
  xb_ = 0;
  yb_ = 0;
  xc_ = 0;
  yc_ = 0;
}
void DrawPoint::set(int color, int thinkness, int xa, int ya) {
  color_ = color;
  thinkness_ = thinkness;
  xa_ = xa;
  ya_ = ya;
}

void DrawPoint::set1(int color, int thinkness, int xa, int ya, int xb, int yb, int xc, int yc) {
  color_ = color;
  thinkness_ = thinkness;
  xa_ = xa;
  ya_ = ya;
  xb_ = xb;
  yb_ = yb;
  xc_ = xc;
  yc_ = yc;
}

CvPoint3D32f DrawPoint::pointAdd(CvPoint3D32f p, CvPoint3D32f q) {
  p.x += q.x;
  p.y += q.y;
  p.z += q.z;
  return p;
}

CvPoint3D32f DrawPoint::pointTimes(float c, CvPoint3D32f p) {
  p.x *= c;
  p.y *= c;
  p.z *= c;
  return p;
}

CvPoint3D32f DrawPoint::Bernstein(float u, CvPoint3D32f* p) {
  CvPoint3D32f a, b, c, r;
  a = pointTimes(pow((1-u),2), p[0]);
  b = pointTimes(2*u*(1-u), p[1]);
  c = pointTimes(pow(u, 2), p[2]);
  r = pointAdd(pointAdd(a, b), c);
  return r;
}


void DrawPoint::DrawLine(cv::Mat& m) {
  IplImage  img = IplImage(m);
  IplImage * pImage = &img;
  CvPoint3D32f control_pts[3];
  CvPoint pt_now, pt_pre;
  control_pts[0].x = (xa_+xb_)/2;
  control_pts[0].y = (ya_+yb_)/2;
  control_pts[1].x = xb_;
  control_pts[1].y = yb_;
  control_pts[2].x = (xb_+xc_)/2;
  control_pts[2].y = (yb_+yc_)/2;
  int div = 50;
  CvPoint3D32f *pControls = control_pts;
  cv::Scalar c = SetColor(color_);
  for(int i = 0; i <= div; i++) {
    float u = (float)i/div;
    CvPoint3D32f newPt = Bernstein(u, pControls);
    pt_now.x = (int)newPt.x;
    pt_now.y = (int)newPt.y;
    if (i > 0) {
      cvLine(pImage, pt_now, pt_pre, c, thinkness_, CV_AA, 0);
    }
    pt_pre = pt_now;
  }
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
