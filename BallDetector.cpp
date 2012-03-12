/* 
 * File:   BallDetector.cpp
 * Author: morwin
 * 
 * Created on 12 березня 2012, 15:26
 */

#include "BallDetector.h"
#include <iostream>

using namespace std;

cBallDetector::cBallDetector()
                    : _pointer(0)
                    , _radius(0.0)
                    , _center_position(false)
                    , _old_position(0.0)
                    , _number(0)
{
}

cBallDetector::~cBallDetector()
{
    cvWaitKey(0);
    cvReleaseCapture(&_capture);
    cvDestroyWindow("Demo");
}

bool cBallDetector::_InitCamera()
{
    _capture = cvCreateCameraCapture(-1);
    assert(_capture);

    if (!_capture)
        return false;

    cvNamedWindow("Demo", 1);
    _center_position = cvGetCaptureProperty(_capture,
                                            CV_CAP_PROP_FRAME_WIDTH) / 2;

    return true;
}

void cBallDetector::_CheckPosition()
{
    if ((_old_position  > _center_position && _center.x < _center_position)
        || (_old_position  < _center_position && _center.x > _center_position))
    {
        _number++;
        _old_position = _center.x;
    }
}

bool cBallDetector::BeginDetect()
{
    if (!_InitCamera())
        return false;

    int c;

     for (;;)
    {
        _img = 0;
        _img = cvQueryFrame(_capture);
        if (!_img)
            break;

        _gsImage = cvCreateImage(cvGetSize(_img), 8, 3);
        cvCopy(_img, _gsImage);

         _CheckPosition();
        _FindBall();

            cvShowImage("Demo", _img);

        c = cvWaitKey(50);
        if ((char) c == 27)
            break;

        _pointer++;
    }


    return true;
}

void cBallDetector::_FindBall()
{
    uchar* ptr1;
    ptr1 = (uchar*) (_gsImage->imageData);

    for (int i = 0; i < _gsImage->height; i++)
        for (int j = 0; j < _gsImage->width; j++)
        {
            if (ptr1[j * 3 + 2 + i * _gsImage->widthStep] > 1.5 *
                ptr1[j * 3 + 1 + i * _gsImage->widthStep] &&
                ptr1[j * 3 + 2 + i * _gsImage->widthStep] > 2 *
                ptr1[j * 3 + i * _gsImage->widthStep])
            {
                ptr1[j * 3 + i * _gsImage->widthStep] = 255;
                ptr1[j * 3 + 1 + i * _gsImage->widthStep] = 255;
                ptr1[j * 3 + 2 + i * _gsImage->widthStep] = 255;
            }
            else
            {
                ptr1[j * 3 + i * _gsImage->widthStep] = 0;
                ptr1[j * 3 + 1 + i * _gsImage->widthStep] = 0;
                ptr1[j * 3 + 2 + i * _gsImage->widthStep] = 0;
            }
        }

    _CounterBall();

    _DrawCircle();

    cvReleaseImage(&_gsImage);
}

void cBallDetector::_InitFind()
{
    _img_gray = cvCreateImage(cvSize(_gsImage->width, _gsImage->height), 8, 1);
    _contours = 0;
    _storage = cvCreateMemStorage(0);
    cvCvtColor(_gsImage, _img_gray, CV_BGR2GRAY);
    // REMEMBER: add cvPoint(0, 0) in end if need
    cvFindContours(_img_gray, _storage, &_contours, sizeof (CvContour),
                   CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
}

void cBallDetector::_DestractFind()
{
    cvReleaseMemStorage(&_storage);
    cvReleaseImage(&_img_gray);
}

void cBallDetector::_CounterBall()
{
    _InitFind();
    CvSeq* h_next = 0;
    CvSeq* c = _contours;

    if (c == 0)
    {
        _DestractFind();
        return;
    }

    for( ; c!=0; c=c->h_next )
    {
        if (c != _contours)
            if (h_next->total >= c->total)
            {
                h_next->h_next = h_next->h_next->h_next;
                continue;
            }
        h_next = c;
    }
    _center.x = -1;
    if (h_next->total < _ball_size)
    {
        _DestractFind();
        return;
    }

    cvDrawContours(_gsImage, h_next, CV_RGB(255, 0, 0),
                                CV_RGB(0, 255, 0), 2, 2, CV_AA, cvPoint(0, 0));
    cvMinEnclosingCircle(h_next, &_center, &_radius);
    _DestractFind();
}

void cBallDetector::_DrawCircle()
{
    if (_center.x>-1)
    {
        CvPoint p;
        p.x = _center.x;
        p.y = _center.y;
        cvCircle(_img, p, _radius, CV_RGB(255, 0, 0), 3, 8, 0);
    }
}

int cBallDetector::GetNumber()
{
    return _number;
}

void cBallDetector::SetBallSize(int var)
{
    _ball_size = var;
}