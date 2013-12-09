///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "fwInterface.h"

#include "fwScreen.h"
#include "fwJobQueue.h"
#include "fwServerConnectionMonitor.h"
#include "fwReportMonitor.h"
#include "fwPacketQueue.h"
#include "fwPeerState.h"
#include "fwPRIMEState.h"
#include "fwReceiverThread.h"
#include "fwInputHandler.h"
#include "fwDecodingThread.h"
#include "fwFrameQueue.h"

///////////////////////////////////////////////////////////////////////////

//Custom Event Types Definitions
DEFINE_LOCAL_EVENT_TYPE(wxEVT_LOG_ENTRY)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_INC_PRIME_MESSAGE)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_OUTPUT_READY)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_BUFFER_UPDATED)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_RTP_SESSION_PROPS)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_NEW_OUTPUT)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_INPUT_READY)

enum
{
	LISTENING_SOCKET_ID,
	SERVER_SOCKET_ID,
    PEER_SOCKET_ID
};

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	//this->Maximize();

	wxBoxSizer* m_MainSizer;
	m_MainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_ScreenSizer;
	m_ScreenSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_Screen = new FLOWScreen( this );
	m_Screen->SetBackgroundColour( *wxBLUE );
	
	m_ScreenSizer->Add( m_Screen, 1, wxEXPAND | wxALL, 5 );
	
	m_PeerList = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), wxTR_DEFAULT_STYLE|wxTR_FULL_ROW_HIGHLIGHT );
	//m_PeerList->SetForegroundColour( wxColour( 128, 255, 255 ) );
	//m_PeerList->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	m_PeerList->SetMinSize( wxSize( 200,-1 ) );
	m_PeerList->SetMaxSize( wxSize( 200,-1 ) );
	
	m_ScreenSizer->Add( m_PeerList, 0, wxALL|wxEXPAND, 5 );
	
	m_MainSizer->Add( m_ScreenSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_LogSizer;
	m_LogSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_LogSizer->SetMinSize( wxSize( -1,200 ) ); 
	m_Log = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,200 ), wxTE_CHARWRAP|wxTE_MULTILINE|wxTE_NOHIDESEL|wxTE_READONLY|wxTE_WORDWRAP|wxVSCROLL );
	//m_Log->SetForegroundColour( wxColour( 128, 255, 255 ) );
	//m_Log->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	m_Log->SetMinSize( wxSize( -1,200 ) );
	m_Log->SetMaxSize( wxSize( -1,200 ) );
	
	m_LogSizer->Add( m_Log, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* m_StatsSizer;
	m_StatsSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_StatsSizer->SetMinSize( wxSize( 200,-1 ) ); 
	wxBoxSizer* m_LeftStats;
	m_LeftStats = new wxBoxSizer( wxVERTICAL );
	
	m_ConnectedPeersLabel = new wxStaticText( this, wxID_ANY, wxT("Connected Peers"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ConnectedPeersLabel->Wrap( -1 );
	m_LeftStats->Add( m_ConnectedPeersLabel, 0, 0, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	m_LeftStats->Add( m_staticText6, 0, 0, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	m_LeftStats->Add( m_staticText5, 0, 0, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	m_LeftStats->Add( m_staticText4, 0, 0, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	m_LeftStats->Add( m_staticText3, 0, 0, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	m_LeftStats->Add( m_staticText8, 0, 0, 5 );
	
	m_PlayStartSeqLabel = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PlayStartSeqLabel->Wrap( -1 );
	m_PlayStartSeqLabel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	m_LeftStats->Add( m_PlayStartSeqLabel, 0, 0, 5 );
	
	m_StatsSizer->Add( m_LeftStats, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_LeftStatsValues;
	m_LeftStatsValues = new wxBoxSizer( wxVERTICAL );
	
	m_staticText15 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	m_LeftStatsValues->Add( m_staticText15, 0, 0, 5 );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	m_LeftStatsValues->Add( m_staticText16, 0, 0, 5 );
	
	m_staticText171 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText171->Wrap( -1 );
	m_LeftStatsValues->Add( m_staticText171, 0, 0, 5 );
	
	m_staticText181 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText181->Wrap( -1 );
	m_LeftStatsValues->Add( m_staticText181, 0, 0, 5 );
	
	m_staticText191 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText191->Wrap( -1 );
	m_LeftStatsValues->Add( m_staticText191, 0, 0, 5 );
	
	m_staticText201 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText201->Wrap( -1 );
	m_LeftStatsValues->Add( m_staticText201, 0, 0, 5 );
	
	m_staticText211 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	m_LeftStatsValues->Add( m_staticText211, 0, 0, 5 );
	
	m_StatsSizer->Add( m_LeftStatsValues, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_RightStats;
	m_RightStats = new wxBoxSizer( wxVERTICAL );
	
	m_RTPPortLabel = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_RTPPortLabel->Wrap( -1 );
	m_RightStats->Add( m_RTPPortLabel, 0, 0, 5 );
	
	m_ReportPortLabel = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ReportPortLabel->Wrap( -1 );
	m_RightStats->Add( m_ReportPortLabel, 0, 0, 5 );
	
	m_staticText17 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	m_RightStats->Add( m_staticText17, 0, 0, 5 );
	
	m_staticText18 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	m_RightStats->Add( m_staticText18, 0, 0, 5 );
	
	m_staticText19 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	m_RightStats->Add( m_staticText19, 0, 0, 5 );
	
	m_staticText20 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	m_RightStats->Add( m_staticText20, 0, 0, 5 );
	
	m_PlayEndSeqLabel = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PlayEndSeqLabel->Wrap( -1 );
	m_RightStats->Add( m_PlayEndSeqLabel, 0, 0, 5 );
	
	m_StatsSizer->Add( m_RightStats, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_RightStatsValues;
	m_RightStatsValues = new wxBoxSizer( wxVERTICAL );
	
	m_RTPPortValueLabel = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_RTPPortValueLabel->Wrap( -1 );
	m_RightStatsValues->Add( m_RTPPortValueLabel, 0, 0, 5 );
	
	m_ReportPortValueLabel = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_ReportPortValueLabel->Wrap( -1 );
	m_RightStatsValues->Add( m_ReportPortValueLabel, 0, 0, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	m_RightStatsValues->Add( m_staticText24, 0, 0, 5 );
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	m_RightStatsValues->Add( m_staticText25, 0, 0, 5 );
	
	m_staticText26 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	m_RightStatsValues->Add( m_staticText26, 0, 0, 5 );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	m_RightStatsValues->Add( m_staticText27, 0, 0, 5 );
	
	m_staticText28 = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	m_RightStatsValues->Add( m_staticText28, 0, 0, 5 );
	
	m_StatsSizer->Add( m_RightStatsValues, 0, wxEXPAND, 5 );
	
	m_LogSizer->Add( m_StatsSizer, 0, wxEXPAND, 5 );
	
	m_MainSizer->Add( m_LogSizer, 0, wxEXPAND, 5 );
	//m_MainSizer->Remove( m_LogSizer );
	m_MainSizer->Hide(m_LogSizer, true);
	this->SetSizer( m_MainSizer );
	this->Layout();

	m_StatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_MenuBar = new wxMenuBar( 0 );
	m_FileMenu = new wxMenu();
	wxMenuItem* m_FileOpen;
	m_FileOpen = new wxMenuItem( m_FileMenu, wxID_ANY, wxString( wxT("Open...") ) , wxEmptyString, wxITEM_NORMAL );
	m_FileMenu->Append( m_FileOpen );
	
	wxMenuItem* m_FileClose;
	m_FileClose = new wxMenuItem( m_FileMenu, wxID_ANY, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	m_FileMenu->Append( m_FileClose );
	m_FileClose->Enable( false );
	
	wxMenuItem* m_FileSeparator;
	m_FileSeparator = m_FileMenu->AppendSeparator();
	
	wxMenuItem* m_FileExit;
	m_FileExit = new wxMenuItem( m_FileMenu, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_FileMenu->Append( m_FileExit );
	
	m_MenuBar->Append( m_FileMenu, wxT("File") ); 
	
	m_LogMenu = new wxMenu();
	wxMenuItem* m_ShowLog;
	m_ShowLog = new wxMenuItem( m_LogMenu, wxID_ANY, wxString( wxT("Show Log") ) , wxEmptyString, wxITEM_CHECK );
	m_LogMenu->Append( m_ShowLog );
	m_ShowLog->Enable( false );
	
	wxMenuItem* m_LogSeparator;
	m_LogSeparator = m_LogMenu->AppendSeparator();
	
	wxMenuItem* m_VideoLog;
	m_VideoLog = new wxMenuItem( m_LogMenu, wxID_ANY, wxString( wxT("Video Log") ) , wxEmptyString, wxITEM_CHECK );
	m_LogMenu->Append( m_VideoLog );
	m_VideoLog->Enable( false );
	
	wxMenuItem* m_SoundLog;
	m_SoundLog = new wxMenuItem( m_LogMenu, wxID_ANY, wxString( wxT("Sound Log") ) , wxEmptyString, wxITEM_CHECK );
	m_LogMenu->Append( m_SoundLog );
	m_SoundLog->Enable( false );
	
	wxMenuItem* m_StreamLog;
	m_StreamLog = new wxMenuItem( m_LogMenu, wxID_ANY, wxString( wxT("Stream Log") ) , wxEmptyString, wxITEM_CHECK );
	m_LogMenu->Append( m_StreamLog );
	m_StreamLog->Enable( false );
	
	wxMenuItem* m_PrimeLog;
	m_PrimeLog = new wxMenuItem( m_LogMenu, wxID_ANY, wxString( wxT("PRIME Log") ) , wxEmptyString, wxITEM_CHECK );
	m_LogMenu->Append( m_PrimeLog );
	m_PrimeLog->Enable( false );
	
	wxMenuItem* m_FlowLog;
	m_FlowLog = new wxMenuItem( m_LogMenu, wxID_ANY, wxString( wxT("FLOW Log") ) , wxEmptyString, wxITEM_CHECK );
	m_LogMenu->Append( m_FlowLog );
	m_FlowLog->Enable( false );
	
	m_MenuBar->Append( m_LogMenu, wxT("Log") ); 
	
	m_AboutMenu = new wxMenu();
	wxMenuItem* m_AboutHelp;
	m_AboutHelp = new wxMenuItem( m_AboutMenu, wxID_ANY, wxString( wxT("Help") ) , wxEmptyString, wxITEM_NORMAL );
	m_AboutMenu->Append( m_AboutHelp );
	
	wxMenuItem* m_AboutInfo;
	m_AboutInfo = new wxMenuItem( m_AboutMenu, wxID_ANY, wxString( wxT("Info") ) , wxEmptyString, wxITEM_NORMAL );
	m_AboutMenu->Append( m_AboutInfo );
	
	m_MenuBar->Append( m_AboutMenu, wxT("About") ); 
	
	this->SetMenuBar( m_MenuBar );
	
	this->Centre( wxBOTH );
	
	this->Centre( wxBOTH );

	m_Screen->SetDoubleBuffered(true);

          m_CBuffer = new PacketQueue;
	
          wxIPV4address addr;
	wxString s( addr.Hostname() );
	m_PeerList->AddRoot(s);
	//m_WorkerPool = new WorkerPool(this);
	m_CSock = NULL;
	m_ReceiverQueue = NULL;
          m_StreamPort = -1;
          m_InputReady = false;

          wxIPV4address LocalAddress;
        wxIPV4address remote;
        remote.Hostname(_T("www.google.com"));
        remote.Service(80);
 
        wxIPV4address local;
 
        wxSocketClient client;
        if(client.Connect(remote)) client.GetLocal(local);
        m_IpAddr = local.IPAddress();

        LocalAddress.Hostname(local.IPAddress());
        //LocalAddress.AnyAddress();
          LocalAddress.Service(0);
 
          m_InReportSocket = new wxDatagramSocket(LocalAddress);
          
          if(m_InReportSocket->Ok())
	{
              m_InReportSocket->GetLocal(LocalAddress);
              //m_InReportSocket->SetNotify(false);
              log(wxString::Format("Port %d %s\n", LocalAddress.Service(), LocalAddress.IPAddress()));
	}

	m_PRIMEState = new PRIMEState;

          m_RTPRecvSocket = NULL;
	m_DecoderQueue = NULL;
	m_ReportQueue = NULL;
	m_ReceiverQueue = NULL;
	m_ConnMonitorQueue = NULL;
	m_DisplayTimer = NULL;
	m_SwarmReportTimer = NULL;

	m_ExitCounter = 0;
	// Connect Events

	// GUI Events
	this->Connect( m_FileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileOpen ) );
	this->Connect( m_FileClose->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileClose ) );
	this->Connect( m_FileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileExit ) );
	this->Connect( m_ShowLog->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnShowLog ) );
	this->Connect( m_VideoLog->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnVideoLog ) );
	this->Connect( m_SoundLog->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnSoundLog ) );
	this->Connect( m_StreamLog->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnStreamLog ) );
	this->Connect( m_PrimeLog->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnPrimeLog ) );
	this->Connect( m_FlowLog->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFlowLog ) );
	this->Connect( m_AboutHelp->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnAboutHelp ) );
	this->Connect( m_AboutInfo->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnAboutInfo ) );
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );

	//Socket Events
	this->Connect( LISTENING_SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnListenEvent ), NULL, this );
	this->Connect( SERVER_SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnServerEvent ), NULL, this );
	this->Connect( PEER_SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnPeerEvent ), NULL, this );

	//Custom Events
	this->Connect( wxEVT_LOG_ENTRY, wxCommandEventHandler( MainFrame::OnLogEntry ), NULL, this );
	this->Connect( wxEVT_SERVER_REPORT, ServerReportEventHandler( MainFrame::OnServerReport ), NULL, this);
	this->Connect( wxEVT_SERVER_PROPERTIES, ServerPropertiesEventHandler( MainFrame::OnServerProperties ), NULL, this);
	this->Connect( wxEVT_NEW_OUTPUT, wxCommandEventHandler( MainFrame::OnNewOutput ), NULL, this );
          this->Connect( wxEVT_INC_PRIME_MESSAGE, wxCommandEventHandler( MainFrame::OnIncMessage ), NULL, this );
          this->Connect( wxEVT_INPUT_READY, wxCommandEventHandler(MainFrame::OnInputReady), NULL, this);

          m_ReportQueue = new JobQueue;
          Job init_j(Job::THREAD_READ_DATA_ID, NULL);
          m_ReportQueue->AddNewJob(init_j);
          ReportMonitor* dect = new ReportMonitor(this, m_ReportQueue, m_InReportSocket);
            if (dect->Create() == wxTHREAD_NO_ERROR) {
                dect->Run();
            }
            else
            {
                log("Server : Unable to start DecodingThread");
            }
}

void MainFrame::ExitCounterInc() {
    wxCriticalSectionLocker locker(m_ExitCounterGuard);
    m_ExitCounter++;
}

void MainFrame::ExitCounterDec() {
    wxCriticalSectionLocker locker(m_ExitCounterGuard);
    m_ExitCounter--;
}

bool MainFrame::ExitCounterCheck() {
    return (m_ExitCounter == 0);
}

void MainFrame::OnMainFrameClose(wxCloseEvent& WXUNUSED(event)) {
    ::google::protobuf::ShutdownProtobufLibrary();
    printf("Yiha!!!!!!\n");
    m_InputReady = false;
    if(m_DecoderQueue) {
        Job j(Job::THREAD_EXIT_ID, NULL);
        m_DecoderQueue->AddNewJob(j);
    }

    if(m_ReportQueue) {
        Job j(Job::THREAD_EXIT_ID, NULL);
        m_ReportQueue->AddNewJob(j);
    }

    if (m_ConnMonitorQueue) {
        Job j(Job::THREAD_EXIT_ID, NULL);
        m_ConnMonitorQueue->AddNewJob(j);
    }

    if(m_ReceiverQueue) {
        //Receiver Thread will safely delete the queue
        Job j(Job::THREAD_EXIT_ID, NULL);
        m_ReceiverQueue->AddNewJob(j);
    } else {
        if(m_CBuffer)
	  delete m_CBuffer;
    }

    if(m_DisplayTimer) 
        delete m_DisplayTimer;

    if(m_SwarmReportTimer)
        delete m_SwarmReportTimer;

    if(m_CSock)
        m_CSock->Destroy();

    if(m_PRIMEState)
        delete m_PRIMEState;

    while(!ExitCounterCheck());

    //delete m_FrameQueue;
    //wxThread::Sleep(1000);
    printf("Yiha!!!!!!\n");
    Destroy();
}

static int counter = 0;
static double luda = 0.008f;
static double start = 0.0f;
static double factor = 0.0f;

void MainFrame::OnDisplayRefresh(wxTimerEvent& evt) {
    double actual_delay, delay, sync_threshold, ref_clock, diff;
    int ret;
    
    VideoFrame vf;
    m_InHandle->m_FrameTimer = av_gettime() / 1000000.0;
    luda = m_InHandle->m_FrameTimer - start;
    
    //if(!m_IsRendered)
    //    return;

    //printf("Ludacris %g %f\n",m_InHandle->m_LastDelay - luda, m_InHandle->m_LastDelay-luda);
    factor = m_InHandle->m_LastDelay - luda;
    if (factor<0.0) {
        factor = -factor;
    }
    if (factor > m_InHandle->m_LastDelay)
        factor = 0.01f;
    //printf("Ludacris %g %f\n",factor, factor);
    //factor = 0.008;
    if(m_InputReady) {
        ret = m_FrameQueue->get(&vf);
        if(ret == 1) {
            m_DisplayTimer->Start(1000, wxTIMER_ONE_SHOT);
	  return;
        }
        //printf("Frame with PTS %f\n", vf.m_PTS);
        counter++;
        delay = vf.m_PTS - m_InHandle->m_LastPTS; // the pts from last time
        if(delay <= 0 || delay >= 1.0) {
            // if incorrect delay, use previous one
            delay = m_InHandle->m_LastDelay;
        }
      /* save for next time */
      m_InHandle->m_LastDelay = delay;
      m_InHandle->m_LastPTS = vf.m_PTS;
      //printf("Delay %g\n", delay);
      /* Skip or repeat the frame. Take delay into account
	 FFPlay still doesn't "know if this is the best guess." 
      sync_threshold = (delay > AV_SYNC_THRESHOLD) ? delay : AV_SYNC_THRESHOLD;
      if(fabs(diff) < AV_NOSYNC_THRESHOLD) {
	if(diff <= -sync_threshold) {
	  delay = 0;
	} else if(diff >= sync_threshold) {
	  delay = 2 * delay;
	}
      }*/
      m_InHandle->m_FrameTimer += delay;
      /* computer the REAL delay */
      actual_delay = m_InHandle->m_FrameTimer - (av_gettime() / 1000000.0);
      if(actual_delay < 0.010) {
	/* Really it should skip the picture instead */
	actual_delay = 0.010;
      }
      m_InHandle->m_LastDelay = actual_delay - factor;
      //printf("Stuff happening");
      //printf("Actual Delay %g\n", actual_delay);
      int32_t clock = (int32_t)(abs((actual_delay - factor)) * 1000 + 0.5);
      bool fired = m_DisplayTimer->Start(clock, wxTIMER_ONE_SHOT);
      start = av_gettime() / 1000000.0;
      /* show the picture! */
      m_Screen->SwapImage(vf.m_Frame, m_InHandle);
      m_Screen->Refresh();
      avcodec_free_frame(&vf.m_Frame);
      delete vf.m_Buffer;
      //printf("Stuff happening %d %g %g", clock, delay, luda);
  } else {
      m_DisplayTimer->Start(1000, wxTIMER_ONE_SHOT);
  }
    
}

void MainFrame::OnInputReady( wxCommandEvent& evt ) {
    if(evt.GetInt() != 0) {
        m_InputReady = true;
        //av_usleep(100000);
        log("File Opened");
        m_DisplayTimer = new wxTimer;
        m_DisplayTimer->SetOwner(this);
        m_SwarmReportTimer = new wxTimer;
        m_SwarmReportTimer->SetOwner(this);
        this->Connect(m_DisplayTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(MainFrame::OnDisplayRefresh), NULL, this);
        this->Connect( m_SwarmReportTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(MainFrame::OnSwarmReport), NULL, this);
        m_SwarmReportTimer->Start(200, wxTIMER_CONTINUOUS);
        m_DisplayTimer->Start(1, wxTIMER_ONE_SHOT);
        //Job j(Job::THREAD_NEXT_CHUNK_ID, NULL);
        //m_DecoderQueue->AddNewJob(j);
    } else {
        m_InputReady = false;
        log("Problem Opening File");
    }
}

void MainFrame::OnIncMessage( wxCommandEvent& evt ) {
    PRIME::PRIMEMessage* msg = (PRIME::PRIMEMessage*)evt.GetClientData();
    //log(wxString::Format("Type %d", msg->type()));
    switch(msg->type()) {
    case PRIME::PRIMEMessage_MsgType_PRIME_REPLY:
    {
        PRIME::Reply rep = msg->reply();
        DecodeReply(evt, rep);
        break;
    }
    case PRIME::PRIMEMessage_MsgType_PRIME_REPORT:
    {
        PRIME::Report rep = msg->report();
        DecodeReport(evt, rep);
  
        break;
    }
    case PRIME::PRIMEMessage_MsgType_PRIME_REQUEST:
    {
        PRIME::Request req = msg->request();
        DecodeRequest(evt, req);
        break;
    }
    default:
        log(wxT("Comple 4!!!"));
        break;
    }
}

void MainFrame::DecodeRequest(wxCommandEvent& evt, PRIME::Request& req) {

}

void MainFrame::DecodeReply(wxCommandEvent& evt, PRIME::Reply& repl) {
    printf("Message Type == %d\n", repl.type());
    switch( repl.type() ) {
    case PRIME::Reply_ReplyType_CS_SESSION_INIT:
    {
        PRIME::Reply_CSSIReply csrep = repl.cs_session_state();
        HandleCSSIReply(evt, csrep);

        break;
    }
    case PRIME::Reply_ReplyType_P2P_SESSION_INIT:
    {
        PRIME::Reply_P2PSIReply p2prep = repl.p2p_session_state();
        HandleP2PSIReply(evt, p2prep);

        break;    
    }
    default:
        break;
    }
}

static double last = 0, now = 0;
void MainFrame::DecodeReport(wxCommandEvent& evt, PRIME::Report& rep) {
    switch( rep.type() ) {
    case PRIME::Report_ReportType_DIFF_REPORT_TYPE:
    {
        now = av_gettime() / 1000000.0;
        PRIME::Report_DiffusionReport diffrep = rep.diff_report();
        //log(wxString::Format(" Start Seq %u End Seq %u", (uint32_t)m_CBuffer->diff_start_seq, (uint32_t)m_CBuffer->diff_end_seq));
        //log(wxString::Format(" Start Seq %u End Seq %u\n", (uint32_t)diffrep.start_seq(), (uint32_t)diffrep.end_seq()));
        //printf("\n Start Seq %u End Seq %u", (uint32_t)m_CBuffer->diff_start_seq, (uint32_t)m_CBuffer->diff_end_seq);
        //printf(" Start Seq %u End Seq %u\n", (uint32_t)diffrep.start_seq(), (uint32_t)diffrep.end_seq());
        m_CBuffer->UpdateDiffState(diffrep.start_seq(), diffrep.end_seq());
        if(m_CBuffer->swapQueues() == 0) {
	  //log(wxString::Format(" Start Seq %u End Seq %u", (uint32_t)diffrep.start_seq(), (uint32_t)diffrep.end_seq()));
	  //printf("Diffusion Report\n");
	  //printf("Queue Size %d\n", m_CBuffer->m_DiffPackets->size());
            if(!m_InputReady) {
                Job j(Job::THREAD_OPEN_STREAM_ID, NULL);
                m_DecoderQueue->AddNewJob(j);
            } 
	  Job j(Job::THREAD_NEXT_CHUNK_ID, NULL);
	  m_DecoderQueue->AddNewJob(j);
          
        } else {
	  //log(wxString::Format(" Start Seq %u End Seq %u", (uint32_t)diffrep.start_seq(), (uint32_t)diffrep.end_seq()));
        }
        
        //log(wxString::Format("Diff %g", now-last));
        last = now;
        
        break;
    }
    case PRIME::Report_ReportType_SWARM_REPORT_TYPE:
    {
        //printf("Swarming Report\n");
        break;    
    }
    case PRIME::Report_ReportType_ADD_DIFF_PARENT_TYPE:
    {
        PRIME::Report_DiffusionReport diffrep = rep.diff_report();
        break;    
    }
    case PRIME::Report_ReportType_ADD_SWARM_PARENT_TYPE:
    {
        printf("Swarm Parent\n");
        PRIME::Report_AddSwarmParentReport swarmpa_rep = rep.add_swarm_parent_report();
        HandleAddSwarmParentReport(evt, swarmpa_rep);
        break;    
    }
    case PRIME::Report_ReportType_ADD_SWARM_CHILD_TYPE:
    {
        printf("Swarm Child\n");
        PRIME::Report_AddSwarmChildReport swarmch_rep = rep.add_swarm_child_report();
        HandleAddSwarmChildReport(evt, swarmch_rep);

        break;    
    }
    default:
        break;
    }
}

void MainFrame::HandleAddDiffParentReport(wxCommandEvent& evt, PRIME::Report_AddDiffParentReport& diffpa) {

}
//void							HandleAddDiffChildReport(wxCommandEvent& evt, PRIME::Report_AddSwarmChildReport& p2prep);
void MainFrame::HandleAddSwarmParentReport(wxCommandEvent& evt, PRIME::Report_AddSwarmParentReport& swarmpa) {
    PRIME::PRIMEMessage holePunchMsg;
    holePunchMsg.set_type(PRIME::PRIMEMessage_MsgType_PRIME_HOLE_PUNCH);
    holePunchMsg.set_dummy("HOLE PUNCH!!!!");
    PRIMEPacket* pkt = new PRIMEPacket(holePunchMsg);
    wxIPV4address SwarmParentAddress;
    SwarmParentAddress.Hostname(swarmpa.swarmparent().ipaddr());
    SwarmParentAddress.Service(swarmpa.swarmparent().repport());
    pkt->m_Destination = SwarmParentAddress;
    
    //log("ludacristos");
    Job j(Job::THREAD_HOLE_PUNCH_ID, (void*)pkt);
    m_ReportQueue->AddNewJob(j);

    SwarmParentAddress.Service(swarmpa.swarmparent().rtpport());

    PRIMEPacket* pkt2 = new PRIMEPacket(holePunchMsg);
    pkt2->m_Destination = SwarmParentAddress;

    Job j2(Job::THREAD_HOLE_PUNCH_ID, (void*)pkt2);
    m_ReceiverQueue->AddNewJob(j2);

    PeerState* ps = new PeerState;
    ps->m_Addr = SwarmParentAddress;
    ps->m_RepPort = swarmpa.swarmparent().repport();
    ps->m_StreamPort = swarmpa.swarmparent().rtpport();

    m_PRIMEState->addSwarmParent(ps);
}

void MainFrame::HandleAddSwarmChildReport(wxCommandEvent& evt, PRIME::Report_AddSwarmChildReport& swarmch) {
    PRIME::PRIMEMessage holePunchMsg;
    holePunchMsg.set_type(PRIME::PRIMEMessage_MsgType_PRIME_HOLE_PUNCH);
    holePunchMsg.set_dummy("HOLE PUNCH!!!!");
    PRIMEPacket* pkt = new PRIMEPacket(holePunchMsg);
    wxIPV4address SwarmChildAddress;
    SwarmChildAddress.Hostname(swarmch.swarmchild().ipaddr());
    SwarmChildAddress.Service(swarmch.swarmchild().repport());
    pkt->m_Destination = SwarmChildAddress;
    
    //log("ludacristos");
    Job j(Job::THREAD_HOLE_PUNCH_ID, (void*)pkt);
    m_ReportQueue->AddNewJob(j);

    PeerState* ps = new PeerState;
    ps->m_Addr = SwarmChildAddress;
    ps->m_RepPort = swarmch.swarmchild().repport();
    ps->m_StreamPort = swarmch.swarmchild().rtpport();

    m_PRIMEState->addSwarmChild(ps);
}
	
void MainFrame::HandleCSSIReply(wxCommandEvent& evt, PRIME::Reply_CSSIReply& csrep) {
    //log(csrep.sdp());
    log(wxString::Format("Diff Parent %s %d %d\n", csrep.diffparent().ipaddr() , csrep.diffparent().repport(), csrep.diffparent().rtpport()));
    log(wxString::Format("My RTP Stream on %d\n", csrep.streamport()));
    PRIME::PRIMEMessage holePunchMsg;
    holePunchMsg.set_type(PRIME::PRIMEMessage_MsgType_PRIME_HOLE_PUNCH);
    holePunchMsg.set_dummy("HOLE PUNCH!!!!");
    PRIMEPacket* pkt = new PRIMEPacket(holePunchMsg);
    wxIPV4address DiffParentAddress;
    DiffParentAddress.Hostname(csrep.diffparent().ipaddr());
    DiffParentAddress.Service(csrep.diffparent().repport());
    pkt->m_Destination = DiffParentAddress;
    
    //log("ludacristos");
    Job j(Job::THREAD_HOLE_PUNCH_ID, (void*)pkt);
    DiffParentAddress.Service(csrep.diffparent().rtpport());
    m_ReportQueue->AddNewJob(j);

    PRIMEPacket* pkt2 = new PRIMEPacket(holePunchMsg);
    pkt2->m_Destination = DiffParentAddress;

    JobQueue* decQueue = new JobQueue;
    m_DecoderQueue = decQueue;
    m_InHandle = new InputHandler(csrep.sdp().c_str(), csrep.width(), csrep.height(), (AVPixelFormat)csrep.pixfmt());
    m_FrameQueue = new FrameQueue;
    m_FrameQueue->init(csrep.maxframecount(), (AVPixelFormat)csrep.pixfmt(), csrep.width(), csrep.height());

    if (m_CBuffer == NULL) {
        m_CBuffer = new PacketQueue;
    }

    m_InHandle->in_buffer = m_CBuffer;

    DecodingThread* dect = new DecodingThread(this, m_InHandle, decQueue, m_CBuffer,
                               m_FrameQueue);
    if (dect->Create() == wxTHREAD_NO_ERROR) {
        dect->Run();
    }
    else
    {
        log("Server : Unable to start DecodingThread");
    }

    wxIPV4address locAddr;
       
    locAddr.Hostname();
    locAddr.Service(csrep.streamport());
 
    m_RTPRecvSocket = new wxDatagramSocket(locAddr);
    if(m_RTPRecvSocket->Ok())
    {
        m_RTPRecvSocket->GetLocal(locAddr);
        //m_InReportSocket->SetNotify(false);
        log(wxString::Format("RTP Socket %d %s\n", locAddr.Service(), locAddr.IPAddress()));
    }
    int sf;
    int len = sizeof(sf);
    m_RTPRecvSocket->GetOption(SOL_SOCKET, SO_RCVBUF, &sf, &len);
    printf("Socket %d\n", sf);
    sf = 1 << 20;
    m_RTPRecvSocket->SetOption(SOL_SOCKET, SO_RCVBUF, &sf, len);
    m_RTPRecvSocket->GetOption(SOL_SOCKET, SO_RCVBUF, &sf, &len);
    printf("Socket %d\n", sf);
    
    m_ReceiverQueue = new JobQueue;
    //log("ludacristos");
    Job j2(Job::THREAD_HOLE_PUNCH_ID, (void*)pkt2);
    m_ReceiverQueue->AddNewJob(j2);
    Job init_j(Job::THREAD_RECV_CHUNK_ID, NULL);
    m_ReceiverQueue->AddNewJob(init_j);
    ReceiverThread* rect = new ReceiverThread(this, m_ReceiverQueue, m_CBuffer , m_RTPRecvSocket);
    if (rect->Create() == wxTHREAD_NO_ERROR) {
        rect->Run();
    }
    else
    {
        log("Server : Unable to start ReceiverThread");
    }
}

void MainFrame::OnSwarmReport( wxTimerEvent& evt ) {
    for(int i = 0; i < m_PRIMEState->m_SwarmChildren.size(); i++) {
        PRIMEPacket* pkt = m_CBuffer->getSwarmReport();
        wxIPV4address addr; 
        addr.Hostname(m_PRIMEState->m_SwarmChildren[i]->m_Addr.IPAddress());
        //addr.Hostname("192.168.1.87");
        addr.Service(m_PRIMEState->m_SwarmChildren[i]->m_RepPort);
        pkt->m_Destination = addr;
        
        Job j(Job::THREAD_WRITE_DATA_ID, (void*)pkt);
        m_ReportQueue->AddNewJob(j); 
        //printf("OK\n");
    }
}

void MainFrame::HandleP2PSIReply(wxCommandEvent& evt, PRIME::Reply_P2PSIReply& p2prep) {
    log(wxString::Format("Diff Parent %s %d %d\n", p2prep.diffparent().ipaddr() , p2prep.diffparent().repport(), p2prep.diffparent().rtpport()));
    log(wxString::Format("My RTP Stream on %d\n", p2prep.streamport()));
    PRIME::PRIMEMessage holePunchMsg;
    holePunchMsg.set_type(PRIME::PRIMEMessage_MsgType_PRIME_HOLE_PUNCH);
    holePunchMsg.set_dummy("HOLE PUNCH!!!!");
    PRIMEPacket* pkt = new PRIMEPacket(holePunchMsg);
    wxIPV4address DiffParentAddress;
    DiffParentAddress.Hostname(p2prep.diffparent().ipaddr());
    DiffParentAddress.Service(p2prep.diffparent().repport());
    pkt->m_Destination = DiffParentAddress;
    
    //log("ludacristos");
    Job j(Job::THREAD_HOLE_PUNCH_ID, (void*)pkt);
    DiffParentAddress.Service(p2prep.diffparent().rtpport());
    m_ReportQueue->AddNewJob(j);

    PRIMEPacket* pkt2 = new PRIMEPacket(holePunchMsg);
    pkt2->m_Destination = DiffParentAddress;

    JobQueue* decQueue = new JobQueue;
    m_DecoderQueue = decQueue;
    m_InHandle = new InputHandler(p2prep.sdp().c_str(), p2prep.width(), p2prep.height(), (AVPixelFormat)p2prep.pixfmt());
    m_FrameQueue = new FrameQueue;
    m_FrameQueue->init(p2prep.maxframecount(), (AVPixelFormat)p2prep.pixfmt(), p2prep.width(), p2prep.height());

    if (m_CBuffer == NULL) {
        m_CBuffer = new PacketQueue;
    }

    m_InHandle->in_buffer = m_CBuffer;

    DecodingThread* dect = new DecodingThread(this, m_InHandle, decQueue, m_CBuffer,
                               m_FrameQueue);
    if (dect->Create() == wxTHREAD_NO_ERROR) {
        dect->Run();
    }
    else
    {
        log("Server : Unable to start DecodingThread");
    }

    wxIPV4address locAddr;
       
    locAddr.Hostname();
    locAddr.Service(p2prep.streamport());
 
    m_RTPRecvSocket = new wxDatagramSocket(locAddr);
    if(m_RTPRecvSocket->Ok())
    {
        m_RTPRecvSocket->GetLocal(locAddr);
        //m_InReportSocket->SetNotify(false);
        log(wxString::Format("RTP Socket %d %s\n", locAddr.Service(), locAddr.IPAddress()));
    }
    int sf;
    int len = sizeof(sf);
    m_RTPRecvSocket->GetOption(SOL_SOCKET, SO_RCVBUF, &sf, &len);
    printf("Socket %d\n", sf);
    sf = 1 << 20;
    m_RTPRecvSocket->SetOption(SOL_SOCKET, SO_RCVBUF, &sf, len);
    m_RTPRecvSocket->GetOption(SOL_SOCKET, SO_RCVBUF, &sf, &len);
    printf("Socket %d\n", sf);
    
    m_ReceiverQueue = new JobQueue;
    //log("ludacristos");
    Job j2(Job::THREAD_HOLE_PUNCH_ID, (void*)pkt2);
    m_ReceiverQueue->AddNewJob(j2);
    Job init_j(Job::THREAD_RECV_CHUNK_ID, NULL);
    m_ReceiverQueue->AddNewJob(init_j);
    ReceiverThread* rect = new ReceiverThread(this, m_ReceiverQueue, m_CBuffer , m_RTPRecvSocket);
    if (rect->Create() == wxTHREAD_NO_ERROR) {
        rect->Run();
    }
    else
    {
        log("Server : Unable to start ReceiverThread");
    }
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	//if(m_WorkerPool)
	//	delete m_WorkerPool;
    
	// GUI Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileExit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnShowLog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnVideoLog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnSoundLog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnStreamLog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnPrimeLog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFlowLog ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnAboutHelp ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnAboutInfo ) );
	
	//Socket Events
	this->Disconnect( LISTENING_SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnListenEvent ), NULL, this );
	this->Disconnect( SERVER_SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnServerEvent ), NULL, this );
	this->Disconnect( PEER_SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnPeerEvent ), NULL, this );

	//Custom Events
	this->Disconnect( wxEVT_LOG_ENTRY, wxCommandEventHandler( MainFrame::OnLogEntry ), NULL, this );
	this->Disconnect( wxEVT_SERVER_REPORT, ServerReportEventHandler( MainFrame::OnServerReport ), NULL, this);
	this->Disconnect( wxEVT_SERVER_PROPERTIES, ServerPropertiesEventHandler( MainFrame::OnServerProperties ), NULL, this);
}

// GUI Event Callbacks
void MainFrame::OnFileOpen( wxCommandEvent& event ) { 
	ConnectionPropertiesFrame *fp = new ConnectionPropertiesFrame(this);
	//Disable();
          fp->Show();
          fp->MakeModal(true);
}

void MainFrame::OnFileClose( wxCommandEvent& evt ) { evt.Skip(); }
void MainFrame::OnFileExit( wxCommandEvent& evt ) { evt.Skip(); }
void MainFrame::OnShowLog( wxCommandEvent& evt ) { evt.Skip(); }
void MainFrame::OnVideoLog( wxCommandEvent& evt ) { evt.Skip(); }
void MainFrame::OnSoundLog( wxCommandEvent& evt ) { evt.Skip(); }
void MainFrame::OnStreamLog( wxCommandEvent& evt ) { evt.Skip(); }
void MainFrame::OnPrimeLog( wxCommandEvent& evt ) { evt.Skip(); }
void MainFrame::OnFlowLog( wxCommandEvent& evt ) { evt.Skip(); }
void MainFrame::OnAboutHelp( wxCommandEvent& evt ) { evt.Skip(); }
void MainFrame::OnAboutInfo( wxCommandEvent& evt ) { evt.Skip(); }

//Socket Event Callbacks
void MainFrame::OnPeerEvent( wxSocketEvent& evt ) 
{ 
    
}

void MainFrame::OnListenEvent(wxSocketEvent& event)
{
    
}

void MainFrame::DecodeMessage(PRIME::PRIMEMessage* msg) {	
	
}

void MainFrame::setBuffer(ChunkBuffer* buff) {
	//m_CBuffer = buff;
}

void MainFrame::OnServerEvent(wxSocketEvent& evt)
{
}

//Custom Event Callbacks
void MainFrame::OnLogEntry( wxCommandEvent& event ) { 
	m_Log->AppendText("Client : " + event.GetString() + "\n");
}

void MainFrame::OnServerReport (ServerReportEvent& event ) {

}

void MainFrame::OnServerProperties (ServerPropertiesEvent& evt ) {
    ServerSession *sess = new ServerSession;
    *sess = evt.m_Sess;

    if ( sess->m_SreamIpStr.IsEmpty() || sess->m_StreamPort == 0) {
        return;
    } else {      
        m_ConnMonitorQueue = new JobQueue;
        m_CSock = new wxSocketClient;
        PRIME::PRIMEMessage* msg = new PRIME::PRIMEMessage;
        PRIME::Request req;
        
        m_StreamAddress = sess->m_SreamIpStr;
        m_StreamPort = sess->m_StreamPort;
        if ( sess->m_ubw == 0.0f ) {
            PRIME::Request_CSSIRequest stats;
            req.set_type(PRIME::Request_RequestType_CS_SESSION_INIT);
            stats.set_dbw(sess->m_dbw);
            stats.set_peername(wxGetFullHostName());
            stats.set_ipaddr(m_IpAddr);
            wxIPV4address addr;
            m_InReportSocket->GetLocal(addr);
            stats.set_repport(addr.Service());
            
            req.mutable_cs_session_props()->CopyFrom(stats);
            msg->set_type(PRIME::PRIMEMessage_MsgType_PRIME_REQUEST);
            msg->mutable_request()->CopyFrom(req);
            
            Job init_j(Job::THREAD_WRITE_DATA_ID, msg);
            m_ConnMonitorQueue->AddNewJob(init_j);

	  Job j(Job::THREAD_READ_DATA_ID, NULL);
            m_ConnMonitorQueue->AddNewJob(j);
        } else {
            PRIME::Request_P2PSIRequest stats;
            req.set_type(PRIME::Request_RequestType_P2P_SESSION_INIT);
            stats.set_dbw(sess->m_dbw);
            stats.set_ubw(sess->m_ubw);
            stats.set_peername(wxGetFullHostName());
            stats.set_ipaddr(m_IpAddr);
	  stats.set_rtprecvport(0);
	  stats.set_repsendport(0);
	  wxIPV4address addr;
            m_InReportSocket->GetLocal(addr);
            stats.set_repport(addr.Service());
            req.mutable_p2p_session_props()->CopyFrom(stats);
            msg->set_type(PRIME::PRIMEMessage_MsgType_PRIME_REQUEST);
            msg->mutable_request()->CopyFrom(req);

            Job init_j(Job::THREAD_WRITE_DATA_ID, msg);
            m_ConnMonitorQueue->AddNewJob(init_j);

	  Job j(Job::THREAD_READ_DATA_ID, NULL);
            m_ConnMonitorQueue->AddNewJob(j);
        }
        ServerConnectionMonitor* conn = new ServerConnectionMonitor(m_CSock, this, m_ConnMonitorQueue);
        if (conn->Create() == wxTHREAD_NO_ERROR) {
            conn->Run();
        }
        else
        {
            //delete m_ConnMonitorQueue;
            //m_CSock->Destroy();
            //delete msg;
            m_StreamAddress = wxEmptyString;
            m_StreamPort = -1;
            log("Server : Unable to start Connection Thread");
        }
    }
}

void MainFrame::log(const wxString& text)
{
	wxCommandEvent* event = new wxCommandEvent(wxEVT_LOG_ENTRY, wxID_ANY);
	event->SetString(wxString(text));

	wxQueueEvent( this, event );
}

//void MainFrame::setOutputHandle(OutputHandler* hnd) {
	//m_OutHandle = hnd;
//}

void MainFrame::OnNewOutput (wxCommandEvent& evt) {
	/*m_CBuffer->lock();
	int size = m_CBuffer->GetSize();
	for(int i = 0; i < size; i++) {
		ProcessedContent* cont = new ProcessedContent;
		cont->m_Guard = &m_OutGuard;
		cont->m_Content = m_CBuffer->PopChunk();
		cont->m_OutHnd = m_OutHandle;
   
		if ( cont->m_Content ) {
			wxString s;
			s.Printf("Writing %d", cont->m_Content->id);
			log(s);
		}

		Job j(Job::THREAD_WRITE_CHUNK, (void*)cont );
		m_WorkerPool->AssignJob(j);
	}
	m_CBuffer->unlock();

	Job j(Job::THREAD_NEXT_CHUNK_ID, (void*)NULL); 
	m_ReceiverQueue->AddNewJob(j);*/
}

void MainFrame::setRTPstate(int port) {
	/*jrtplib::RTPUDPv4TransmissionParams transparams;
	jrtplib::RTPSessionParams sessparams;

	sessparams.SetOwnTimestampUnit(1.0/30.0);		
	
	sessparams.SetAcceptOwnPackets(true);
	sessparams.SetMaximumPacketSize(65000);
	transparams.SetPortbase(port);
	transparams.SetRTPReceiveBuffer(1024 * 4096);

	m_ReceiverQueue = new JobQueue;
	Job init_j(Job::THREAD_OPEN_OUTPUT_ID, (void*)NULL); 
	m_ReceiverQueue->AddNewJob(init_j);
	Job j(Job::THREAD_NEXT_CHUNK_ID, (void*)NULL); 
	m_ReceiverQueue->AddNewJob(j);

	ReceiverThread* recvth = new ReceiverThread(this, m_ReceiverQueue, transparams, sessparams);

	if (recvth->Create() == wxTHREAD_NO_ERROR) {
        recvth->Run();
    } else {
		log("Unable to start ReceiverThread");
    }*/
}

ConnectionPropertiesFrame::ConnectionPropertiesFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	//this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_MainSizer;
	m_MainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_PropertySection;
	m_PropertySection = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* m_StaticTextSection;
	m_StaticTextSection = new wxBoxSizer( wxVERTICAL );
	
	m_IpText = new wxStaticText( this, wxID_ANY, wxT("Server IP"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_IpText->Wrap( -1 );
	m_StaticTextSection->Add( m_IpText, 0, wxALL|wxEXPAND, 9 );
	
	m_PortText = new wxStaticText( this, wxID_ANY, wxT("Server Port"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_PortText->Wrap( -1 );
	m_StaticTextSection->Add( m_PortText, 0, wxALL|wxEXPAND, 9 );
	
	m_DBWText = new wxStaticText( this, wxID_ANY, wxT("Download (Kb/s)"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_DBWText->Wrap( -1 );
	m_StaticTextSection->Add( m_DBWText, 0, wxALL|wxEXPAND, 9 );
	
	m_UBWText = new wxStaticText( this, wxID_ANY, wxT("Upload (Kb/s)"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_UBWText->Wrap( -1 );
	m_StaticTextSection->Add( m_UBWText, 0, wxALL|wxEXPAND, 9 );
	
	m_PropertySection->Add( m_StaticTextSection, 1, wxEXPAND, 30 );
	
	
	m_PropertySection->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_FieldSection;
	m_FieldSection = new wxBoxSizer( wxVERTICAL );
	
	m_IpProperty = new wxTextCtrl( this, wxID_ANY, wxT("192.168.1.87"), wxDefaultPosition, wxDefaultSize, wxTE_NO_VSCROLL|wxTE_RIGHT );
	m_FieldSection->Add( m_IpProperty, 0, wxALL|wxEXPAND, 5 );
	
	m_PortProperty = new wxTextCtrl( this, wxID_ANY, wxT("3000"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_FieldSection->Add( m_PortProperty, 0, wxALL|wxEXPAND, 5 );
	
	m_DBWProperty = new wxSpinCtrl( this, wxID_ANY, wxT("300"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, 300 );
	m_FieldSection->Add( m_DBWProperty, 0, wxALL|wxEXPAND, 5 );
	
	m_UBWProperty = new wxSpinCtrl( this, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, 100 );
	m_UBWProperty->Enable( false );
	
	m_FieldSection->Add( m_UBWProperty, 0, wxALL|wxEXPAND, 5 );
	
	m_PropertySection->Add( m_FieldSection, 1, wxEXPAND, 5 );
	
	m_MainSizer->Add( m_PropertySection, 1, wxEXPAND, 5 );
	
	m_LayoutLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_MainSizer->Add( m_LayoutLine, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_ButtonSection;
	m_ButtonSection = new wxBoxSizer( wxHORIZONTAL );
	
	m_ServerCheck = new wxCheckBox( this, wxID_ANY, wxT("Client-Server"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ServerCheck->SetValue(true); 
	m_ButtonSection->Add( m_ServerCheck, 0, wxALL, 5 );
	
	m_P2PCheck = new wxCheckBox( this, wxID_ANY, wxT("Peer-To-Peer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ButtonSection->Add( m_P2PCheck, 0, wxALL, 5 );
	
	
	m_ButtonSection->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_OkBtn = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ButtonSection->Add( m_OkBtn, 0, wxRIGHT, 5 );
	
	m_CancelBtn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ButtonSection->Add( m_CancelBtn, 0, wxLEFT, 5 );
	
	m_MainSizer->Add( m_ButtonSection, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( m_MainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_ServerCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConnectionPropertiesFrame::OnCSModeChecked ), NULL, this );
	m_P2PCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConnectionPropertiesFrame::OnP2PModeChecked ), NULL, this );
	m_OkBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectionPropertiesFrame::OnOkPressed ), NULL, this );
	m_CancelBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectionPropertiesFrame::OnCancelPressed ), NULL, this );
}

ConnectionPropertiesFrame::~ConnectionPropertiesFrame()
{
	m_ServerCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConnectionPropertiesFrame::OnCSModeChecked ), NULL, this );
	m_P2PCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ConnectionPropertiesFrame::OnP2PModeChecked ), NULL, this );
	m_OkBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectionPropertiesFrame::OnOkPressed ), NULL, this );
	m_CancelBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectionPropertiesFrame::OnCancelPressed ), NULL, this );
}

void ConnectionPropertiesFrame::OnCancelPressed( wxCommandEvent& event ) {
	ServerPropertiesEvent* evt = new ServerPropertiesEvent;
          
          this->MakeModal(false);
	this->GetParent()->GetEventHandler()->QueueEvent( evt );

	Destroy();
}

void ConnectionPropertiesFrame::OnOkPressed( wxCommandEvent& event ) {
	if ( m_IpProperty->GetValue().IsEmpty() 
	  || m_PortProperty->GetValue().IsEmpty() ) {
		wxMessageBox("Please fill all fields", "Attention!",
                            wxOK , this);
		return;
	}
	
	ServerPropertiesEvent* evt = new ServerPropertiesEvent;

	if ( !m_P2PCheck->IsChecked() ) {
		evt->m_Sess.m_SreamIpStr = m_IpProperty->GetValue();
		evt->m_Sess.m_StreamPort = wxAtoi(m_PortProperty->GetValue());
		evt->m_Sess.m_ubw = 0.0f;
		evt->m_Sess.m_dbw = m_DBWProperty->GetValue();
	} else {
		evt->m_Sess.m_SreamIpStr = m_IpProperty->GetValue();
		evt->m_Sess.m_StreamPort = wxAtoi(m_PortProperty->GetValue());
		evt->m_Sess.m_ubw = m_UBWProperty->GetValue();
		evt->m_Sess.m_dbw = m_DBWProperty->GetValue();
	}

          this->MakeModal(false);
	this->GetParent()->GetEventHandler()->QueueEvent( evt );

	Destroy();
}

void ConnectionPropertiesFrame::OnCSModeChecked( wxCommandEvent& event ) {
	m_P2PCheck->SetValue(false);
	m_ServerCheck->SetValue(true);

	m_UBWProperty->Enable( false );
}

void ConnectionPropertiesFrame::OnP2PModeChecked( wxCommandEvent& event ) {
	m_ServerCheck->SetValue(false);
	m_P2PCheck->SetValue(true);

	m_UBWProperty->Enable();
}

/*
	int to_process;
	int header;

	to_process = 4;
    do
    {
		sock->Read(&header,to_process);
        if (sock->Error())
        {
			return;
        }
        to_process -= sock->LastCount();
    } while (!sock->Error() && to_process != 0);

	BYTE* buffer = new BYTE[header];
	do
    {
		sock->Read(buffer,to_process);
        if (sock->Error())
        {
			return;
        }
		to_process -= sock->LastCount();
    } while (!sock->Error() && to_process != 0);
	*/

/*
enum
{
	SERVER_ID,
    SOCKET_ID,
	UDPSOCKET_ID
};


MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetSize(596,470);

	wxBoxSizer* m_MainSizer;
	m_MainSizer = new wxBoxSizer( wxVERTICAL );

	m_ChatLabel = new wxStaticText( this, wxID_ANY, wxT("Chat :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ChatLabel->Wrap( -1 );
	m_MainSizer->Add( m_ChatLabel, 0, wxLEFT|wxTOP, 5 );

	m_OutputText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,200 ), wxTE_CHARWRAP|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_MainSizer->Add( m_OutputText, 0, wxALL|wxEXPAND, 5 );

	m_MessageLabel = new wxStaticText( this, wxID_ANY, wxT("Your Message :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_MessageLabel->Wrap( -1 );
	m_MainSizer->Add( m_MessageLabel, 0, wxLEFT, 5 );

	m_InputText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_CHARWRAP|wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_WORDWRAP );
	m_MainSizer->Add( m_InputText, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* m_ButtonSizer;
	m_ButtonSizer = new wxBoxSizer( wxHORIZONTAL );

	m_ConnButton = new wxButton( this, wxID_ANY, wxT("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ButtonSizer->Add( m_ConnButton, 0, wxALL, 5 );

	m_SendButton = new wxButton( this, wxID_ANY, wxT("SEND"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SendButton->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );

	m_ButtonSizer->Add( m_SendButton, 0, wxALL|wxEXPAND, 5 );

	m_MainSizer->Add( m_ButtonSizer, 0, wxALIGN_RIGHT, 5 );

	this->SetSizer( m_MainSizer );
	this->Layout();
	m_StatusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );

	
	wxIPV4address addr;
	addr.Service(::wxGetProcessId()+10000);

	printf("Server Event %d\n",::wxGetProcessId()+10000);
	// Create the socket
	m_SSock = new wxSocketServer(addr);

	// We use Ok() here to see if the server is really listening
	if (!m_SSock->Ok())
	{
		m_OutputText->AppendText(_("Could not listen at the specified port!\n\n"));
	}
	else
	{
		m_OutputText->AppendText(_("Server listening.\n\n"));
	}

	// Setup the event handler and subscribe to connection events
	m_SSock->SetEventHandler(*this, SERVER_ID);
	m_SSock->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG);
	m_SSock->Notify(true);
	
	wxIPV4address m_localaddr;
	m_localaddr.AnyAddress();
	m_localaddr.Service(3000);

	m_dsocket = new wxDatagramSocket (m_localaddr, wxSOCKET_NOWAIT | wxSOCKET_REUSEADDR);
	
	if(m_dsocket->Ok())
	{
		m_OutputText->AppendText(_("Udp Server Ready.\n\n"));
		int broadcast = 1;
		if(! m_dsocket->SetOption( SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof( broadcast ) ) )
			printf("Datagram issue\n");
		
		if (m_dsocket->Error ())
		{
			printf("Datagram Proble\n");
		}

		m_dsocket->SetEventHandler(*this, UDPSOCKET_ID);
		m_dsocket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG);
		m_dsocket->Notify(true);
	}
	else
	{
		m_OutputText->AppendText(_("Udp Socket Not working!\n\n"));		
	}

	wxIPV4address addra;
	addra.Hostname(wxGetFullHostName());
	wxString ipAddr = addra.IPAddress();
	
	connected_peers = 0;

	m_OutputText->AppendText(ipAddr);		
	// Connect Events
	m_InputText->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::OnInput ), NULL, this );
	m_ConnButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConnect ), NULL, this );
	m_SendButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSend ), NULL, this );

	//Socket Events
	this->Connect(SERVER_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnServerEvent ), NULL, this );
	this->Connect(SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnSocketEvent ), NULL, this );
	this->Connect(UDPSOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnBroadcastEvent ), NULL, this );
}

// --------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// --------------------------------------------------------------------------

MainFrame::~MainFrame()
{
	m_dsocket->Destroy();
	m_SSock->Destroy();

	m_InputText->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::OnInput ), NULL, this );
	m_ConnButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConnect ), NULL, this );
	m_SendButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnSend ), NULL, this );
}

void MainFrame::OnSend(wxCommandEvent& WXUNUSED(event))
{
	char b[] = {'H','b','y','!','!','!','\0'};
	long pid = ::wxGetProcessId();
	wxIPV4address m_broadcast;
	

	//m_localaddr.AnyAddress();
	m_broadcast.Hostname("255.255.255.255");
	m_broadcast.Service(3000);
		
	m_dsocket->SendTo(m_broadcast,&pid,sizeof pid);
	
	if ( m_dsocket->Error ())
	{
		printf("Datagram Proble\n");
	}
}

void MainFrame::OnConnect(wxCommandEvent& WXUNUSED(event))
{
	wxString hostname = wxGetTextFromUser(
		_("Enter IP Address:"),
		_("Start ..."),
		_("127.0.0.1"));

	wxString port = wxGetTextFromUser(
		_("Enter the listening port:"),
		_("Start ..."),
		_("3000"));

	long portNo = 0;
	port.ToLong(&portNo);

    wxIPV4address addr;
    addr.Hostname(hostname);
    addr.Service(portNo);

    // Create the socket
    m_CSock[connected_peers] = new wxSocketClient();

    // Set up the event handler and subscribe to most events
    m_CSock[connected_peers]->SetEventHandler(*this, SOCKET_ID);
    m_CSock[connected_peers]->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
    m_CSock[connected_peers]->Notify(true);

    // Wait for the connection event
    m_CSock[connected_peers++]->Connect(addr, false);

	//connected_peers++;
    //OpenConnection(3);
}


void MainFrame::OnServerEvent(wxSocketEvent& event)
{
	wxEventType t = event.GetSocketEvent();
	printf("Server Event %d\n",t);
	// Accept the connection
	if (event.GetSocketEvent() == wxSOCKET_CONNECTION)
    {
		wxSocketBase* sock = m_SSock->Accept(false);

		sock->SetFlags(wxSOCKET_WAITALL | wxSOCKET_BLOCK);
	
		//wxMutexGuiEnter();

		m_OutputText->AppendText(_("\n\nGot connection!!!!!\n\n"));
	
		//wxMutexGuiLeave();
		char b[] = {'H','e','y','!','!','!','\0'};

		wxSocketBase &sb = sock->Write(b,strlen(b));
		if(sb.Error())
			printf("Problem\n");
			////new m_ListenThread(this, sock);
	}
}
	

void MainFrame::OnBroadcastEvent(wxSocketEvent& event)
{
	wxEventType t = event.GetSocketEvent();
	printf("Broadcast Event %d\n",t);
	if (event.GetSocketEvent() == wxSOCKET_INPUT)
    {
		long b = 0;
		m_dsocket->Read(&b,sizeof b);
		
		printf("Edw Eisai %d\n",b);
	}
	
}

void MainFrame::OnSocketEvent(wxSocketEvent& event)
{
	wxEventType t = event.GetSocketEvent();
	printf("Polu %d\n",t);
	//m_OutputText->AppendText();

    // All we need is the connection, that's the green light to proceed
    if (event.GetSocketEvent() == wxSOCKET_CONNECTION)
    {
		m_OutputText->AppendText(_("\n\nStarting the thread, check for output file in a moment..."));
			
		// Start the file transfer
        //new CFileReceiveThread(wxT("WriteTo.jpg"), event.GetSocket());
    }
	else if (event.GetSocketEvent() == wxSOCKET_LOST)
    {
		m_OutputText->AppendText(_("\n\nSession Lost..."));
    }
	else if (event.GetSocketEvent() == wxSOCKET_INPUT)
    {
		char buff[32];
		m_OutputText->AppendText(_("\n\nData Pending..."));
		event.GetSocket()->Read(buff,6);
		buff[6] = '\0';
		m_OutputText->AppendText(buff);
    }
}

void MainFrame::OnInput( wxCommandEvent& event ){
	wxChar *buf2 = new wxChar[wxStrlen(C3OInputText->GetValue().mb_str(wxConvUTF8)) + 1];
	wxPrintf(_("Is ASCII %d ...\n"),wxStrlen(C3OInputText->GetValue().mb_str(wxConvUTF8)));
	wxPrintf(_("Is ASCII %d ...\n"),wxStrlen(C3OInputText->GetValue()));
	wxPrintf(_("Is ASCII %d ...\n"),C3OInputText->GetValue().IsAscii());
	strcpy(buf2,(const char *)C3OInputText->GetValue().mb_str(wxConvUTF8));
	wxPrintf(_("Is ASCII %d ...\n"),C3OInputText->GetValue().IsAscii());
	C3OOutputText->AppendText(_("Sending a test buffer to the server ..."));
	C3OCSock->Write(buf2, wxStrlen(C3OInputText->GetValue()) + 1);
	C3OOutputText->AppendText(C3OCSock->Error() ? _("failed !\n") : _("done\n"));
	delete [] buf2;
}

bool MainFrame::OpenConnection(int family)
{
	wxIPV4address addr;
	/*
	wxString hostname = wxGetTextFromUser(
    _("Enter the address and port of the peer you wish to chat with:"),
    _("Connect ..."),
    _("localhost"));

	addr.Hostname(hostname);
	addr.Service(3000);

	C3OOutputText->AppendText(_("\nTrying to connect (timeout = 10 sec) ...\n"));
	C3OCSock->Connect(addr, false);
	C3OCSock->WaitOnConnect(10);

	if (C3OCSock->IsConnected())
		C3OOutputText->AppendText(_("Succeeded ! Connection established\n"));
	else
	{
		C3OCSock->Close();
		C3OOutputText->AppendText(_("Failed ! Unable to connect\n"));
		wxMessageBox(_("Can't connect to the specified host"), _("Alert !"));
	}
	
	return true;
}*/


