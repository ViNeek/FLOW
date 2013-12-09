#include "CustomEvents.h"

// Define Custom Events
DEFINE_LOCAL_EVENT_TYPE( wxEVT_SERVER_REPORT )
DEFINE_LOCAL_EVENT_TYPE( wxEVT_SERVER_PROPERTIES )

// Server Report Event methods
wxEvent* ServerReportEvent::Clone() const {
	return new ServerReportEvent(*this); 
}

ServerReportEvent::ServerReportEvent( ) {
	this->SetEventType(wxEVT_SERVER_REPORT);
	m_Sess.m_SreamIpStr = wxString(wxT("127.0.0.1"));
	m_Sess.m_StreamPort = 0;
}

// Server Properties Event methods
wxEvent* ServerPropertiesEvent::Clone() const {
	return new ServerPropertiesEvent(*this); 
}

ServerPropertiesEvent::ServerPropertiesEvent( ){
	this->SetEventType(wxEVT_SERVER_PROPERTIES);
	m_Sess.m_SreamIpStr = wxString(wxT("127.0.0.1"));
	m_Sess.m_StreamPort = 0;
}