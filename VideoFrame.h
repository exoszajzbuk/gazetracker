////////////////////////////////////////////////////////////////////////////////
// Filename: VideoFrame.h
// Author:   Balazs KOVACS
// Year:     2010
//
// Description: Header of VideoFrame class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __VideoFrame__
#define __VideoFrame__

// OpenCV
#include <cv.h>

// others
#include "gui.h"
#include "ImagePanel.h"

class ImagePanel;

////////////////////////////////////////////////////////////
// Class:        VideoFrame
// Derived from: VideoFrameBase
//
// Description: VideoFrame class contains the result of
//              the processing in a frame.
////////////////////////////////////////////////////////////
class VideoFrame
    : public VideoFrameBase
{
public:
    // default constructor
	VideoFrame(wxWindow* parent, OwnCVImage* image, GazeTracker* model);
	
    // image panel
    ImagePanel* m_imagePanel;
};

#endif
