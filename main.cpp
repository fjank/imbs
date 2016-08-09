/*
 *  IMBS Background Subtraction Library
 *  Copyright 2012 Domenico Daniele Bloisi
 *
 *  This file is part of IMBS and it is distributed under the terms of the
 *  GNU Lesser General Public License (Lesser GPL)
 *
 *  
 *
 *  IMBS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IMBS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with IMBS.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  This file contains the C++ OpenCV based implementation for
 *  IMBS algorithm described in
 *  
 *  D. D. Bloisi and L. Iocchi
 *  "Independent Multimodal Background Subtraction"
 *  In Proc. of the Third Int. Conf. on Computational Modeling of Objects
 *  Presented in Images: Fundamentals, Methods and Applications, pp. 39-44, 2012.
 *  Please, cite the above paper if you use IMBS.
 *  
 *
 *  IMBS has been written by Domenico Daniele Bloisi
 *
 *  Please, report suggestions/comments/bugs to
 *  domenico.bloisi@gmail.com
 *
 */

//C
#include <stdio.h>
//C++
#include <iostream>
//OpenCV
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "imagemanager.h"

//imbs
#include "imbs.hpp"

#include<time.h>
#include<stdio.h>
#include <limits.h> 

using namespace cv;
using namespace std;

/**
 * Global variables
 */
Mat frame;     //current frame
Mat fgMask;    //fg mask generated by IMBS method
Mat bgImage;   //bg model generated by IMBS method
double fps;   //frame per second for the input video sequence
int keyboard;  //input from keyboard

/**
 * Function Headers
*/
void help();
void processVideo(char* videoFilename);
void processImages(char* firstFrameFilename);
void onmouse(int event, int x, int y, int flags, void* data);

/**
* @function help
*/
void help()
{
    cout
    << "--------------------------------------------------------------------------" << endl
    << "IMBS Background Subtraction Library "                                       << endl
    << "This file main.cpp contains an example of usage for"                        << endl
    << "IMBS algorithm described in"                                                << endl
    << "D. D. Bloisi and L. Iocchi"                                                 << endl
    << "\"Independent Multimodal Background Subtraction\""                          << endl
    << "In Proc. of the Third Int. Conf. on Computational Modeling of Objects"      << endl
    << "Presented in Images: Fundamentals, Methods and Applications,"               << endl
    << "pp. 39-44, 2012."                                                           << endl
                                                                                    << endl
    << "written by Domenico D. Bloisi"                                              << endl
    << "domenico.bloisi@gmail.com"                                                  << endl
    << "--------------------------------------------------------------------------" << endl
    << "You can process both videos (-vid) and images (-img)."                      << endl
                                                                                    << endl
    << "Usage:"                                                                     << endl
    << "imbs {-vid <video filename>|-img <image filename> [-fps <value>]}"          << endl
    << "for example: imbs -vid video.avi"                                           << endl
    << "or: imbs -img /data/images/1.png"                                           << endl
    << "or: imbs -img /data/images/1.png -fps 7"                                    << endl
    << "--------------------------------------------------------------------------" << endl
                                                                                    << endl;
}

/**
* @function main
*/
int main(int argc, char* argv[])
{

	//print help information
	help();

	//check for the input parameter correctness
	if(argc < 3) {
		cerr <<"Incorrect input list" << endl;
		cerr <<"exiting..." << endl;
		return EXIT_FAILURE;
	}

	//create GUI windows
	namedWindow("Frame");
	namedWindow("FG Mask");
	namedWindow("BG Model");

	if(strcmp(argv[1], "-vid") == 0) {
		//input data coming from a video
		processVideo(argv[2]);
	}
	else if(strcmp(argv[1], "-img") == 0) {
		//input data coming from a sequence of images
		if (argc > 4) {
			if (strcmp(argv[3], "-fps") == 0) {
				fps = atof(argv[4]);
			}
			else {
				fps = 25.;
			}
		}
		else {
			fps = 25.;
		}
		processImages(argv[2]);
	}
	else {
		//error in reading input parameters
		cerr <<"Please, check the input parameters." << endl;
		cerr <<"Exiting..." << endl;
		return EXIT_FAILURE;
	}
	//destroy GUI windows
	destroyAllWindows();
	return EXIT_SUCCESS;
}

/**
* @function processVideo
*/
void processVideo(char* videoFilename) {
    //create the capture object
    VideoCapture capture(videoFilename);
    if(!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open video file: " << videoFilename << endl;
        exit(EXIT_FAILURE);
    }
	time_t start, end;
	int counter = 0;
    //IMBS Background Subtractor
    BackgroundSubtractorIMBS* pIMBS;
    fps = capture.get(5); //CV_CAP_PROP_FPS
    //fps = capture.get(cv::CV_CAP_PROP_FPS);


    if(fps != fps) { //check for nan value
	fps = 25.;
    }

    pIMBS = new BackgroundSubtractorIMBS(fps);
	//string bgfilename = "bg.txt";
	//pIMBS->saveBg(&bgfilename);

    setMouseCallback("FG Mask", onmouse, pIMBS);


    //read input data. ESC or 'q' for quitting
    while( (char)keyboard != 'q' && (char)keyboard != 27 ){
        //read the current frame
        if(!capture.read(frame)) {
            cerr << "Unable to read next frame." << endl;
            cerr << "Exiting..." << endl;
            exit(EXIT_FAILURE);
        }

		if (counter == 0){
			time(&start);
		}

		//get the fgmask and update the background model
        pIMBS->apply(frame, fgMask);

		time(&end);
		++counter;
		double sec = difftime(end, start);
		double fps = counter / sec;
		if (counter > 30)
			cout << "FPS: " << fps << endl;
		// overflow protection
		if (counter == (INT_MAX - 1000))
			counter = 0;
		

        //get background image
        pIMBS->getBackgroundImage(bgImage);
        //get the frame number and write it on the current frame
        stringstream ss;
        rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
                  cv::Scalar(255,255,255), -1);
        ss << capture.get(1); //CV_CAP_PROP_POS_FRAMES
        string frameNumberString = ss.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask", fgMask);
        imshow("BG Model", bgImage);
        //get the input from the keyboard
        keyboard = waitKey( 30 );
    }
    //delete capture object
    capture.release();
}

/**
* @function processImages
* WARNING: this function can read only image sequences in the form
* <n>.<ext>
* where <n> is a number without zeros as prefix, e.g., 7
* and <ext> is the extension provided by command line, e.g., png, jpg, etc.
* Example of sequence:
*             1.png, 2.png, ..., 15.png, 16.png, ..., 128.png, 129.png, ...
*/
void processImages(char* firstFrameFilename) {	

	string foldername(firstFrameFilename);
	size_t folder_index = foldername.find_last_of("/");
    if(folder_index == string::npos) {
    	folder_index = foldername.find_last_of("\\");
    }
    foldername = foldername.substr(0,folder_index+1);
    
	ImageManager *im = new ImageManager(foldername);


    //read the first file of the sequence
	string s = im->next();

    frame = imread(foldername+s);
//fistFrameFilename);
    if(!frame.data){
        //error in opening the first image
        cerr << "Unable to open first image frame: " << foldername+s << endl;
        exit(EXIT_FAILURE);
    }
    //IMBS Background Subtractor
    BackgroundSubtractorIMBS* pIMBS;
    pIMBS = new BackgroundSubtractorIMBS(fps);
    
    //read input data. ESC or 'q' for quitting
    while( (char)keyboard != 'q' && (char)keyboard != 27 ){
    	//update the background model
    	pIMBS->apply(frame, fgMask);
    	//get background image
    	pIMBS->getBackgroundImage(bgImage);
        //get the frame number and write it on the current frame
        
        
		ostringstream oss;
        oss << im->getCount();
        string frameNumberString = oss.str();


        rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
                  cv::Scalar(255,255,255), -1);
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask", fgMask);
        imshow("BG Model", bgImage);
        //get the input from the keyboard
        keyboard = waitKey( 30 );
        //search for the next image in the sequence
        
        
        //read the next frame
        s = im->next();
        frame = imread(foldername+s);
        if(!frame.data){
            //error in opening the next image in the sequence
            cerr << "Unable to open image frame: " << foldername+s << endl;
            exit(EXIT_FAILURE);
        }
        
    }
}

void onmouse(int event, int x, int y, int flags, void* data)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        //cout << "mouse pointer position (" << x << ", " << y << ")" << endl;
        BackgroundSubtractorIMBS* pIMBS = (BackgroundSubtractorIMBS*) data;

        Mat bgImg;
        pIMBS->getBackgroundImage(bgImg);

        BackgroundSubtractorIMBS::BgModel bgValue = pIMBS->getBgModelValue(y*bgImg.cols + x);
        
        int i = 0;
	while(bgValue.isValid[i]) {
            cout << bgValue.values[i] << " isfg:" << bgValue.isFg[i] << " counter:" << (int)bgValue.counter[i] << endl;
            i++;
        }
    }
}

