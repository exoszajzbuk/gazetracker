////////////////////////////////////////////////////////////////////////////////
// Filename: ImagePanel.h
// Author:   Balazs KOVACS
// Year:     2010
//
// Description: Header of ImagePanel class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ImagePanel__
#define __ImagePanel__

// using OpenGL canvas
//#define USE_GL

// wxWidgets
#include <wx/wx.h>

#ifdef USE_GL
#include <wx/glcanvas.h>
#endif

// others
#include "OwnCVImage.h"
#include "GazeTracker.h"
#include "VideoFrame.h"

class VideoFrame;

////////////////////////////////////////////////////////////
// Class name:   ImagePanel
// Derived from: wxPanel / wxGLCanvas
//
// Description: ImagePanel class can update itself from
//              a given CV image.
////////////////////////////////////////////////////////////
#ifndef USE_GL
class ImagePanel : public wxPanel
#else
class ImagePanel : public wxGLCanvas
#endif
{
private:
    // the bitmap to display
    wxBitmap      m_bitmap;

    // pointer to CV image
    OwnCVImage*   m_cvimage;

    // pointer to model
    GazeTracker*  m_model;

    // parent frame
    VideoFrame*   m_parentframe;

public:
    // default constructor
    ImagePanel(wxWindow* parent, OwnCVImage* image, GazeTracker* model, VideoFrame* parentframe);
    
    // paint event handler
    void paintEvent(wxPaintEvent& event);

    // method for explicit paint
    void paintNow();
    
    // rendering result
    void render(wxDC& dc);
  
protected:
    DECLARE_EVENT_TABLE()
};

#endif
