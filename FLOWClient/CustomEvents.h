#ifndef __CUSTOM_EVENTS_HEADER_H__
#define __CUSTOM_EVENTS_HEADER_H__

#include "../fwUtilities.h"

class ServerSession {
public:
	//bandwidth statistics
	float					m_ubw;
	float					m_dbw;

	//Streaming Server Location
	wxString				m_SreamIpStr;
	short					m_StreamPort;

	wxSocketClient*			m_Socket;
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

class ServerPropertiesEvent : public wxEvent
{
public:
	ServerPropertiesEvent( );

	// Override from parent
	wxEvent* Clone() const;
 
	ServerSession	m_Sess;
};

//Declare Custom Event
DECLARE_LOCAL_EVENT_TYPE( wxEVT_SERVER_PROPERTIES, -1 )

//Declare Function Pointer Macro
typedef void (wxEvtHandler::*ServerPropertiesEventFunction)(ServerPropertiesEvent&);

#define ServerPropertiesEventHandler(func)                                         \
	(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)\
	wxStaticCastEvent(ServerPropertiesEventFunction, &func)

//Declare Table Entry Macro
#define EVT_SERVER_PROPERTIES(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_SERVER_PROPERTIES, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNotifyEventFunction) \
    wxStaticCastEvent( ServerPropertiesEventFunction, & fn ), (wxObject *) NULL ),

#endif // __CUSTOM_EVENTS_HEADERR_H__