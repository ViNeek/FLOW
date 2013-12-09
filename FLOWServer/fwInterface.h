///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __INTERFACE_HEADER_H__
#define __INTERFACE_HEADER_H__

#include "../fwUtilities.h"
#include "fwTrackerUtilities.h"

///////////////////////////////////////////////////////////////////////////

//We want every thread to be able to use the Log so we create a custom event
//which we forward to the main thread
DECLARE_LOCAL_EVENT_TYPE(wxEVT_LOG_ENTRY, -1)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_INPUT_READY, -1)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_BUFFER_FULL, -1)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_INC_PRIME_MESSAGE, -1)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_PEER_DEPARTURE, -1)

class PeerState;
class FLOWScreen;
class WorkerPool;
class PacketBuffer;
class NewRTPSessionEvent;
class JobQueue;
class PacketQueue;
class FrameQueue;
class InputHandler;
class OutputHandler;
class PRIMEState;
class TrackerPeerState;
class FlowTree;
class PRIMENode;

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
private:
	
protected:
    //FLOW State
    PacketQueue*			    m_CBuffer;
    JobQueue*			    m_DecoderQueue;
    JobQueue*                               m_SenderQueue;
    JobQueue*			    m_PeerConnQueue;
    JobQueue*                               m_ReportQueue;
    FrameQueue*                             m_FrameQueue;
    bool				    m_FileOpened;
    bool                                    m_IsRendered;
    uint16_t                                m_NextRTPport;
    uint32_t                                m_SessionType;
    wxString                                m_ServerAddress;
    wxDatagramSocket*                       m_InReportSocket;
    wxDatagramSocket*                       m_OutReportSocket;
    wxDatagramSocket*                       m_RTPSendSocket;

    TrackerPeerState*		    m_RootNode;
    FlowTree*			    m_NetworkTree;
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
    wxStaticText* m_PlayStartSeqValueLabel;
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
    wxStaticText* m_PlayEndSeqValueLabel;
    wxPanel* m_BasePanel;

    //Log
    wxTextCtrl*			    m_Log;

    //Timer
    wxTimer*			    m_Timer;
    wxTimer*                                m_DisplayTimer;

    //Peer List
    wxTreeCtrl*									m_PeerList;

    //Screen
    FLOWScreen*                                 m_Screen;
    
    // Menus
    wxStatusBar*								m_StatusBar;
    wxMenuBar*									m_MenuBar;
    wxMenu*										m_FileMenu;
    wxMenu* m_SchedulerMenu;

    //Server
    wxSocketServer*								m_Server;
    wxDatagramSocket*                                                                     m_SenderSock;
public:
    //GUI callbacks
    void										OnTimerTick(wxTimerEvent& evt);
    void										OnDisplayRefresh(wxTimerEvent& evt);
    void										OnFileOpen( wxCommandEvent& evt );
    void										OnInputReady( wxCommandEvent& evt );
    void            OnCSStreamingSelected( wxCommandEvent& event );
    void            OnP2PStreamingSelected( wxCommandEvent& event );
    void            OnPeerDeparture( wxCommandEvent& event );
    void		    OnEraseBackGround(wxEraseEvent& event);
    //Socket callbacks
    void										OnServerEvent(wxSocketEvent& WXUNUSED(evt));
    void										OnSocketEvent(wxSocketEvent& evt);
    void										OnBroadcastEvent(wxSocketEvent& evt);
    void										OnMainFrameClose(wxCloseEvent& WXUNUSED(evt));

    //Custom Events Callbacks
    void										OnLogEntry ( wxCommandEvent& evt );
    void										OnNewRTPSession( NewRTPSessionEvent& evt );
    void										OnFullBuffer( wxCommandEvent& evt );
    void                                                                                            OnIncMessage( wxCommandEvent& evt );

    // Message Handling
    void										DecodeRequest(wxCommandEvent& evt, PRIME::Request& req);
    void										DecodeReply(wxCommandEvent& evt, PRIME::Reply& repl);
    void										DecodeReport(wxCommandEvent& evt, PRIME::Report& rep);
    void										HandleCSSIRequest(wxCommandEvent& evt, PRIME::Request_CSSIRequest& csreq);
    void										HandleP2PSIRequest(wxCommandEvent& evt, PRIME::Request_P2PSIRequest& p2preq);
    //void							HandleDiffParentReport(wxCommandEvent& evt, PRIME::Report_DiffParentReport& diffpa);
    //void							HandleDiffChildReport(wxCommandEvent& evt, PRIME::Reply_P2PSIReply& p2prep);
    //void							HandleSwarmParentReport(wxCommandEvent& evt, PRIME::Reply_CSSIReply& csrep);
    //void							HandleSwarmChildReport(wxCommandEvent& evt, PRIME::Reply_P2PSIReply& p2prep);
		
													MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY,
														const wxString& title = wxEmptyString,
														const wxPoint& pos = wxDefaultPosition,
														const wxSize& size = wxSize( 800,600 ),
														long style = wxDEFAULT_FRAME_STYLE|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
		
													~MainFrame();
	
		void										log(const wxString text);
		FlowTree*									  initializeTree(TrackerPeerState* rootNode);
		std::vector<StateModification>								  bootstrapNewNode(FlowTree* tree, TrackerPeerState* newNode);
		//Input Handle
		InputHandler*								m_InHandle;
                    OutputHandler*                                                                            m_OutHandler;
                    
                    //PRIME State
                    PRIMEState*                              m_PRIMEState;
};

///////////////////////////////////////////////////////////////////////////////
/// Class FLOWTreeFrame
///////////////////////////////////////////////////////////////////////////////

class FLOWTreeDrawPanel : public wxPanel
{
private:
    bool	      m_MouseClicked;
    wxBitmap*   m_TreeBitmap;
    wxMemoryDC  m_TreeDC;
    int	      m_DrawStartX;
    int	      m_DrawStartY;
    int	      m_PrevX;
    int	      m_PrevY;

public:
    FLOWTreeDrawPanel(wxFrame* parent);
 
    void paintNow();
    void OnLeftClickPressed( wxMouseEvent& event );
    void OnLeftClickReleased( wxMouseEvent& event );
    void OnMouseMoved( wxMouseEvent& event );
    void OnDrawTree( wxPaintEvent& event );

    void render(wxDC& dc);
};

class FLOWTreeFrame : public wxFrame 
{
private:
    
protected:

    FLOWTreeDrawPanel*    m_TreeCanvas;
public:

    FLOWTreeFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("FLOW Tree"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_NO_TASKBAR|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
    ~FLOWTreeFrame();
	
};

#endif //__INTERFACE_HEADER_H__