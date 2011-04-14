////////////////////////////////////////////////////////////////////////////////
// Filename: MainFrame.cpp
// Author:   Balazs KOVACS
// Year:     2010
//
// Description: Imlementation of MainFrame class.
//
////////////////////////////////////////////////////////////////////////////////

// STL
#include <string.h>
#include <iostream>

// wxWidgets
#include <wx/wx.h>

// other
#include "MainFrame.h"
#include "gui.h"
#include "images/applicationicon.xpm"

// namespaces
using namespace std;

////////////////////////////////////////////////////////////
// Class name:  MainFrame
// Method name: MainFrame
//
// Description: MainFrame class default constructor
////////////////////////////////////////////////////////////
MainFrame::MainFrame(wxWindow* parent, GazeTracker* model)
    : MainFrameBase(parent)
{
    // program icon
    SetIcon(applicationicon_xpm);

    // setting model
    m_model = model;

    // resultframe to null
    m_resultFrame = NULL;
    m_leftEyeFrame = NULL;
    m_rightEyeFrame = NULL;
}


////////////////////////////////////////////////////////////
// Class name:  MainFrame
// Method name: activateRenderLoop
//
// Description: Switches rendering on or off.
////////////////////////////////////////////////////////////
void MainFrame::activateRenderLoop(bool on)
{
    // if not rendering and want to switch on
    if(on /*and !m_model->getCapturing()*/)
    {
        // connect evt. handler
        Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MainFrame::onIdle) );

        // enable rendering in model
        //m_model->setIsRendering(true);
    }
    // if rendering and want to switch off
    else if(!on /*and m_model->getCapturing()*/)
    {
        // disconnect evt. handler
        Disconnect( wxEVT_IDLE, wxIdleEventHandler(MainFrame::onIdle) );

        // disable rendering in model
        //m_model->setIsRendering(false);
    }
}


////////////////////////////////////////////////////////////
// Class name:  MainFrame
// Method name: onIdle
//
// Description: Idle event handler (render loop).
////////////////////////////////////////////////////////////
void MainFrame::onIdle(wxIdleEvent& event)
{
    // process next frame
    m_model->processNextFrame();
       
    // update video frame
    m_resultFrame->m_imagePanel->paintNow();
    m_leftEyeFrame->m_imagePanel->paintNow();
    m_rightEyeFrame->m_imagePanel->paintNow();
        
    // render continously
    event.RequestMore();
}


////////////////////////////////////////////////////////////
// Class name:  MainFrame
// Method name: onButtonWebcam
//
// Description: 'Webcam' button's event handler on menu
//              and toolbar
////////////////////////////////////////////////////////////
void MainFrame::onButtonWebcam(wxCommandEvent& event)
{
    // calculate parameters
    std::string device = std::string(m_textDevice->GetValue().mb_str());
    int width = wxAtoi(m_textWidth->GetValue());
    int height = wxAtoi(m_textHeight->GetValue());

    // start webcam
    m_model->startWebcam(device, width, height);

    // -----
    // destroy previous video frame
    if (m_resultFrame != NULL) 
    {
        m_resultFrame->Destroy();
        m_leftEyeFrame->Destroy();
        m_rightEyeFrame->Destroy();
    }
            
    // -----
    // video frame
    m_resultFrame = new VideoFrame(this, &m_model->m_actualFrame, m_model);
    m_resultFrame->SetTitle(wxT("Tracking Result"));
 
    // left-eye frame
    m_leftEyeFrame = new VideoFrame(this, &m_model->m_leftEyeFrame, m_model);
    m_leftEyeFrame->SetTitle(wxT("Left Eye"));

    // right-eye frame
    m_rightEyeFrame = new VideoFrame(this, &m_model->m_rightEyeFrame, m_model);
    m_rightEyeFrame->SetTitle(wxT("Right Eye"));

    // -----
    // show video frame
    m_resultFrame->Show();
    m_leftEyeFrame->Show();
    m_rightEyeFrame->Show();

    // enable render loop
    activateRenderLoop(true);
}


////////////////////////////////////////////////////////////
// Class name:  MainFrame
// Method name: onButtonStop
//
// Description: 'Stop' button's event handler on menu
//              and toolbar
////////////////////////////////////////////////////////////
void MainFrame::onButtonStop(wxCommandEvent& event)
{
    // close file in model
    m_model->stopWebcam();
    
    // destroy video frame
    m_resultFrame->Destroy();
    m_resultFrame = NULL;

    m_leftEyeFrame->Destroy();
    m_leftEyeFrame = NULL;

    m_rightEyeFrame->Destroy();
    m_rightEyeFrame = NULL;
    
    // disable render loop
    activateRenderLoop(false);
}


////////////////////////////////////////////////////////////
// Class name:  MainFrame
// Method name: onButtonExit
//
// Description: 'Exit' button's event handler (on menu
//              and toolbar).
////////////////////////////////////////////////////////////
void MainFrame::onButtonExit(wxCommandEvent& event)
{
    // if opened
    if (m_resultFrame != NULL)
    {
        // close video frame
        m_resultFrame->Destroy();
        m_leftEyeFrame->Destroy();
        m_rightEyeFrame->Destroy();
    }

    // close application
    Close();
}


////////////////////////////////////////////////////////////
// Class name:  MainFrame
// Method name: onButtonPause
//
// Description: 'Stop' button's event handler (on
//              menu and toolbar).
////////////////////////////////////////////////////////////
void MainFrame::onButtonPause(wxCommandEvent& event)
{
    // status
    bool c = m_toolBar->GetToolState(wxID_PAUSETOOL);
    
    // disable rendering
    activateRenderLoop(c?false:true);
    m_toolBar->ToggleTool(wxID_PAUSETOOL, c?true:false);
}


////////////////////////////////////////////////////////////
// Class name:  MainFrame
// Method name: onButtonAbout
//
// Description: 'About' button's event handler (on menu).
////////////////////////////////////////////////////////////
void MainFrame::onButtonAbout(wxCommandEvent& event)
{
    wxString line1 = wxT("Gaze Tracker application - v0.1\n\n");
    wxString line2 = wxT("Author: Balazs KOVACS\n\n");
    wxString line3 = wxT("Budapest University of Technology and Economics (BUTE)\n");
    wxString line4 = wxT("Faculty of Electrical Engineering and Informatics (VIK)\n");
    wxString line5 = wxT("Department of Control Engineering and Information Technology (IIT)\n\n");
    wxString line6 = wxT("2010");

    wxString aboutstring = line1+line2+line3+line4+line5+line6;

    wxMessageBox(
        aboutstring,
        wxT("About"),
        wxOK,
        this
    );
}

////////////////////////////////////////////////////////////
// Class name:  MainFrame
// Method name: onUpdateUI
//
// Description: asdfadf
////////////////////////////////////////////////////////////
void MainFrame::onUpdateUI(wxUpdateUIEvent& event)
{
    // c=true: before loading
    // c=false: while capturing
    bool c = !m_model->getCapturing();

    // tools
    m_toolBar->EnableTool(wxID_WEBCAMTOOL, c?true:false);
    m_toolBar->EnableTool(wxID_STOPTOOL, c?false:true);
    m_toolBar->EnableTool(wxID_PAUSETOOL, c?false:true);

    // menuitems
    m_menuFile->FindItemByPosition(0)->Enable(c?true:false);
    m_menuFile->FindItemByPosition(1)->Enable(c?false:true);

    // textcontrols
    m_textDevice->Enable(c?true:false);
    m_textWidth->Enable(c?true:false);
    m_textHeight->Enable(c?true:false);
}
