/* 
 * File:   BallDetector.h
 * Author: morwin
 *
 * Created on 12 березня 2012, 15:26
 */

#pragma once

#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

class cBallDetector {
public:
    cBallDetector();
    ~cBallDetector();
    bool BeginDetect();
    int GetNumber();
    void SetBallSize(int);

private:
    bool _InitCamera();
    long long _pointer;
    void _FindBall();
    void _CounterBall();
    void _DrawCircle();
    void _InitFind();
    void _DestractFind();
    void _CheckPosition();


    CvCapture* _capture;
    IplImage* _img;
    IplImage* _gsImage;
    IplImage* _img_gray;
    CvMemStorage* _storage;
    CvPoint2D32f _center;
    CvSeq* _contours;
    float _radius;
    float _center_position;
    float _old_position;
    int _number;
    int _ball_size;
};