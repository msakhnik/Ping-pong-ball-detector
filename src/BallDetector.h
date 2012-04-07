//
// BallDetector.h
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
    void SetDebug();

private:
    bool _InitCamera();
    void _ScanFrame();
    bool _FindBall();
    void _DrawCircle();
    void _InitFind();
    void _DestructFind();
    void _CheckPosition();
    void _TransformColor();


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
    bool _debug;
};