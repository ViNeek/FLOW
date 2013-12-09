///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "fwInterface.h"

#include "fwScreen.h"
#include "fwJobQueue.h"
#include "fwPacketBuffer.h"
#include "fwDecodingThread.h"
#include "fwInputHandler.h"
#include "fwOutputHandler.h"
#include "fwPacketQueue.h"
#include "fwFrameQueue.h"
#include "fwPeerState.h"
#include "fwPeerConnectionMonitor.h"
#include "fwPRIMEState.h"
#include "fwSenderThread.h"
#include "fwReportMonitor.h"
#include "fwTrackerPeerState.h"
#include "fwFlowTree.h"

///////////////////////////////////////////////////////////////////////////

DEFINE_LOCAL_EVENT_TYPE(wxEVT_LOG_ENTRY)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_INPUT_READY)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_BUFFER_FULL)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_INC_PRIME_MESSAGE)
DEFINE_LOCAL_EVENT_TYPE(wxEVT_PEER_DEPARTURE)

enum
{
	LISTENING_SOCKET_ID,
    PEER_SOCKET_ID,
};

enum
{
    P2P_SESSION,
    CS_SESSION
};

void MainFrame::OnEraseBackGround(wxEraseEvent& event) {};

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	//this->SetSize(wxSystemSettings::GetMetric ( wxSYS_SCREEN_X ), wxSystemSettings::GetMetric ( wxSYS_SCREEN_Y ));
	//this->Maximize();
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	//this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME ) );

	wxBoxSizer* m_MainSizer;
	m_MainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_ScreenSizer;
	m_ScreenSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_Screen = new FLOWScreen( this );
	m_ScreenSizer->Add( m_Screen, 1, wxEXPAND | wxALL, 5 );
	
	m_PeerList = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 300,-1 ), wxTR_DEFAULT_STYLE|wxTR_FULL_ROW_HIGHLIGHT );
	m_PeerList->SetMaxSize( wxSize( 300,-1 ) );
	//m_PeerList->SetForegroundColour( wxColour( 255, 255, 255 ) );
	//m_PeerList->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	m_PeerList->AddRoot(wxT("ConnectedPeers"));
	
	m_ScreenSizer->Add( m_PeerList, 0, wxALL|wxEXPAND, 5 );
	
	m_MainSizer->Add( m_ScreenSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_LogSizer;
	m_LogSizer = new wxBoxSizer( wxHORIZONTAL );
	m_LogSizer->SetMinSize( wxSize( -1,200 ) );

	m_Log = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_CHARWRAP|wxTE_MULTILINE|wxTE_NOHIDESEL|wxTE_READONLY|wxTE_WORDWRAP|wxVSCROLL );
	m_Log->SetMinSize( wxSize( -1,100 ) );
	m_Log->SetMaxSize( wxSize( -1,100 ) );
	//m_Log->SetForegroundColour( wxColour( 255, 255, 255 ) );
	//m_Log->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	m_LogSizer->Add( m_Log, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* m_StatsSizer;
	m_StatsSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_StatsSizer->SetMinSize( wxSize( 300,-1 ) ); 

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
	
	m_PlayStartSeqLabel = new wxStaticText( this, wxID_ANY, wxT("Play Start Seq"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PlayStartSeqLabel->Wrap( -1 );
	//m_PlayStartSeqLabel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
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
	
	m_PlayStartSeqValueLabel = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PlayStartSeqValueLabel->Wrap( -1 );
	m_LeftStatsValues->Add( m_PlayStartSeqValueLabel, 0, 0, 5 );
	
	m_StatsSizer->Add( m_LeftStatsValues, 1, wxEXPAND, 5 );
	
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
	
	m_PlayEndSeqLabel = new wxStaticText( this, wxID_ANY, wxT("Play End Seq"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PlayEndSeqLabel->Wrap( -1 );
	m_RightStats->Add( m_PlayEndSeqLabel, 0, 0, 5 );
	
	m_StatsSizer->Add( m_RightStats, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_RightStatsValues;
	m_RightStatsValues = new wxBoxSizer( wxVERTICAL );
	
	m_RTPPortValueLabel = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_RTPPortValueLabel->Wrap( -1 );
	m_RightStatsValues->Add( m_RTPPortValueLabel, 0, 0, 5 );
	
	m_ReportPortValueLabel = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
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
	
	m_PlayEndSeqValueLabel = new wxStaticText( this, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PlayEndSeqValueLabel->Wrap( -1 );
	m_RightStatsValues->Add( m_PlayEndSeqValueLabel, 0, 0, 5 );
	
	m_StatsSizer->Add( m_RightStatsValues, 1, wxEXPAND, 5 );
	
	m_LogSizer->Add( m_StatsSizer, 0, wxEXPAND, 5 );
	
	m_MainSizer->Add( m_LogSizer, 0, wxEXPAND, 5 );
	
	m_StatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_MenuBar = new wxMenuBar( 0 );
	m_FileMenu = new wxMenu();
	wxMenuItem* m_FileOpen;
	m_FileOpen = new wxMenuItem( m_FileMenu, wxID_ANY, wxString( wxT("Open File") ) , wxEmptyString, wxITEM_NORMAL );
	m_FileMenu->Append( m_FileOpen );
	
	wxMenuItem* m_separator1;
	m_separator1 = m_FileMenu->AppendSeparator();
	
	wxMenuItem* m_FileExit;
	m_FileExit = new wxMenuItem( m_FileMenu, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_FileMenu->Append( m_FileExit );
	
          m_MenuBar->Append( m_FileMenu, wxT("File") ); 

          m_SchedulerMenu = new wxMenu();
	wxMenuItem* m_SchedulerCS;
	m_SchedulerCS = new wxMenuItem( m_SchedulerMenu, wxID_ANY, wxString( wxT("CS Streaming") ) , wxEmptyString, wxITEM_RADIO );
	m_SchedulerMenu->Append( m_SchedulerCS );
	
	wxMenuItem* m_SchedulerP2P;
	m_SchedulerP2P = new wxMenuItem( m_SchedulerMenu, wxID_ANY, wxString( wxT("P2P Streaming") ) , wxEmptyString, wxITEM_RADIO );
	m_SchedulerMenu->Append( m_SchedulerP2P );
	
	m_MenuBar->Append( m_SchedulerMenu, wxT("Scheduler") ); 
	
    this->SetMenuBar( m_MenuBar );

	this->SetSizer( m_MainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );

	//Set the Timer and start it
	m_Timer = new wxTimer;
	//m_Timer->SetOwner(this);
          //m_Timer->Start(1000);

          //Create Display Refresh Timer
          m_DisplayTimer = NULL;

          wxIPV4address remote;
        remote.Hostname(_T("www.google.com"));
        remote.Service(80);
 
        wxIPV4address local, m_localaddr;
 
        wxSocketClient* client = new wxSocketClient;
        if(client->Connect(remote)) client->GetLocal(local);
        client->Destroy();

        wxIPV4address locAddr;
       
        locAddr.Hostname(local.IPAddress());
          locAddr.Service(20000);
 
          m_RTPSendSocket = new wxDatagramSocket(locAddr, wxSOCKET_NOWAIT);
           
          if(m_RTPSendSocket->Ok())
	{
              m_RTPSendSocket->GetLocal(locAddr);
              //m_InReportSocket->SetNotify(false);
              log(wxString::Format("RTP Socket %d %s\n", locAddr.Service(), locAddr.IPAddress()));
	}
	m_RTPPortLabel->SetLabelText(wxString::Format("RTP Port "));
	m_RTPPortValueLabel->SetLabelText(wxString::Format(": %d", locAddr.Service()));
	int sf;
          int len = sizeof(sf);
	m_RTPSendSocket->GetOption(SOL_SOCKET, SO_SNDBUF, &sf, &len);
          printf("Socket %d\n", sf);
          sf = 1514;
          m_RTPSendSocket->SetOption(SOL_SOCKET, SO_SNDBUF, &sf, len);
          m_RTPSendSocket->GetOption(SOL_SOCKET, SO_SNDBUF, &sf, &len);
          printf("Socket %d\n", sf);
	wxIPV4address LocalAddress;

        //LocalAddress.Hostname(local.IPAddress());
        LocalAddress.AnyAddress();
          LocalAddress.Service(30000);

          m_InReportSocket = new wxDatagramSocket(LocalAddress, wxSOCKET_NOWAIT );
          
          if(m_InReportSocket->Ok())
	{
              m_InReportSocket->GetLocal(LocalAddress);
              m_InReportSocket->SetNotify(false);
              log(wxString::Format("Report Socket %d %s\n", LocalAddress.Service(), LocalAddress.IPAddress()));
	}
	m_ReportPortLabel->SetLabelText(wxString::Format("Report Port "));
	m_ReportPortValueLabel->SetLabelText(wxString::Format(": %d", LocalAddress.Service()));
          m_localaddr.Hostname(local.IPAddress());
	m_localaddr.Service(0);

          m_OutReportSocket = new wxDatagramSocket (m_localaddr, wxSOCKET_NOWAIT );
	
	if(m_OutReportSocket->Ok())
	{
              m_OutReportSocket->SetNotify(false);
              m_OutReportSocket->GetLocal(m_localaddr);
              log(wxString::Format("Fuck all Socket %d %s\n", m_localaddr.Service(), m_localaddr.IPAddress()));
	}

	m_RootNode = new TrackerPeerState(local.IPAddress().ToStdString(), "",  locAddr.Service(), 0, LocalAddress.Service(), 20000.0, 0.0);
	m_NetworkTree = initializeTree(m_RootNode);

        m_localaddr.AnyAddress();
        m_localaddr.Service(0);

        m_SenderSock = NULL;

        wxString ipAddr=local.IPAddress();
        log(ipAddr);
	// Server address
	wxIPV4address serverAddr;
	serverAddr.Hostname();
	serverAddr.Service(3000);
	// Create the server socket
	m_Server = new wxSocketServer(serverAddr,wxSOCKET_WAITALL|wxSOCKET_REUSEADDR);
	// Setup the event handler and subscribe to connection events
	m_Server->SetEventHandler(*this, LISTENING_SOCKET_ID);
    m_Server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    m_Server->Notify(true);
	// We use Ok() here to see if the server is really listening
	if (!m_Server->Ok()) {
		log(wxT("Could not listen at the specified port!"));
	}
	else {
		log(wxT("Server is up!"));
		wxIPV4address l;
		l.Hostname(wxGetFullHostName());
                    l.Service(3000);
		log(l.IPAddress());
                    SetTitle(wxString::Format("FLOW Server (%s//%d)", local.IPAddress(), l.Service())); 
                    m_ServerAddress = local.IPAddress();
                     //Populate Tree List
                    wxTreeItemId idRoot = m_PeerList->GetRootItem();
                    if (idRoot.IsOk())
                    {
                        m_PeerList->AppendItem(idRoot, wxString::Format("FLOW Server (%s//%d)", l.IPAddress(), l.Service()));
                    }
	}
          m_Screen->SetDoubleBuffered(true);
	//m_WorkerPool = new WorkerPool(this);
	m_CBuffer = NULL;
	m_DecoderQueue = NULL;
          m_SenderQueue = NULL;
	m_InHandle = NULL;
          m_NextRTPport = 50000;
          m_FileOpened = false;
          m_SessionType = CS_SESSION;
          m_PRIMEState = new PRIMEState;
          m_IsRendered = true;
          m_DisplayTimer = NULL;
          m_SenderSock = NULL;
	//GUI Callbacks
	this->Connect(m_FileOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnFileOpen ) );
          //this->Connect(m_Timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(MainFrame::OnTimerTick), NULL, this);
	this->Connect(wxEVT_INPUT_READY, wxCommandEventHandler(MainFrame::OnInputReady), NULL, this);
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnMainFrameClose ) );
          this->Connect( m_SchedulerCS->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnCSStreamingSelected ) );
	this->Connect( m_SchedulerP2P->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnP2PStreamingSelected ) );
	m_PlayEndSeqLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_PlayStartSeqLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_PlayEndSeqValueLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_PlayStartSeqValueLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_staticText3->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_staticText4->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_staticText5->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_staticText6->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
          m_ConnectedPeersLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_staticText8->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_RTPPortLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_RTPPortValueLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_ReportPortLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_ReportPortValueLabel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_staticText17->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_staticText18->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_staticText19->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );
	m_staticText20->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( MainFrame::OnEraseBackGround ) );

	//Network Callbacks
	this->Connect( LISTENING_SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnServerEvent ), NULL, this );

	//Custom Events
	this->Connect( wxEVT_LOG_ENTRY, wxCommandEventHandler(MainFrame::OnLogEntry), NULL, this);
	this->Connect( wxEVT_BUFFER_FULL, wxCommandEventHandler(MainFrame::OnFullBuffer), NULL, this);
          this->Connect( wxEVT_INC_PRIME_MESSAGE, wxCommandEventHandler( MainFrame::OnIncMessage ), NULL, this );
	this->Connect( wxEVT_PEER_DEPARTURE, wxCommandEventHandler( MainFrame::OnPeerDeparture ), NULL, this );

          m_ReportQueue = new JobQueue;
          Job init_j(Job::THREAD_READ_DATA_ID, NULL);
          m_ReportQueue->AddNewJob(init_j);
          ReportMonitor* dect = new ReportMonitor(this, m_ReportQueue, m_InReportSocket, m_OutReportSocket);
            if (dect->Create() == wxTHREAD_NO_ERROR) {
                dect->Run();
            }
            else
            {
                log("Server : Unable to start DecodingThread");
            }
	//this->Connect(SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnSocketEvent ), NULL, this );
	//this->Connect(UDPSOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler( MainFrame::OnBroadcastEvent ), NULL, this );
}

void MainFrame::OnCSStreamingSelected( wxCommandEvent& event ) {
    if(!m_FileOpened)
        m_SessionType = CS_SESSION;
}

void MainFrame:: OnP2PStreamingSelected( wxCommandEvent& event ) {
    if(!m_FileOpened)
        m_SessionType = P2P_SESSION;
}

void MainFrame::OnPeerDeparture( wxCommandEvent& event ) {
    wxString key = wxString::Format("%s//%d", event.GetString(), event.GetInt());
    PeerState *ps;
    log(wxString::Format("He left %s\n", key));
    wxPrintf("He left %s\n", key);
    printf("Size before %d\n", m_PRIMEState->m_ConnectedPeers.size());
    ps = m_PRIMEState->m_ConnectedPeers[key];
    if(ps) {
        m_PeerList->Delete(ps->m_NodeId);
        m_PRIMEState->m_ConnectedPeers.erase(key);
    }
    printf("Size after %d\n", m_PRIMEState->m_ConnectedPeers.size());
    //printf("Size after %d\n", m_PRIMEState->m_DiffPeers.size());

    if(ps) {
        wxMutexLocker lock(m_PRIMEState->m_DiffGuard);
        for(int i = 0; i < m_PRIMEState->m_DiffPeers.size(); i++) {
	  if ( ( m_PRIMEState->m_DiffPeers[i]->m_Addr.IPAddress() == event.GetString() ) &&
	      ( m_PRIMEState->m_DiffPeers[i]->m_RepPort == event.GetInt() ) ) {
		m_PRIMEState->m_DiffPeers.erase(m_PRIMEState->m_DiffPeers.begin() + i);
	  }
        }
        Job j(Job::THREAD_EXIT_ID, NULL);
        ps->m_Queue->AddNewJob(j);
        //delete ps->m_Queue;
        //ps->m_Socket->Destroy();
        //delete ps;
    }
}

FlowTree* MainFrame::initializeTree(TrackerPeerState* rootNode){

	//Create Root
	PRIMENode* root = new PRIMENode;
	root->diffParents.clear();
	root->diffChildren.clear();
	root->swarmChildren.clear();
	root->swarmParents.clear();

	root->data = rootNode;
	root->diffSubTreeID = 0;		// 0 means it is the root
	root->level = 0;				// 0 means it is the root

	FlowTree* networkTree = new FlowTree;
	networkTree->insertRoot(root);

	return networkTree;
}

/* This method is used to enter a new peer in the network tree. */
std::vector<StateModification> MainFrame::bootstrapNewNode(FlowTree* tree, TrackerPeerState* newNode) {
    std::vector <StateModification> stateModifications;

    PRIMENode *newPRIMENode = new PRIMENode;
    newPRIMENode->data = newNode;

    newPRIMENode->diffParents.clear();
    newPRIMENode->diffChildren.clear();
    newPRIMENode->swarmChildren.clear();
    newPRIMENode->swarmParents.clear();

    //the new node will have to modify everything. modifications will be added last.
    //peersWithModifiedStates.push_back(newPRIMENode);

    //tree->discoverDiffusionParent(newPRIMENode, &peersWithModifiedStates);
    //tree->discoverSwarmingParents(newPRIMENode, &peersWithModifiedStates);
    tree->discoverDiffusionParent(newPRIMENode, &stateModifications);
    tree->discoverSwarmingParents(newPRIMENode, &stateModifications);

    //return peersWithModifiedStates;
    return stateModifications;
}

void MainFrame::OnIncMessage( wxCommandEvent& evt ) {
     PeerMessage* pm = (PeerMessage*)evt.GetClientData();
     PRIME::PRIMEMessage* msg = pm->m_Msg;

    log(wxString::Format("Type %d", msg->type()));
    switch(msg->type()) {
    case PRIME::PRIMEMessage_MsgType_PRIME_REPLY:
        break;
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
        break;
    }
    delete msg;
    delete pm;
}

void MainFrame::DecodeRequest(wxCommandEvent& evt, PRIME::Request& req) {
    switch( req.type() ) {
    case PRIME::Request_RequestType_CS_SESSION_INIT:
    {
        PRIME::Request_CSSIRequest csrep = req.cs_session_props();
        HandleCSSIRequest(evt, csrep);

        break;
    }
    case PRIME::Request_RequestType_P2P_SESSION_INIT:
    {
        PRIME::Request_P2PSIRequest p2prep = req.p2p_session_props();
        HandleP2PSIRequest(evt, p2prep);

        break;    
    }
    default:
        break;
    }
}
void MainFrame::DecodeReply(wxCommandEvent& evt, PRIME::Reply& repl) {}

void MainFrame::DecodeReport(wxCommandEvent& evt, PRIME::Report& rep) {
    switch( rep.type() ) {
    case PRIME::Report_ReportType_ADD_DIFF_PARENT_TYPE:
    {
        //PRIME::Report_DiffParentReport diffpa = rep.diff_parent_report();
        //HandleDiffParentReport(evt, diffpa);
        break;
    }
    /*case PRIME::Report_ReportType_DIFF_CHILD_REPORT_TYPE:
    {
        break;    
    }
    case PRIME::Report_ReportType_SWARM_PARENT_REPORT_TYPE:
    {
        break;    
    }
    case PRIME::Report_ReportType_SWARM_CHILD_REPORT_TYPE:
    {
        break;    
    }*/
    default:
        break;
    }
}
/*
void MainFrame::HandleDiffParentReport(wxCommandEvent& evt, PRIME::Report_DiffParentReport& diffpa) {
    PeerMessage* pm = (PeerMessage*)evt.GetClientData();
    wxString key = wxString::Format("%s//%d", diffpa.peername().c_str(), diffpa.rep_port());
    PeerState* ps = m_PRIMEState->m_ConnectedPeers[key];
    
    //log(wxT("Serving Someone"));
    ps->m_RepPort = diffpa.rep_port();
    ps->m_StreamPort = diffpa.stream_port();
    ps->m_PeerName = diffpa.peername();
    ps->m_Addr.Service(diffpa.stream_port());

    //log(wxString::Format("Size %d", (int)m_PRIMEState->m_ConnectedPeers.size()));
    m_PRIMEState->m_DiffPeers.push_back(ps);
    //log(wxString::Format("Size %d", (int)m_PRIMEState->m_DiffPeers.size()));
}
*/

void MainFrame::HandleCSSIRequest(wxCommandEvent& evt, PRIME::Request_CSSIRequest& csreq) {
    PeerMessage* pm = (PeerMessage*)evt.GetClientData();
    PeerState* ps = pm->m_Peer;

    ps->m_UBW = 0.0f;
    ps->m_DBW = csreq.dbw();
    ps->m_RepPort = csreq.repport();
    ps->m_PeerName = csreq.peername();
    ps->m_Addr.Hostname(evt.GetString());

    wxString key = wxString::Format("%s//%d", evt.GetString(), ps->m_RepPort);
    m_PRIMEState->addPeer(key, ps);

    wxTreeItemId idRoot = m_PeerList->GetRootItem();
    if (idRoot.IsOk())
    {
        wxTreeItemIdValue cookie;
        wxTreeItemId slot = m_PeerList->GetFirstChild(idRoot, cookie);
        ps->m_NodeId = m_PeerList->AppendItem(slot, key);;
    }

    PRIME::PRIMEMessage* msg = new PRIME::PRIMEMessage;
    PRIME::Reply rep;
      
    PRIME::Reply_CSSIReply sess;
    rep.set_type(PRIME::Reply_ReplyType_CS_SESSION_INIT);
    sess.mutable_diffparent()->set_ipaddr(m_ServerAddress);
    wxIPV4address LocalAddress;
    m_InReportSocket->GetLocal(LocalAddress);
    sess.mutable_diffparent()->set_repport(LocalAddress.Service());
    m_RTPSendSocket->GetLocal(LocalAddress);
    sess.mutable_diffparent()->set_rtpport(LocalAddress.Service());
    sess.set_sdp(m_OutHandler->sdp_desc);
    sess.set_width(m_InHandle->video_dec_ctx->width);
    sess.set_height(m_InHandle->video_dec_ctx->height);
    sess.set_pixfmt(m_InHandle->video_dec_ctx->pix_fmt);
    sess.set_maxframecount(m_FrameQueue->m_MaxFrameCount);
    sess.set_streamport(m_NextRTPport);
    rep.mutable_cs_session_state()->CopyFrom(sess);
    msg->set_type(PRIME::PRIMEMessage_MsgType_PRIME_REPLY);
    msg->mutable_reply()->CopyFrom(rep);

    Job j(Job::THREAD_WRITE_DATA_ID, msg);
    ps->m_Queue->AddNewJob(j);

    PRIME::PRIMEMessage holePunchMsg;
    holePunchMsg.set_type(PRIME::PRIMEMessage_MsgType_PRIME_HOLE_PUNCH);
    holePunchMsg.set_dummy("HOLE PUNCH!!!!");
    PRIMEPacket* pkt = new PRIMEPacket(holePunchMsg);
    wxIPV4address addr; 
    addr.Hostname(ps->m_Addr.IPAddress());
    addr.Service(ps->m_RepPort);
    pkt->m_Destination = addr;

    Job j2(Job::THREAD_HOLE_PUNCH_ID, pkt);
    m_ReportQueue->AddNewJob(j2);

    //log(wxT("Serving Someone"));
    ps->m_StreamPort = m_NextRTPport;
    ps->m_Addr.Service(m_NextRTPport);

    //log(wxString::Format("Size %d", (int)m_PRIMEState->m_ConnectedPeers.size()));
    m_PRIMEState->m_DiffPeers.push_back(ps);
    //log(wxString::Format("Size %d", (int)m_PRIMEState->m_DiffPeers.size()));
    m_NextRTPport += 10;

    log(csreq.peername());
}

void MainFrame::HandleP2PSIRequest(wxCommandEvent& evt, PRIME::Request_P2PSIRequest& p2preq) {
    log(wxString::Format("P2P Request %s %s", p2preq.ipaddr(), evt.GetString()));

    PeerMessage* pm = (PeerMessage*)evt.GetClientData();
    PeerState* ps = pm->m_Peer;

    ps->m_UBW = p2preq.ubw();
    ps->m_DBW = p2preq.dbw();
    ps->m_RepPort = p2preq.repport();
    ps->m_PeerName = p2preq.peername();
    ps->m_Addr.Hostname(evt.GetString().ToStdString());

    wxString key = wxString::Format("%s//%d", evt.GetString(), ps->m_RepPort);
    m_PRIMEState->addPeer(key, ps);

    TrackerPeerState *newNode = new TrackerPeerState(evt.GetString().ToStdString(), "",  m_NextRTPport, 0, p2preq.repport(), 9000.0, 10000.0);
    vector <StateModification> stateModifications;

    stateModifications = bootstrapNewNode(m_NetworkTree, newNode);

    for(int i = 0; i < stateModifications.size(); i++ ) {        
        PRIME::PRIMEMessage* msg = new PRIME::PRIMEMessage;
        switch(stateModifications[i].kindOfModification) {
        case 0:
        {
	  if (stateModifications[i].entryIPv4 == m_RootNode->m_Addr
	      && stateModifications[i].entryRepPort == m_RootNode->m_RepPort) {
	      PRIME::Reply rep;
      
	      PRIME::Reply_P2PSIReply sess;
	      rep.set_type(PRIME::Reply_ReplyType_P2P_SESSION_INIT);
	      sess.mutable_diffparent()->set_ipaddr(stateModifications[i].entryIPv4);
	      wxIPV4address LocalAddress;
	      m_InReportSocket->GetLocal(LocalAddress);
	      sess.mutable_diffparent()->set_repport(LocalAddress.Service());
	      m_RTPSendSocket->GetLocal(LocalAddress);
	      sess.mutable_diffparent()->set_rtpport(LocalAddress.Service());
	      sess.set_sdp(m_OutHandler->sdp_desc);
	      sess.set_width(m_InHandle->video_dec_ctx->width);
	      sess.set_height(m_InHandle->video_dec_ctx->height);
	      sess.set_pixfmt(m_InHandle->video_dec_ctx->pix_fmt);
	      sess.set_maxframecount(m_FrameQueue->m_MaxFrameCount);
	      sess.set_streamport(m_NextRTPport);
	      rep.mutable_p2p_session_state()->CopyFrom(sess);
	      msg->set_type(PRIME::PRIMEMessage_MsgType_PRIME_REPLY);
	      msg->mutable_reply()->CopyFrom(rep);

	      Job j(Job::THREAD_WRITE_DATA_ID, msg);
	      ps->m_Queue->AddNewJob(j);

	      PRIME::PRIMEMessage holePunchMsg;
	      holePunchMsg.set_type(PRIME::PRIMEMessage_MsgType_PRIME_HOLE_PUNCH);
	      holePunchMsg.set_dummy("HOLE PUNCH!!!!");
	      PRIMEPacket* pkt = new PRIMEPacket(holePunchMsg);
	      wxIPV4address addr; 
	      addr.Hostname(stateModifications[i].recipientsIPv4);
	      addr.Service(stateModifications[i].recipientsRepPort);
	      pkt->m_Destination = addr;

	      Job j2(Job::THREAD_HOLE_PUNCH_ID, pkt);
	      m_ReportQueue->AddNewJob(j2);

	      ps->m_StreamPort = m_NextRTPport;
	      ps->m_Addr.Service(m_NextRTPport);

	      m_PRIMEState->m_DiffPeers.push_back(ps);
	  } else {
	  
	  }
	  break;
        }
        case 1:
	  {break;}
        case 2:
        {
	  /*
	  if (stateModifications[i].recipientsIPv4 == m_RootNode->m_Addr
	      && stateModifications[i].recipientsRepPort == m_RootNode->m_RepPort) {
	      wxTreeItemId idRoot = m_PeerList->GetRootItem();

	      if (idRoot.IsOk())
	      {
		wxTreeItemIdValue cookie;
		wxTreeItemId slot = m_PeerList->GetFirstChild(idRoot, cookie);
		ps->m_NodeId = m_PeerList->AppendItem(slot, key);
	      }
	  }
	  PRIME::Reply rep;
      
	  PRIME::Reply_P2PSIReply sess;
	  rep.set_type(PRIME::Reply_ReplyType_P2P_SESSION_INIT);
	  sess.mutable_diffparent()->set_ipaddr(stateModifications[i].entryIPv4);
	  wxIPV4address LocalAddress;
	  m_InReportSocket->GetLocal(LocalAddress);
	  sess.mutable_diffparent()->set_repport(LocalAddress.Service());
	  m_RTPSendSocket->GetLocal(LocalAddress);
	  sess.mutable_diffparent()->set_rtpport(LocalAddress.Service());
	  sess.set_sdp(m_OutHandler->sdp_desc);
	  sess.set_width(m_InHandle->video_dec_ctx->width);
	  sess.set_height(m_InHandle->video_dec_ctx->height);
	  sess.set_pixfmt(m_InHandle->video_dec_ctx->pix_fmt);
	  sess.set_maxframecount(m_FrameQueue->m_MaxFrameCount);
	  sess.set_streamport(m_NextRTPport);
	  rep.mutable_cs_session_state()->CopyFrom(sess);
	  msg->set_type(PRIME::PRIMEMessage_MsgType_PRIME_REPLY);
	  msg->mutable_reply()->CopyFrom(rep);

	  Job j(Job::THREAD_WRITE_DATA_ID, msg);
	  ps->m_Queue->AddNewJob(j);

	  PRIME::PRIMEMessage holePunchMsg;
	  holePunchMsg.set_type(PRIME::PRIMEMessage_MsgType_PRIME_HOLE_PUNCH);
	  holePunchMsg.set_dummy("HOLE PUNCH!!!!");
	  PRIMEPacket* pkt = new PRIMEPacket(holePunchMsg);
	  wxIPV4address addr; 
	  addr.Hostname(ps->m_Addr.IPAddress());
	  addr.Service(ps->m_RepPort);
	  pkt->m_Destination = addr;

	  Job j2(Job::THREAD_HOLE_PUNCH_ID, pkt);
	  m_ReportQueue->AddNewJob(j2);*/
	  break;
        }
        case 3:
	  {break;}
        case 4:
        {
	  PRIME::Report rep;
      
	  PRIME::Report_AddSwarmParentReport sess;
	  wxString key = wxString::Format("%s//%d", stateModifications[i].entryIPv4, stateModifications[i].entryRepPort);
	  PeerState* remote_ps = m_PRIMEState->m_ConnectedPeers[key];
	  key = wxString::Format("%s//%d", stateModifications[i].recipientsIPv4, stateModifications[i].recipientsRepPort);
	  PeerState* recipients_ps = m_PRIMEState->m_ConnectedPeers[key];

	  rep.set_type(PRIME::Report_ReportType_ADD_SWARM_PARENT_TYPE);
	  sess.mutable_swarmparent()->set_ipaddr(stateModifications[i].entryIPv4);
	  sess.mutable_swarmparent()->set_repport(stateModifications[i].entryRepPort);
	  sess.mutable_swarmparent()->set_rtpport(remote_ps->m_TCPPort);
	  rep.mutable_add_swarm_parent_report()->CopyFrom(sess);
	  msg->set_type(PRIME::PRIMEMessage_MsgType_PRIME_REPORT);
	  msg->mutable_report()->CopyFrom(rep);

	  Job j(Job::THREAD_WRITE_DATA_ID, msg);
	  recipients_ps->m_Queue->AddNewJob(j);
	  break;
        }
        case 5:
        {
	  break;
        }
        case 6:
        {
	  PRIME::Report rep;
      
	  PRIME::Report_AddSwarmChildReport sess;
	  wxString key = wxString::Format("%s//%d", stateModifications[i].entryIPv4, stateModifications[i].entryRepPort);
	  PeerState* remote_ps = m_PRIMEState->m_ConnectedPeers[key];
	  key = wxString::Format("%s//%d", stateModifications[i].recipientsIPv4, stateModifications[i].recipientsRepPort);
	  PeerState* recipients_ps = m_PRIMEState->m_ConnectedPeers[key];

	  rep.set_type(PRIME::Report_ReportType_ADD_SWARM_CHILD_TYPE);
	  sess.mutable_swarmchild()->set_ipaddr(stateModifications[i].entryIPv4);
	  sess.mutable_swarmchild()->set_repport(stateModifications[i].entryRepPort);
	  sess.mutable_swarmchild()->set_rtpport(remote_ps->m_StreamPort);
	  rep.mutable_add_swarm_child_report()->CopyFrom(sess);
	  msg->set_type(PRIME::PRIMEMessage_MsgType_PRIME_REPORT);
	  msg->mutable_report()->CopyFrom(rep);

	  Job j(Job::THREAD_WRITE_DATA_ID, msg);
	  recipients_ps->m_Queue->AddNewJob(j);

	  break;
        }
        default:
	  break;
        }
    }

    m_NextRTPport+=10;
}

MainFrame::~MainFrame()
{
	// Destroy Server
	m_Server->Destroy();
	
	//Clear Timer
	if ( m_Timer->IsRunning() )
		m_Timer->Stop();

	delete m_Timer;

	//if ( m_WorkerPool ) {
	//	delete m_WorkerPool;
	//}

	//Clear Thread Pools
	//typedef std::map< unsigned long , JobQueue* >::iterator it_type;
	//for(it_type iterator = m_initTPool.begin(); iterator != m_initTPool.end(); iterator++) {
	//	delete iterator->second;
	//}
}

void MainFrame::OnMainFrameClose(wxCloseEvent& WXUNUSED(event)) {
	::google::protobuf::ShutdownProtobufLibrary();

    m_IsRendered = false;        
    if(m_DecoderQueue) {
        Job j(Job::THREAD_EXIT_ID, NULL);
        m_DecoderQueue->AddNewJob(j);
    }
         
    if(m_SenderQueue) {
        Job j(Job::THREAD_EXIT_ID, NULL);
        m_SenderQueue->AddNewJob(j);
    }

    if(m_ReportQueue) {
        Job j(Job::THREAD_EXIT_ID, NULL);
        m_ReportQueue->AddNewJob(j);
    }

    typedef std::map< wxString, PeerState* >::iterator it_type;
    for(it_type iter = m_PRIMEState->m_ConnectedPeers.begin(); iter != m_PRIMEState->m_ConnectedPeers.end(); iter++) {
        Job j(Job::THREAD_EXIT_ID, NULL);
        iter->second->m_Queue->AddNewJob(j);
    }

    delete m_CBuffer;
    delete m_PRIMEState;
    //delete m_RootNode;
    //delete m_NetworkTree;

    if(m_DisplayTimer) 
        delete m_DisplayTimer;

    if(m_SenderSock)
        m_SenderSock->Destroy();

    if(m_RTPSendSocket)
        m_RTPSendSocket->Destroy();

    //delete m_FrameQueue;
    wxThread::Sleep(1000);
	
    Destroy();
}

// no AV sync correction is done if below the AV sync threshold
#define AV_SYNC_THRESHOLD 0.01
// no AV correction is done if too big error
#define AV_NOSYNC_THRESHOLD 10.0

static int counter = 0;
static double luda = 0.008f;
    static double start = 0.0f;
static double factor = 0.0f;
static int frames = 0;
static double now = 0, last = 0;

void MainFrame::OnDisplayRefresh(wxTimerEvent& evt) {
    double actual_delay, delay, sync_threshold, ref_clock, diff;
    int ret;
    
    VideoFrame vf;
    m_InHandle->m_FrameTimer = av_gettime() / 1000000.0;
    luda = m_InHandle->m_FrameTimer - start;
    
    if(!m_IsRendered)
        return;

    //Job j(Job::THREAD_SEND_CHUNK_ID, NULL);
    //m_SenderQueue->AddNewJob(j);
    //printf("Ludacris %g %f\n",m_InHandle->m_LastDelay - luda, m_InHandle->m_LastDelay-luda);
    factor = m_InHandle->m_LastDelay - luda;
    if (factor<0.0) {
        factor = -factor;
    }
    if (factor > m_InHandle->m_LastDelay)
        factor = 0.01f;
    //printf("Ludacris %g %f\n",factor, factor);
    //factor = 0.008;
    if(m_FileOpened) {
        ret = m_FrameQueue->get(&vf);
        if(ret == 1) {
	  now = av_gettime() / 1000000.0;
	  printf("Ludacris %g %f %d\n",now-last,now-last, frames);
	  last = now;
	  m_FrameQueue->swapQueues();
            m_FrameQueue->m_NextInputFrameIndex = 0;
            m_CBuffer->swapQueues();
            m_PlayEndSeqValueLabel->SetLabelText(wxString::Format(": %d", m_CBuffer->play_end_seq));
	  m_PlayStartSeqValueLabel->SetLabelText(wxString::Format(": %d", m_CBuffer->play_start_seq));
	  
            Job j(Job::THREAD_NEXT_CHUNK_ID, NULL);
            m_DecoderQueue->AddNewJob(j);

	  typedef std::vector<PeerState*>::iterator it_type;
            for(int i = 0; i < m_PRIMEState->m_DiffPeers.size(); i++) {
	      PRIMEPacket* pkt = m_CBuffer->getDiffReport();
                wxIPV4address addr; 
                addr.Hostname(m_PRIMEState->m_DiffPeers[i]->m_Addr.IPAddress());
                //addr.Hostname("192.168.1.87");
                addr.Service(m_PRIMEState->m_DiffPeers[i]->m_RepPort);
                pkt->m_Destination = addr;
                Job j(Job::THREAD_WRITE_DATA_ID, (void*)pkt);
                m_ReportQueue->AddNewJob(j); 
            }

	  if(m_SessionType == CS_SESSION) {
	      Job j2(Job::THREAD_SEND_CHUNK_ID, NULL);
	      m_SenderQueue->AddNewJob(j2);
	  } else {
	      Policy* pol = new Policy;
	      Job j2(Job::THREAD_SEND_CHUNK_WITH_POLICY_ID, (void*)pol);
	      m_SenderQueue->AddNewJob(j2);
	  }
	  frames=-1;
        }
        frames++;    
        counter++;
        delay = vf.m_PTS - m_InHandle->m_LastPTS; // the pts from last time
        if(delay <= 0 || delay >= 1.0) {
            // if incorrect delay, use previous one
            delay = m_InHandle->m_LastDelay;
        }
        //printf("Max FPS %g\n", vf.m_PTS);
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
      //printf("Stuff happening %d %g %g", clock, delay, luda);
  } else {
      m_DisplayTimer->Start(1000, wxTIMER_ONE_SHOT);
  }
    
}

void MainFrame::OnFullBuffer (wxCommandEvent& evt) {
	//log(wxT("Buffer Full"));
	typedef std::map< wxString, PeerState* >::iterator it_type;
	/*m_CBuffer->lock();
	int size = m_CBuffer->GetSize();
	for( int i = 0; i < size; i++ ) {
		DeliverableContent* cont = new DeliverableContent(m_CBuffer->PopChunk(), m_ConnectedPeers.size());
		for(it_type iter = m_ConnectedPeers.begin(); iter != m_ConnectedPeers.end(); iter++) {
			//log(wxT("Buffer Full Event"));
			ContentDestination* dest = new ContentDestination(iter->second, cont);
			Job j(Job::THREAD_SEND_CHUNK_ID, (void*)dest);
			m_WorkerPool->AssignJob(j);
		}
	}
	m_CBuffer->unlock();*/
}

static const wxChar *FILETYPES =    _T("FLV files|*.flv|")
			      _T("MP4 files|*.mp4|")
                                    _T("AVI files|*.avi|")
                                    _T("All files|*.*");

void MainFrame::OnFileOpen( wxCommandEvent& event ) {
    wxFileDialog* openFileDialog =
                        new wxFileDialog( this, _("Open File"), "", "", FILETYPES, wxFD_OPEN, wxDefaultPosition);
    
    if ( openFileDialog->ShowModal() == wxID_OK )
    {
        wxString path;
        path.append( openFileDialog->GetDirectory() );
        path.append( wxFileName::GetPathSeparator() );
        path.append( openFileDialog->GetFilename() );
        wxString str = wxT("Opening ") + path + wxT("...");

        this->m_StatusBar->SetStatusText(path);
        JobQueue* decQueue = new JobQueue;
        m_DecoderQueue = decQueue;
       
        m_FrameQueue = new FrameQueue;
        
        Job j(Job::THREAD_OPEN_FILE_ID, NULL);
        decQueue->AddNewJob(j);

        if (m_CBuffer == NULL) {
            m_CBuffer = new PacketQueue;
        }

        DecodingThread* dect = new DecodingThread(this, path, decQueue, m_CBuffer,
                               m_FrameQueue);
        if (dect->Create() == wxTHREAD_NO_ERROR) {
            dect->Run();
        }
        else
        {
            log("Server : Unable to start DecodingThread");
        }
    }

    delete openFileDialog;
    //FLOWTreeFrame *fp = new FLOWTreeFrame(this);
	//Disable();
    //      fp->Show();
}

void MainFrame::OnServerEvent(wxSocketEvent& event)
{
	switch(event.GetSocketEvent())
    {
        case wxSOCKET_INPUT:
            //No way, we do not expect Input Events
            break;
        case wxSOCKET_OUTPUT:
            //No way, we do not expect Output Events
            break;
        case wxSOCKET_CONNECTION:
        {
            wxSocketBase* sock = m_Server->Accept();
            wxIPV4address addr;
            if (!sock->GetPeer(addr))
            {
                log(wxT("Server: cannot get peer info"));
            } else {
                JobQueue* q = new JobQueue;
                Job init_j(Job::THREAD_READ_DATA_ID, NULL);
                q->AddNewJob(init_j);
	      log(wxString::Format("Got Connection From %s//%d", addr.IPAddress(), addr.Service()));
                PeerState *newPeer = new PeerState(q, sock, addr);
                PeerConnectionMonitor* pconn = new PeerConnectionMonitor(this, newPeer);
                
                //if(m_SessionType == CS_SESSION)
                //    m_PRIMEState->addDiffChild(newPeer);

                if (pconn->Create() == wxTHREAD_NO_ERROR) {
                    pconn->Run();
                }
                else
                {
                    log("Server : Unable to start Peer Connection Monitor");
                }
            }
        }
        break;
        case wxSOCKET_LOST:
            //No way, we do not expect Socket Lost Events
        break;
    }
}
	
void MainFrame::OnNewRTPSession(NewRTPSessionEvent& evt ) {
	/*if (evt.m_PeerState->m_StreamPort < 0 ) {
		wxString str = wxString::Format("Could not start RTPSession with %s",evt.m_PeerState->m_Addr.Hostname());
		log(str);
	} else {
		wxString str = wxString::Format("Starting RTPSession with %s:%d",evt.m_PeerState->m_Addr.Hostname(),evt.m_PeerState->m_StreamPort);
		wxString peerRep = wxString::Format("%s:%d",evt.m_PeerState->m_Addr.Hostname(),evt.m_PeerState->m_StreamPort);
		log(str);
		wxTreeItemId idRoot = m_PeerList->GetRootItem();
        if (idRoot.IsOk())
        {
			m_PeerList->AppendItem(idRoot,peerRep);
        }
		m_ConnectedPeers.insert(std::pair<wxString,PeerState*>(evt.m_RemoteAddr,evt.m_PeerState));
	}*/
}

void MainFrame::OnBroadcastEvent(wxSocketEvent& evt)
{
	
}

void MainFrame::OnSocketEvent(wxSocketEvent& evt)
{
	
}

void MainFrame::log(const wxString text)
{
	wxCommandEvent* event = new wxCommandEvent(wxEVT_LOG_ENTRY, wxID_ANY);
	event->SetString(wxString(text));

	this->QueueEvent( event );
}

void MainFrame::OnLogEntry (wxCommandEvent& evt)
{
	m_Log->AppendText("Server : " + evt.GetString() + "\n");
}

void MainFrame::OnInputReady (wxCommandEvent& evt)
{
	if(evt.GetInt() != 0) {
		m_FileOpened = true;
		log("File Opened");
		//m_CBuffer->SetTimeInterval(evt.GetInt());
		m_DisplayTimer = new wxTimer;
	          m_DisplayTimer->SetOwner(this);
                    this->Connect(m_DisplayTimer->GetId(), wxEVT_TIMER, wxTimerEventHandler(MainFrame::OnDisplayRefresh), NULL, this);

                    m_SenderQueue = new JobQueue;
                    //Job init_j(Job::THREAD_SEND_CHUNK_ID, NULL);
                    //m_SenderQueue->AddNewJob(init_j);

                    wxIPV4address m_localaddr;
                    m_localaddr.AnyAddress();
                    m_localaddr.Service(0);

                    //m_SenderSock = new wxDatagramSocket (m_localaddr, wxSOCKET_NOWAIT);
	          //if (m_SenderSock->Error ())
	          //{
                    //    log("Problem with datagram socket");
	          //}

                    SenderThread* st = new SenderThread(this, m_SenderQueue, m_CBuffer, m_SessionType, m_RTPSendSocket);

                    if (st->Create() == wxTHREAD_NO_ERROR) {
                        st->Run();
                    }
                    else
                    {
                        log("Server : Unable to start Sender Thread");
                    }

                    m_DisplayTimer->Start(1,wxTIMER_ONE_SHOT);
	} else {
		m_FileOpened = false;
		log("Problem Opening File");
	}
}

void MainFrame::OnTimerTick(wxTimerEvent& evt)
{
	//Every second, new content is produced
	//We clear everything that' s left and proceed
	//We want to keep up with the source
	//log(wxT("Timer Event"));
	/*if ( m_CBuffer ) {
		m_CBuffer->Clear();
		//log("POLU!!!!");
		Job j(Job::THREAD_NEXT_CHUNK_ID, NULL);
		m_DecoderQueue->AddNewJob(j);
	}*/
}

FLOWTreeFrame::FLOWTreeFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
    wxBoxSizer* m_MainSizer;
    m_MainSizer = new wxBoxSizer( wxVERTICAL );
	
    m_TreeCanvas = new FLOWTreeDrawPanel( this );
    m_MainSizer->Add( m_TreeCanvas, 1, wxEXPAND, 5 );
    m_TreeCanvas->SetDoubleBuffered(true);

    this->SetSizer( m_MainSizer );
    this->Layout();
	
    this->Centre( wxBOTH );
}

FLOWTreeDrawPanel::FLOWTreeDrawPanel(wxFrame* parent) :
wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL) {
    //Init Values
    m_MouseClicked = false;
    m_TreeBitmap = new wxBitmap(2000,2000);
    m_TreeDC.SelectObject(*m_TreeBitmap);
    m_TreeDC.SetTextForeground(*wxBLACK);
    wxClientDC dc(this);
    m_TreeDC.SetBrush(dc.GetBackground());
    m_TreeDC.SetPen(*wxTRANSPARENT_PEN);
    m_TreeDC.DrawRectangle(0,0,2000,2000);
    m_TreeDC.SetPen(*wxBLACK_PEN);
    m_TreeDC.SetBrush(*wxBLUE_BRUSH);
    for(int i = 0; i <4; i++) {
        for(int j = 0; j < 4; j++ ) {
	  m_TreeDC.DrawCircle(250*(i+1),250*(j+1),100);
        }
    }
   // m_TreeDC.DrawText(wxT("Mpampis"),0,0);

    // Initialize Values
    m_DrawStartX = 0;
    m_DrawStartY = 0;
    m_MouseClicked = false;

    // Connect Events
    this->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( FLOWTreeDrawPanel::OnLeftClickPressed ), NULL, this );
    this->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( FLOWTreeDrawPanel::OnLeftClickReleased ), NULL, this );
    this->Connect( wxEVT_PAINT, wxPaintEventHandler( FLOWTreeDrawPanel::OnDrawTree ), NULL, this );
    this->Connect( wxEVT_MOTION, wxMouseEventHandler( FLOWTreeDrawPanel::OnMouseMoved ), NULL, this );
}

void FLOWTreeDrawPanel::OnLeftClickPressed( wxMouseEvent& event ) {
    //m_TreeDC.SetUserScale(1.5,1.5);
    m_MouseClicked = true;
    m_PrevX = event.GetX();
    m_PrevY = event.GetY();
    if(!HasCapture())
        CaptureMouse();
}

void FLOWTreeDrawPanel::OnLeftClickReleased( wxMouseEvent& event ) {
    m_MouseClicked = false;
    if(HasCapture())
        ReleaseMouse();
}

void FLOWTreeDrawPanel::OnMouseMoved( wxMouseEvent& event ) {
    if( m_MouseClicked ) {
        m_DrawStartX -= (event.GetX() - m_PrevX );
        m_DrawStartY -= (event.GetY() - m_PrevY );
        m_PrevX = event.GetX();
        m_PrevY = event.GetY();
        Refresh();
    }
}

void FLOWTreeDrawPanel::OnDrawTree( wxPaintEvent& event ) {
    //printf("Draw\n");
    wxPaintDC dc(this);
    //dc.DrawBitmap(*m_TreeBitmap,0,0);
    //dc.DrawText(wxT("Mpampis"), 0, 0);
    render(dc);
}

void FLOWTreeDrawPanel::render(wxDC& dc) {
    dc.Clear();
    dc.Blit(0,0,2000,2000,&m_TreeDC,m_DrawStartX,m_DrawStartY);
    dc.DrawSpline(0,100,100,200,0,300);
    wxRect rect(0,0,50,100);
    wxString s("adssadasdsadsda\nadadasdasdadadds");
    wxSize sz = dc.GetMultiLineTextExtent(s);
    dc.DrawRectangle(wxRect(0,0,sz.GetX(),sz.GetY()));
    dc.DrawLabel(s, wxRect(0,0,sz.GetX(),sz.GetY()));
}

FLOWTreeFrame::~FLOWTreeFrame()
{
}
