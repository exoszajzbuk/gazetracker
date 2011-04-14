////////////////////////////////////////////////////////////////////////////////
// Filename: GazeTracker.h
// Author:   Balazs KOVACS
// Year:     2010
//
// Description: Header of GazeTracker class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GazeTracker__
#define __GazeTracker__

// STL
#include <string.h>
#include <time.h>
#include <vector>

// OpenCV
#include <cv.h>
#include <highgui.h>

// others
#include "OwnCVImage.h"
#include "Webcamera.h"



////////////////////////////////////////////////////////////
// Class:       GazeTracker
//
// Description: GazeTracker gives the model of tracking
//              for the GUI.
////////////////////////////////////////////////////////////
class GazeTracker
{
    public:
        // default constructor
        GazeTracker();

        // start and stop webcam
        bool startWebcam(std::string device, int width, int height);
        void stopWebcam();

        // process actual or next frame
        bool processNextFrame();

        // actual image
        OwnCVImage m_actualFrame;
        OwnCVImage m_leftEyeFrame;
        OwnCVImage m_rightEyeFrame;

        // ==========
        // INFOS
       
        // get video width, height and capture status
        int getWidth();
        int getHeight();
        bool getCapturing();

        // get original and current fps
        double getCurrentFPS();

        // get actual time
        double getTimeAct();
        
    private:
        // webcamera object
        WebCamera*	        m_wcam;
        cv::Mat             m_prevImage;

        // haar classifier
        cv::CascadeClassifier* m_classifier;
        bool    m_detect;
        std::vector<cv::Rect> detected;
        int     m_eyesize;
        int     m_redetect;

        // grabs frame
        cv::Mat grabFrame();

        // is capturing, and paused
        bool m_capturing;
        bool m_paused;

        // time variables
        double m_timePrev;
        double m_timeNow;

        // get actual time in nanoseconds
        double getTime();

        // point vectors
        std::vector<cv::Point2f> m_prevPoints;
        std::vector<cv::Point2f> m_points;
        
        // sort predicate        
        static bool pointSortPredicate(const cv::Point2f &left, const cv::Point2f &right);

        // process eyes
        cv::Mat processEye(cv::Mat eye);

};

#endif
