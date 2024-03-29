//
// main.cpp
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
#include <getopt.h>

using namespace std;

static char const* _Basename(char const* fname)
{
    char const* res = strrchr(fname, '/');
    return res ? res + 1 : fname;
}

static void _ReadHelp(const char *progname)
{
    cout << "Ping-pong ball detector\n\n"
            "Description:\n"
            " The program calculates the movement of a tennis ball "
            "over the mesh tennis. \n\n"

            "Synopsis:\n"
            "  " << progname <<" [options] \n\n"
            "Options:\n"
            "  -s,--size\t\tSet minimum ball size (Default size=100)\n"
            "  -d,--debug\t\tSet debug mode\n"
            "  -h,--help\t\tThis help message\n\n"
         << endl;
}

int main(int argc, char** argv)
{
    char const* progname = _Basename(argv[0]);
    int size = 100;
    bool debug = false;
      while (true)
        {
            static struct option long_options[] =
            {
                { "size",          required_argument, 0, 's' },
                { "debug",          no_argument, 0, 'd' },
                { "help",           no_argument,           0, 'h' },
                { 0, 0, 0, 0 }
            };

            int c = 0;
            int option_index = 0;

            c = getopt_long(argc, argv, "s:dh",
                            long_options, &option_index);
            if (c == -1)
                break;

            switch (c)
            {
            case 'h':
                _ReadHelp(progname);
                return 0;

            case 'd':
                debug = true;
                break;

            case 's':
                size = atoi(optarg);
                if (size < 1 || size > 1000)
                {
                    cerr << "Minimum size of the ball! Only 1..1000 range"
                            << endl;
                    return 1;
                }
                break;

            default:
                cerr << "Unknown option '" << c << "'" << endl;
                return 1;
            }
        }

        cBallDetector detector;

        if (size != 100)
            detector.SetBallSize(size);
        if (debug)
            detector.SetDebug();

        detector.BeginDetect();
        cout << "Number is: \t" << detector.GetNumber() << endl;

    return 0;
}