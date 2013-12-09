///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __INTERFACE_HEADER_H__
#define __INTERFACE_HEADER_H__

#include "../fwUtilities.h"
#include "CustomEvents.h"
//#include "ServerConnector.h"
//#include "WorkerPool.h"
//#include "ChunkBuffer.h"
//#include "ReceiverThread.h"
//#include "OutputHandler.h"

//Custom Event Types
DECLARE_LOCAL_EVENT_TYPE(wxEVT_LOG_ENTRY, -1)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_OUTPUT_READY, -1)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_BUFFER_UPDATED, -1)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_RTP_SESSION_PROPS, -1)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_NEW_OUTPUT, -1)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_INC_PRIME_MESSAGE, -1)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_INPUT_READY, -1)

//Forward Declare
class ConnectionPropertiesFrame;
class JobQueue;
class WorkerPool;
class ChunkBuffer;
class FLOWScreen;
class PacketQueue;
class FrameQueue;
class InputHandler;
class PRIMEState;

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
private:
	
protected:
    //FLOW State
    PacketQueue*			    m_CBuffer;
    JobQueue*	    		    m_ReceiverQueue;
    JobQueue*                               m_ConnMonitorQueue;
    JobQueue*                               m_ReportQueue;
    JobQueue*                               m_DecoderQueue;
    FrameQueue*                             m_FrameQueue;
    InputHandler*                           m_InHandle;
    PRIMEState*			    m_PRIMEState;

    //Thread Safety On Exit
    wxCriticalSection		    m_ExitCounterGuard;
    int				    m_ExitCounter;

    bool				    m_InputReady;

    //PRIME State
		//WorkerPool*				m_WorkerPool;
		//jrtplib::RTPSession*	m_RTPSess;
                    wxDatagramSocket*                                 m_InReportSocket;
                    wxDatagramSocket*                                 m_RTPRecvSocket;
		wxDatagramSocket*                                 m_RTPSendSocket;

                    wxString                                          m_IpAddr;
                    wxTimer*                                          m_SwarmReportTimer;
		wxTimer*					m_DisplayTimer;

		wxStatusBar*				m_StatusBar;
		wxMenuBar*				m_MenuBar;
		wxMenu*					m_FileMenu;
		wxMenu*					m_LogMenu;
		wxMenu*					m_AboutMenu;
		FLOWScreen*				m_Screen;
		wxTextCtrl*				m_Log;
		wxTreeCtrl*				m_PeerList;

		wxStaticText* m_ConnectedPeersLabel;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText5;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText8;
		wxStaticText* m_PlayStartSeqLabel;
		wxStaticText* m_staticText15;
		wxStaticText* m_staticText16;
		wxStaticText* m_staticText171;
		wxStaticText* m_staticText181;
		wxStaticText* m_staticText191;
		wxStaticText* m_staticText201;
		wxStaticText* m_staticText211;
		wxStaticText* m_RTPPortLabel;
		wxStaticText* m_ReportPortLabel;
		wxStaticText* m_staticText17;
		wxStaticText* m_staticText18;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText20;
		wxStaticText* m_PlayEndSeqLabel;
		wxStaticText* m_RTPPortValueLabel;
		wxStaticText* m_ReportPortValueLabel;
		wxStaticText* m_staticText24;
		wxStaticText* m_staticText25;
		wxStaticText* m_staticText26;
		wxStaticText* m_staticText27;
		wxStaticText* m_staticText28;

		wxSocketClient*			m_CSock;
		wxSocketServer*			m_Server;

		// GUI Event Callbacks
		void					OnFileOpen( wxCommandEvent& evt );
		void					OnFileClose( wxCommandEvent& evt );
		void					OnFileExit( wxCommandEvent& evt );
		void					OnShowLog( wxCommandEvent& evt );
		void					OnVideoLog( wxCommandEvent& evt );
		void					OnSoundLog( wxCommandEvent& evt );
		void					OnStreamLog( wxCommandEvent& evt );
		void					OnPrimeLog( wxCommandEvent& evt );
		void					OnFlowLog( wxCommandEvent& evt );
		void					OnAboutHelp( wxCommandEvent& evt );
		void					OnAboutInfo( wxCommandEvent& evt );
		void					OnMainFrameClose(wxCloseEvent& WXUNUSED(evt));
                    void                                              OnSwarmReport( wxTimerEvent& evt );
                    void					OnInputReady( wxCommandEvent& evt );
		void					OnDisplayRefresh(wxTimerEvent& evt);

		//Socket Event Callbacks
		void					OnListenEvent( wxSocketEvent& evt );	// Handle accept()s 
		void					OnPeerEvent( wxSocketEvent& evt );		// Handle incoming messages from peers
		void					OnServerEvent( wxSocketEvent& evt );	// Handle incoming messages from the server
	
		//Custom Events Callbacks
		void					OnLogEntry (wxCommandEvent& evt);
		void					OnServerReport (ServerReportEvent& evt );
		void					OnServerProperties (ServerPropertiesEvent& evt );
		void					OnNewOutput (wxCommandEvent& evt);
    void                                                               OnIncMessage( wxCommandEvent& evt );

		// Message Handling
		void					DecodeMessage(PRIME::PRIMEMessage* msg);
                        void					DecodeRequest(wxCommandEvent& evt, PRIME::Request& req);
    void							DecodeReply(wxCommandEvent& evt, PRIME::Reply& repl);
    void							DecodeReport(wxCommandEvent& evt, PRIME::Report& rep);
    void							HandleCSSIReply(wxCommandEvent& evt, PRIME::Reply_CSSIReply& csrep);
    void							HandleP2PSIReply(wxCommandEvent& evt, PRIME::Reply_P2PSIReply& p2prep);
    void							HandleAddDiffParentReport(wxCommandEvent& evt, PRIME::Report_AddDiffParentReport& diffpa);
    //void							HandleAddDiffChildReport(wxCommandEvent& evt, PRIME::Report_AddSwarmChildReport& p2prep);
    void							HandleAddSwarmParentReport(wxCommandEvent& evt, PRIME::Report_AddSwarmParentReport& swarmpa);
    void							HandleAddSwarmChildReport(wxCommandEvent& evt, PRIME::Report_AddSwarmChildReport& swarmch);
	public:
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("FLOW"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrame();
	
		//void setOutputHandle(OutputHandler* hnd);
		void setRTPstate(int port);
		void setBuffer(ChunkBuffer* buff);
		void log(const wxString& text);
		void ExitCounterInc();
		bool ExitCounterCheck();
		void ExitCounterDec();
                    wxString                                          m_StreamAddress;
                    uint16_t                                          m_StreamPort;
};

///////////////////////////////////////////////////////////////////////////////
/// Class ConnectionPropertiesFrame
///////////////////////////////////////////////////////////////////////////////
class ConnectionPropertiesFrame : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_IpText;
		wxStaticText* m_PortText;
		wxStaticText* m_DBWText;
		wxStaticText* m_UBWText;
		
		wxTextCtrl* m_IpProperty;
		wxTextCtrl* m_PortProperty;
		wxSpinCtrl* m_DBWProperty;
		wxSpinCtrl* m_UBWProperty;
		wxStaticLine* m_LayoutLine;
		wxCheckBox* m_ServerCheck;
		wxCheckBox* m_P2PCheck;

		wxButton* m_OkBtn;
		wxButton* m_CancelBtn;
	
		void OnCSModeChecked( wxCommandEvent& event );
		void OnP2PModeChecked( wxCommandEvent& event );
		void OnOkPressed( wxCommandEvent& event );
		void OnCancelPressed( wxCommandEvent& event );

	public:
		
		ConnectionPropertiesFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Connasaasasasection Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,400 ), long style = wxCAPTION|wxFRAME_NO_TASKBAR|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		~ConnectionPropertiesFrame();

};


#endif //__INTERFACE_HEADER_H__

