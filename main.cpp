////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
// Author:   Balazs KOVACS
// Year:     2010
//
// Description: Application definition and program entry point.
//
////////////////////////////////////////////////////////////////////////////////

// wxWidgets
#include <wx/wx.h>

// others
#include "MainFrame.h"
#include "GazeTracker.h"


////////////////////////////////////////////////////////////
// Class:        PupilTracker
// Derived from: wxApp
//
// Description: Main application class
////////////////////////////////////////////////////////////
class GazeTrackerApp : public wxApp
{
    public:
	    bool OnInit();
};


// imlement macro
IMPLEMENT_APP(GazeTrackerApp)


////////////////////////////////////////////////////////////
// Class name:  GazeTrackerApp
// Method name: OnInit
//
// Description: wxWidgets program entry point.
////////////////////////////////////////////////////////////
bool GazeTrackerApp::OnInit()
{
    // create model
    GazeTracker *model = new GazeTracker();

    // create main frame, with model
    MainFrame *frame = new MainFrame(NULL, model);
    
    // show frame
    frame->Show();
    return true;
}
