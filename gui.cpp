///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

#include "images/applicationexit16.xpm"
#include "images/applicationexit32.xpm"
#include "images/cameraweb.xpm"
#include "images/helpabout16.xpm"
#include "images/mediaplaybackpause32.xpm"
#include "images/windowclose16.xpm"
#include "images/windowclose32.xpm"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 230,250 ), wxSize( 230,250 ) );
	
	m_menuBar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuItemWebcam;
	m_menuItemWebcam = new wxMenuItem( m_menuFile, wxID_ANY, wxString( wxT("Start webcam") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemWebcam );
	
	wxMenuItem* m_menuItemStop;
	m_menuItemStop = new wxMenuItem( m_menuFile, wxID_CLOSEMENU, wxString( wxT("Stop webcam") ) , wxT("Close current file..."), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemStop->SetBitmaps( wxBitmap( windowclose16_xpm ) );
	#elif defined( __WXGTK__ )
	m_menuItemStop->SetBitmap( wxBitmap( windowclose16_xpm ) );
	#endif
	m_menuFile->Append( m_menuItemStop );
	
	wxMenuItem* m_separator1;
	m_separator1 = m_menuFile->AppendSeparator();
	
	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menuFile, wxID_EXITMENU, wxString( wxT("Exit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Exit Demo Program..."), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemExit->SetBitmaps( wxBitmap( applicationexit16_xpm ) );
	#elif defined( __WXGTK__ )
	m_menuItemExit->SetBitmap( wxBitmap( applicationexit16_xpm ) );
	#endif
	m_menuFile->Append( m_menuItemExit );
	
	m_menuBar->Append( m_menuFile, wxT("&File") );
	
	m_menuAbout = new wxMenu();
	wxMenuItem* m_menuItemAbout;
	m_menuItemAbout = new wxMenuItem( m_menuAbout, wxID_ANY, wxString( wxT("About") ) , wxT("Open 'About' dialog..."), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemAbout->SetBitmaps( wxBitmap( helpabout16_xpm ) );
	#elif defined( __WXGTK__ )
	m_menuItemAbout->SetBitmap( wxBitmap( helpabout16_xpm ) );
	#endif
	m_menuAbout->Append( m_menuItemAbout );
	
	m_menuBar->Append( m_menuAbout, wxT("&About") );
	
	this->SetMenuBar( m_menuBar );
	
	m_toolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_toolBar->AddTool( wxID_WEBCAMTOOL, wxT("tool"), wxBitmap( cameraweb_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxT("Start webcam") );
	m_toolBar->AddTool( wxID_STOPTOOL, wxT("Close Webcam/File"), wxBitmap( windowclose32_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxT("Stop webcam...") );
	m_toolBar->AddSeparator();
	m_toolBar->AddTool( wxID_PAUSETOOL, wxT("Pause"), wxBitmap( mediaplaybackpause32_xpm ), wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxT("Pause tracking...") );
	m_toolBar->AddSeparator();
	m_toolBar->AddTool( wxID_EXITTOOL, wxT("Exit"), wxBitmap( applicationexit32_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxT("Exit demo program...") );
	m_toolBar->Realize();
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	bSizer2->SetMinSize( wxSize( 0,0 ) ); 
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Device"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textDevice = new wxTextCtrl( this, wxID_ANY, wxT("/dev/video1"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textDevice, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textWidth = new wxTextCtrl( this, wxID_ANY, wxT("800"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textWidth, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textHeight = new wxTextCtrl( this, wxID_ANY, wxT("600"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textHeight, 0, wxALL|wxEXPAND, 5 );
	
	bSizer2->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	// Connect Events
	this->Connect( m_menuItemWebcam->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onButtonWebcam ) );
	this->Connect( m_menuItemStop->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onButtonStop ) );
	this->Connect( m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onButtonExit ) );
	this->Connect( m_menuItemAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onButtonAbout ) );
	this->Connect( wxID_WEBCAMTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::onButtonWebcam ) );
	this->Connect( wxID_WEBCAMTOOL, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrameBase::onUpdateUI ) );
	this->Connect( wxID_STOPTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::onButtonStop ) );
	this->Connect( wxID_STOPTOOL, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrameBase::onUpdateUI ) );
	this->Connect( wxID_PAUSETOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::onButtonPause ) );
	this->Connect( wxID_PAUSETOOL, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrameBase::onUpdateUI ) );
	this->Connect( wxID_EXITTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::onButtonExit ) );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onButtonWebcam ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onButtonStop ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onButtonExit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onButtonAbout ) );
	this->Disconnect( wxID_WEBCAMTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::onButtonWebcam ) );
	this->Disconnect( wxID_WEBCAMTOOL, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrameBase::onUpdateUI ) );
	this->Disconnect( wxID_STOPTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::onButtonStop ) );
	this->Disconnect( wxID_STOPTOOL, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrameBase::onUpdateUI ) );
	this->Disconnect( wxID_PAUSETOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::onButtonPause ) );
	this->Disconnect( wxID_PAUSETOOL, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrameBase::onUpdateUI ) );
	this->Disconnect( wxID_EXITTOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameBase::onButtonExit ) );
}

VideoFrameBase::VideoFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* b_mainSizer;
	b_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	this->SetSizer( b_mainSizer );
	this->Layout();
}

VideoFrameBase::~VideoFrameBase()
{
}
