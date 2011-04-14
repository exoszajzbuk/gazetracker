////////////////////////////////////////////////////////////////////////////////
// Filename: MainFrame.h
// Author:   Balazs KOVACS
// Year:     2010
//
// Description: Header of MainFrame class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MainFrame__
#define __MainFrame__

// others
#include "GazeTracker.h"
#include "VideoFrame.h"

// forward declarations
class VideoFrame;


////////////////////////////////////////////////////////////
// Class:        MainFrame
// Derived from: MainFrameBase
//
// Description: MainFrame class handles most of GUI calls
//              and events.
////////////////////////////////////////////////////////////
class MainFrame
    : public MainFrameBase
{
public:
    // default constructor
	MainFrame(wxWindow* parent, GazeTracker* model);

protected:
    // button/menu events
    void    onButtonWebcam(wxCommandEvent& event);
    void    onButtonStop(wxCommandEvent& event);
    void    onButtonPause(wxCommandEvent& event);
    void    onButtonExit(wxCommandEvent& event);
    void    onButtonAbout(wxCommandEvent& event);

    // update UI
    void    onUpdateUI(wxUpdateUIEvent& event);

    // idle event handler for rendering
    void    onIdle(wxIdleEvent& event);

private:
    // variables
    GazeTracker*    m_model;        // pointer to model
    VideoFrame*     m_resultFrame;  // pointer to video window
    VideoFrame*     m_leftEyeFrame;
    VideoFrame*     m_rightEyeFrame;

    // methods
    void        activateRenderLoop(bool on);    // activating render-loop 
};

#endif
