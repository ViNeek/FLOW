#include "fwCustomEvents.h"

// Define Custom Events
DEFINE_LOCAL_EVENT_TYPE( wxEVT_NEW_RTP_SESSION )

// Server Report Event methods
wxEvent* NewRTPSessionEvent::Clone() const {
	return new NewRTPSessionEvent(*this); 
}

NewRTPSessionEvent::NewRTPSessionEvent( PeerState* st ) {
	this->SetEventType(wxEVT_NEW_RTP_SESSION);
	m_PeerState = st;
}
