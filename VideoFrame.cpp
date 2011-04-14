////////////////////////////////////////////////////////////////////////////////
// Filename: VideoFrame.cpp
// Author:   Balazs KOVACS
// Year:     2010
//
// Description: Implementation of VideoFrame class.
//
////////////////////////////////////////////////////////////////////////////////

// OpenCV
#include <cv.h>

// wxWidgets
#include <wx/wx.h>

// others
#include "VideoFrame.h"
#include "OwnCVImage.h"
#include "ImagePanel.h"
#include "images/applicationicon.xpm"


////////////////////////////////////////////////////////////
// Class name:  VideoFrame
// Method name: VideoFrame
//
// Description: VideoFrame class default constructor
////////////////////////////////////////////////////////////
VideoFrame::VideoFrame( wxWindow* parent, OwnCVImage* image, GazeTracker* model)
    : VideoFrameBase( parent )
{
    // set window icon
    SetIcon(applicationicon_xpm);

    // initialize image panel
    m_imagePanel = new ImagePanel(this, image, model, this);
 
    // add panel to frame's sizer   
    GetSizer()->Add(m_imagePanel, 1, wxEXPAND | wxALL, 5);
}
