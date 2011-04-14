////////////////////////////////////////////////////////////////////////////////
// Filename: ImagePanel.cpp
// Author:   Balazs KOVACS
// Year:     2010
//
// Description: Implementation of ImagePanel class.
//
////////////////////////////////////////////////////////////////////////////////

// STL
#include <iostream>

// OpenCV
#include <cv.h>

// wxWidgets
#include <wx/wx.h>
#include <wx/glcanvas.h>

// others
#include "ImagePanel.h"
#include "MainFrame.h"

// namespaces
using namespace std;

////////////////////////////////////////////////////////////
// Name:    ImagePanel event table
////////////////////////////////////////////////////////////
#ifndef USE_GL
BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
#else
BEGIN_EVENT_TABLE(ImagePanel, wxGLCanvas)
#endif
    // catch paint events
    EVT_PAINT(ImagePanel::paintEvent)
    //EVT_LEFT_UP(ImagePanel::onLeftClick)
    //EVT_RIGHT_UP(ImagePanel::onRightClick)
END_EVENT_TABLE()


////////////////////////////////////////////////////////////
// Class name:  ImagePanel
// Method name: ImagePanel
//
// Description: ImagePanel class default constructor.
////////////////////////////////////////////////////////////
ImagePanel::ImagePanel(wxWindow* parent, OwnCVImage* image, GazeTracker* model, VideoFrame* parentframe)
#ifndef USE_GL
    : wxPanel(parent)
#else
    : wxGLCanvas(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxT(""))
#endif
{
    // set CV image pointer
    m_cvimage = image;

    // set model pointer
    m_model = model;

    // set parent
    m_parentframe = parentframe;
}


////////////////////////////////////////////////////////////
// Class name:  ImagePanel
// Method name: paintEvent
//
// Description: This method is the paint event handler of
//              ImagePanel class. It's called, when
//              the frame needs to be redrawn.
////////////////////////////////////////////////////////////
void ImagePanel::paintEvent(wxPaintEvent &evt)
{
    wxPaintDC dc(this);
    render(dc);
}
 

////////////////////////////////////////////////////////////
// Class name:  ImagePanel
// Method name: paintNow
//
// Description: This method can be used to manually repaint
//              the ImagePanel.
////////////////////////////////////////////////////////////
void ImagePanel::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}
 

////////////////////////////////////////////////////////////
// Class name:  ImagePanel
// Method name: render
//
// Description: This method renders the actual CV image
//              (if changed) to the panel.
////////////////////////////////////////////////////////////
void ImagePanel::render(wxDC& dc)
{

#ifndef USE_GL
    // WXWIDGETS method

    // if image is changed
    if (m_cvimage->getChanged() == true)
    {
        int imgwidth = m_cvimage->getImage().cols;
        int imgheight = m_cvimage->getImage().rows;

        int framewidth = m_parentframe->GetSize().GetWidth();
        int frameheight = m_parentframe->GetSize().GetHeight();

        // resize frame to image
        if (framewidth != imgwidth || frameheight != imgheight)
        {
            wxSize size = wxSize(imgwidth+10, imgheight+10);
            m_parentframe->SetSize(size);
            m_parentframe->SetMinSize(size);
            m_parentframe->SetMaxSize(size);
        }

        // convert CV image to wxImage
        wxImage wximage = wxImage(
                              m_cvimage->getImage().cols,   // width
                              m_cvimage->getImage().rows,   // height
                              m_cvimage->getImage().data,   // data
                              TRUE);

        // convert wxImage to wxBitmap
        m_bitmap = wxBitmap(wximage);

        // disable changed flag
        m_cvimage->setChanged(false);
    }

    // draw bitmap on panel
    dc.DrawBitmap(m_bitmap, 0, 0, false);

#else
    // OPENGL method

    SetCurrent();
    
    int width = m_cvimage->getImage().cols;
    int height = m_cvimage->getImage().rows;

    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, m_cvimage->getImage().data);

    glFlush();
    SwapBuffers();
#endif
}

/*
////////////////////////////////////////////////////////////
// Class name:  ImagePanel
// Method name: onLeftClick
//
// Description: Left mouse click event handler.
////////////////////////////////////////////////////////////
void ImagePanel::onLeftClick(wxMouseEvent &evt)
{
    m_model->insertPoint((int)evt.GetX(), (int)evt.GetY());
}


////////////////////////////////////////////////////////////
// Class name:  ImagePanel
// Method name: onRightClick
//
// Description: Right mouse click event handler.
////////////////////////////////////////////////////////////
void ImagePanel::onRightClick(wxMouseEvent &evt)
{
    m_model->removePoint((int)evt.GetX(), (int)evt.GetY());
}
*/
