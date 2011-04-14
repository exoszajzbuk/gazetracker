////////////////////////////////////////////////////////////////////////////////
// Filename: GazeTracker.cpp
// Author:   Balazs KOVACS
// Year:     2010
//
// Description: Imlementation of GazeTracker class.
//
////////////////////////////////////////////////////////////////////////////////

// STL
#include <iostream>
#include <math.h>
#include <vector>
#include <string.h>
#include <algorithm>

// OpenCV
#include <cv.h>
#include <highgui.h>

// other
#include "GazeTracker.h"

// namespaces
using namespace std;
using namespace cv;

// macros
#define PT_THRES 5

////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: GazeTracker
//
// Description: GazeTracker class default constructor
////////////////////////////////////////////////////////////
GazeTracker::GazeTracker()
{
    // default options
    m_capturing = false;
    m_paused = false;

    // fps init
    m_timePrev = 0;
    m_timeNow = 0;
}



////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: loadVideoFile
//
// Description: This method loads a video file and sets
//              corresponding variables.
////////////////////////////////////////////////////////////
bool GazeTracker::startWebcam(string device, int width, int height)
{
    // init webcam
    m_wcam = new WebCamera(device, width, height);
    m_classifier = new CascadeClassifier("haarcascade_eye.xml");

    // initally detect
    m_detect = true;
    m_redetect = 0;

    // initialize and start capturing
    if (!m_capturing)
    {
        m_wcam->Init();
        m_wcam->StartCapturing();

        m_capturing = true;
    }
}


////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: closeVideoFile
//
// Description: This method closes the actually loaded
//              video file.
////////////////////////////////////////////////////////////
void GazeTracker::stopWebcam()
{
    // stop capturing and close
    if (m_capturing)
    {
        m_wcam->StopCapturing();
        m_wcam->Close();

        m_capturing = false;
    }
}


int clamp(double x)
{
    int r = x;      /* round to nearest */

    if (r < 0)         return 0;
    else if (r > 255)  return 255;
    else               return r;
}


////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: processNextFrame
//
// Description: This method processes the next frame in
//              video using the private variables to
//              determine what to do. (e.g. process
//              method, thresholding, blurring, etc).
////////////////////////////////////////////////////////////
bool GazeTracker::processNextFrame()
{
    // grabs next frame
    cv::Mat img = grabFrame();
    cv::Mat grey;

    cvtColor(img, grey, CV_RGB2GRAY);

    // if there's no previous image
    if (m_prevImage.empty())
    {
        std::cout << "empty" << std::endl;
        m_prevImage = grey;
    }

    if (m_detect)
    {
        // detect eye region with cascade classifier
        m_classifier->detectMultiScale(
            img,        // input image
            detected,   // detected features
            1.3,        // scaleFactor
            5,          // minNeighbors
            0,          // flags
            Size()      // minSize
        );

        // only first two eyes matters
        detected.erase(detected.begin()+2, detected.end());

        // clear trackingpoints
        m_points.clear();

        int sum = 0;
        // add pupil points
        for (int i=0; i<detected.size(); i++)
        {
            m_points.push_back(Point2f(detected[i].x+detected[i].width/2, detected[i].y+detected[i].width/2));
            sum += detected[i].width;
        }

        // sort eyes by x coordinate
        std::sort(m_points.begin(), m_points.end(), pointSortPredicate);

        m_eyesize = (sum/detected.size())+15;

        // disable haar detecting
        m_detect = false;     
    }

    // save points to prev, erase current
    m_prevPoints.clear();
    m_prevPoints.resize(m_points.size());
    std::copy(m_points.begin(), m_points.end(), m_prevPoints.begin());
    m_points.clear();
    m_points.resize(m_prevPoints.size());

    // ***===***===***===***===***===***===***===***===***===***===***===***===
    // OPENCV code here

    // optical flow variables
    std::vector<uchar> status(m_prevPoints.size());
    std::vector<float> err(m_prevPoints.size());

    // calculate optical flow
    calcOpticalFlowPyrLK(
        m_prevImage,        // previous image
        grey,               // current image
        m_prevPoints,       // previous point vector
        m_points,           // current point vector
        status,             // point status
        err,                // error vector
        Size(50,50),      // search window size
        0,                  // maxLevel
        TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 20, 0.01), // termination criteria
        0,                  // deriv lambda
        0                   // initial estimations
    );

    // save previous
    m_prevImage = grey;

    // extract eye regions
    cv::Mat tmp, leftEye, leftEyeProc, rightEye, rightEyeProc;
    tmp = img(cv::Rect(m_points[0].x-m_eyesize/2, m_points[0].y-m_eyesize/2, m_eyesize, m_eyesize));
    tmp.copyTo(leftEye);

    tmp = img(cv::Rect(m_points[1].x-m_eyesize/2, m_points[1].y-m_eyesize/2, m_eyesize, m_eyesize));
    tmp.copyTo(rightEye);

    // process eyes
    leftEyeProc = processEye(leftEye);
    rightEyeProc = processEye(rightEye);
    
    // render results to frame    
    for (int i=0; i<m_points.size(); i++)
    {   
        // render circle
        circle(img, m_points[i], 3, Scalar(255,0,0), 1);

        // render rectangle
        rectangle(
            img,
            Point(m_points[i].x-m_eyesize/2, m_points[i].y-m_eyesize/2),
            Point(m_points[i].x+m_eyesize/2, m_points[i].y+m_eyesize/2),
            Scalar(255,0,0),
            1, 8, 0);
    }

    // redetect at every Nth frame
    if (m_redetect++ > 100)
    {
        m_detect = true;
        m_redetect = 0;
    }

    // ***===***===***===***===***===***===***===***===***===***===***===***===

    // render current fps to image
    std::ostringstream o;
    o << "FPS: " << getCurrentFPS();
    cv::putText(img, o.str(), Point(getWidth()-100, 13), FONT_HERSHEY_PLAIN, 1.0, Scalar(0,255,0), 2);

    // notify videoframe
    m_actualFrame.setImage(img);
    m_actualFrame.setChanged(true);

    // notify left-eye frame
    cv::Mat leftEyeBig;
    resize(leftEyeProc, leftEyeBig, Size(), 2, 2);
    m_leftEyeFrame.setImage(leftEyeBig);
    m_leftEyeFrame.setChanged(true);

    // notify left-eye frame
    cv::Mat rightEyeBig;
    resize(rightEye, rightEyeBig, Size(), 2, 2);
    m_rightEyeFrame.setImage(rightEyeBig);
    m_rightEyeFrame.setChanged(true);

    // save time values
    m_timePrev = m_timeNow;
    m_timeNow = getTime();
}


////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: processEye
//
// Description: Process one eye
////////////////////////////////////////////////////////////
cv::Mat GazeTracker::processEye(cv::Mat eye)
{
    // tmps
    cv::Mat back, grey, thresh, morph, blur;

    cvtColor(eye, grey, CV_RGB2GRAY);

    threshold(grey, thresh, 30, 255, THRESH_BINARY);

    cv::Mat element;
    element = getStructuringElement(MORPH_CROSS, Size(2,2));
    morphologyEx(thresh, morph, MORPH_OPEN, element, Point(-1, -1), 2);


    GaussianBlur(thresh, blur, Size(3,3), 0,0);
/*
    std::vector<Vec3f> circles;

                    HoughCircles(
                        blur,  			 // input image
                        circles,		     // output vector of found circles
                        CV_HOUGH_GRADIENT,	 // the '21HT' method
                        1,	     // accumulator resolution
                        1,	     // minimum distance between circles
                        10,	 // param1 (Canny-parameter)
                        10,	 // param2, default=100
                        0,       // minimum radius, default=0
                        0		 // maximum radius, default=0
                    );
*/

    // return normal image
    cvtColor(blur, back, CV_GRAY2RGB);

                    //std::cout << circles.size() << std::endl;
                    // draw detected circles
 /*                   for(size_t i = 0; i < circles.size(); i++)
                    {
                        Point center(
                            cvRound(circles[i][0]), 
                            cvRound(circles[i][1])
                        );
                        int radius = cvRound(circles[i][2]);

                        // draw the circle center
                        circle(back, center, 3, Scalar(0,255,0), -1, 7, 0);

                        // draw the circle outline
                        circle(back, center, radius, Scalar(255,0,0), 1, 8, 0);
                    }
*/

    return back;
}

////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: grabFrame
//
// Description: grabs a new frame from webcam
////////////////////////////////////////////////////////////
cv::Mat GazeTracker::grabFrame()
{
    const unsigned char *buffer = NULL;
    buffer = (const unsigned char*)m_wcam->NextFrame();

    int Y0, Y1, Cb, Cr;            /* gamma pre-corrected input [0;255] */
    int ER0,ER1,EG0,EG1,EB0,EB1;    /* output [0;255] */
    double r0,r1,g0,g1,b0,b1;             /* temporaries */
    double y0,y1, pb, pr;
    unsigned height = 0, width = 0;

	width = getWidth();
	height = getHeight();

    unsigned char bufferOut[width*height*3];

    int k = 0;

    for (unsigned i = 0; i < height; i++)
    {
	    for(unsigned j = 0; j < width / 2; j++)
	    {
	        Y0 = *(buffer++);
	        Cb = *(buffer++);
	        Y1 = *(buffer++);
	        Cr = *(buffer++);

	        y0 = (255 / 219.0) * (Y0 - 16);
	        y1 = (255 / 219.0) * (Y1 - 16);
	        pb = (255 / 224.0) * (Cb - 128);
	        pr = (255 / 224.0) * (Cr - 128);

	        // Generate first pixel
	        r0 = y0 + 1.402 * pr;
	        g0 = y0 - 0.344 * pb - 0.714 * pr;
	        b0 = y0 + 1.772 * pb;

	        // Generate next pixel - must reuse pb & pr as 4:2:2
	        r1 = y1 + 1.402 * pr;
	        g1 = y1 - 0.344 * pb - 0.714 * pr;
	        b1 = y1 + 1.772 * pb;

    	    ER0 = clamp (r0);
	        ER1 = clamp (r1);
	        EG0 = clamp (g0);
	        EG1 = clamp (g1);
	        EB0 = clamp (b0);
	        EB1 = clamp (b1);

            bufferOut[k++] = (unsigned char)ER0;
            bufferOut[k++] = (unsigned char)EG0;
            bufferOut[k++] = (unsigned char)EB0;
            bufferOut[k++] = (unsigned char)ER1;
            bufferOut[k++] = (unsigned char)EG1;
            bufferOut[k++] = (unsigned char)EB1;
        }
    }

     // convert to opencv matrix
    cv::Mat img = cv::Mat(height, width, CV_8UC3, (unsigned char*)bufferOut);
    return img;
}


////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: getTime
//
// Description: This method returns current time in
//              nanoseconds.
////////////////////////////////////////////////////////////
double GazeTracker::getTime()
{
    // structure to store time in
    struct timespec time;

    // get current time from monotonic clock
    clock_gettime(CLOCK_MONOTONIC, &time);

    return ((time.tv_sec * 1000000000)+time.tv_nsec);
}



////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: getCapturing
////////////////////////////////////////////////////////////
bool GazeTracker::getCapturing()
{
    return m_capturing;
}



////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: getWidth
//
// Description: This method returns the video width.
////////////////////////////////////////////////////////////
int GazeTracker::getWidth()
{
    return m_wcam->GetWidth();
}


////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: getWidth
//
// Description: This method returns the video height.
////////////////////////////////////////////////////////////
int GazeTracker::getHeight()
{
    return m_wcam->GetHeight();
}


////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: getCurrentFPS
//
// Description: This method returns the current FPS
//              calculated from previous and current
//              time.
////////////////////////////////////////////////////////////
double GazeTracker::getCurrentFPS()
{
    double fps = 1000/((m_timeNow-m_timePrev)/1000000);
    return fps>0?fps:0.0;
}



////////////////////////////////////////////////////////////
// Class name:  GazeTracker
// Method name: pointSortPredicate
//
// Description: adfasdf
////////////////////////////////////////////////////////////
bool GazeTracker::pointSortPredicate(const cv::Point2f& left, const cv::Point2f& right)
{
    return left.x < right.x;
}

