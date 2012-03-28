//
// BallDetector.cpp
//
//     Created: 20.03.2012
//      Author: Sakhnik
//
// This file is part of Ball Detector program.
//
//  Ball Detector program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Asf Player License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
//  Ball Detector program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with  Ball Detector program.  If not, see <http://www.gnu.org/licenses/>

#include "BallDetector.h"
#include <iostream>

using namespace std;

cBallDetector::cBallDetector()
                    : _radius(0.0)
                    , _center_position(false)
                    , _old_position(0.0)
                    , _number(0)
                    , _ball_size(0)
                    , _debug(false)
{
}

cBallDetector::~cBallDetector()
{
    cvReleaseCapture(&_capture);
    cvDestroyWindow("MainCamera");
    if (_debug)
        cvDestroyWindow("DebugCamera");
    cvReleaseImage(&_img);
    cvReleaseImage(&_gsImage);
    cvReleaseImage(&_img_gray);
    cvReleaseMemStorage(&_storage);
}

bool cBallDetector::_InitCamera()
{
    _capture = cvCreateCameraCapture(-1);
    assert(_capture);

    if (!_capture)
        return false;

    cvNamedWindow("MainCamera", 1);
    if (_debug)
    {
        cvNamedWindow("DebugCamera", 1);
        cvMoveWindow("DebugCamera", 100, 100);
    }
    _center_position = cvGetCaptureProperty(_capture,
                                            CV_CAP_PROP_FRAME_WIDTH) / 2;

    return true;
}

// check ball position
void cBallDetector::_CheckPosition()
{
    if ((_old_position  > _center_position && _center.x < _center_position)
        || (_old_position  < _center_position && _center.x > _center_position))
    {
        _number++;
        _old_position = _center.x;
        if (_debug)
        {
            cerr << "The ball flew through the grid  " << _number << " times" << endl;
            cerr << "Position is: " << _center.x << ";" << _center.y << endl;
        }
    }
}

bool cBallDetector::BeginDetect()
{
    if (!_InitCamera())
        return false;

    int c;

     for (;;)
    {
        _img = cvQueryFrame(_capture);
        if (!_img)
            break;
        cout <<  cvGetCaptureProperty( _capture, CV_CAP_PROP_FPS) << endl;
//copy frame in gray style
        _gsImage = cvCreateImage(cvGetSize(_img), 8, 3);
        cvCopy(_img, _gsImage);

         _CheckPosition();
        _ScanFrame();
        cvShowImage("MainCamera", _img);
        c = cvWaitKey(10);
        if (c == 27 || c == 'q' || c == 'Q')
            break;
    }
    cvReleaseImage(&_img);

    return true;
}

//Find ball on frame
void cBallDetector::_ScanFrame()
{
    //get access for all points
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
    if (_debug)
            cvShowImage("DebugCamera", _gsImage);

    if (_FindBall() && _debug)
        cerr << "Ball detected\n radius is:" << _radius << endl;

    _DrawCircle();

    cvReleaseImage(&_gsImage);
}
// Create new grayStyle image and finding conturs
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

void cBallDetector::_DestructFind()
{
    cvReleaseMemStorage(&_storage);
    cvReleaseImage(&_img_gray);
}
// This section find ball with minimum radius
// and return false when ball not found
bool cBallDetector::_FindBall()
{
    _InitFind();
    CvSeq* h_next = 0;
    CvSeq* contur = _contours;

    if (contur == 0)
    {
        _DestructFind();
        return false;
    }

    for( ; contur!=0; contur=contur->h_next )
    {
        if (contur != _contours)
            if (h_next->total >= contur->total)
            {
                h_next->h_next = h_next->h_next->h_next;
                continue;
            }
        h_next = contur;
    }
    _center.x = -1;
    if (h_next->total < _ball_size)
    {
        _DestructFind();
        return false;
    }

    cvDrawContours(_gsImage, h_next, CV_RGB(255, 0, 0),
                                CV_RGB(0, 255, 0), 2, 2, CV_AA, cvPoint(0, 0));
    cvMinEnclosingCircle(h_next, &_center, &_radius);
    _DestructFind();

    return true;
}
// we draw red circle
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

void cBallDetector::SetDebug()
{
    _debug = true;
}