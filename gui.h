///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gui__
#define __gui__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_CLOSEMENU 1000
#define wxID_EXITMENU 1001
#define wxID_WEBCAMTOOL 1002
#define wxID_STOPTOOL 1003
#define wxID_PAUSETOOL 1004
#define wxID_EXITTOOL 1005

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menuBar;
		wxMenu* m_menuFile;
		wxMenu* m_menuAbout;
		wxToolBar* m_toolBar;
		wxStatusBar* m_statusBar;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textDevice;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textWidth;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textHeight;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onButtonWebcam( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void onUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void onButtonPause( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Gaze Tracker"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 230,250 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class VideoFrameBase
///////////////////////////////////////////////////////////////////////////////
class VideoFrameBase : public wxFrame 
{
	private:
	
	protected:
	
	public:
		
		VideoFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxCAPTION|wxTAB_TRAVERSAL );
		~VideoFrameBase();
	
};

#endif //__gui__
