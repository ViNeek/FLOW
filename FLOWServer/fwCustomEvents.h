#ifndef __CUSTOM_EVENTS_HEADER_H__
#define __CUSTOM_EVENTS_HEADER_H__

#include "../fwUtilities.h"
//#include "PeerState.h"

class PeerState;

/*
class ServerSession {
public:
	//bandwidth statistics
	float					m_ubw;
	float					m_dbw;

	//Streaming Server Location
	wxString				m_SreamIpStr;
	short					m_StreamPort;
};

class ServerReportEvent : public wxEvent
{
public:
	ServerReportEvent( );

	// Override from parent
	wxEvent* Clone() const;

	ServerSession	m_Sess;
};

//Declare Custom Event
DECLARE_LOCAL_EVENT_TYPE( wxEVT_SERVER_REPORT, -1 )

//Declare Function Pointer Macro
typedef void (wxEvtHandler::*ServerReportEventFunction)(ServerReportEvent&);

#define ServerReportEventHandler(func)                                         \
	(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)\
	wxStaticCastEvent(ServerReportEventFunction, &func)

//Declare Table Entry Macro
#define EVT_SERVER_REPORT(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_SERVER_REPORT, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNotifyEventFunction) \
    wxStaticCastEvent( ServerReportEventFunction, & fn ), (wxObject *) NULL ),
*/

class NewRTPSessionEvent : public wxEvent
{
public:
							NewRTPSessionEvent( PeerState *st );

	// Override from parent
	wxEvent*				Clone() const;
 
	wxString				m_RemoteAddr;
	PeerState				*m_PeerState;
};

//Declare Custom Event
DECLARE_LOCAL_EVENT_TYPE( wxEVT_NEW_RTP_SESSION, -1 )

//Declare Function Pointer Macro
typedef void (wxEvtHandler::*NewRTPSessionEventFunction)(NewRTPSessionEvent&);

#define NewRTPSessionEventHandler(func)                                         \
	(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)\
	wxStaticCastEvent(NewRTPSessionEventFunction, &func)

//Declare Table Entry Macro
#define EVT_NEW_RTP_SESSION(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_NEW_RTP_SESSION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNotifyEventFunction) \
    wxStaticCastEvent( NewRTPSessionEventFunction, & fn ), (wxObject *) NULL ),

#endif // __CUSTOM_EVENTS_HEADERR_H__