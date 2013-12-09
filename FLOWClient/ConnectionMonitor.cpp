#include "ConnectionMonitor.h"

ConnectionMonitor::ConnectionMonitor(MainFrame *evtHandler, wxSocketBase* sock) {
	m_socket = sock;
	m_pFrame = evtHandler;

	m_socket->SetNotify(wxSOCKET_LOST_FLAG|wxSOCKET_INPUT_FLAG|wxSOCKET_OUTPUT_FLAG);
	m_socket->Notify(true);
	m_socket->SetEventHandler(*this);
	m_socket->SetFlags(wxSOCKET_WAITALL);

	m_socket->GetPeer(m_peer);
}

ConnectionMonitor::~ConnectionMonitor() {
	m_socket->Destroy();
}

void ConnectionMonitor::OnSocketEvent(wxSocketEvent& pEvent) {
	switch(pEvent.GetSocketEvent())
	{
	case wxSOCKET_INPUT:
		//Read
		break;
	case wxSOCKET_OUTPUT:
		//Write
		break;
	case wxSOCKET_CONNECTION:
		//Connection Established
		break;
	case wxSOCKET_LOST:
		//Connection Lost
		break;
	}
}