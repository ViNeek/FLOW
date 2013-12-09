#ifndef __CONNECTION_MONITOR_HEADER_H__
#define __CONNECTION_MONITOR_HEADER_H__

#include "../fwUtilities.h"
#include "Interface.h"

class MainFrame;

class ConnectionMonitor : public wxEvtHandler
{
public:
	ConnectionMonitor(MainFrame *evtHandler, wxSocketBase *sock);
	~ConnectionMonitor();

	//parent frame
	MainFrame*			m_pFrame;

	wxIPV4address		m_peer;

	wxSocketBase*		m_socket;

private:
	void				OnSocketEvent(wxSocketEvent& pEvent);
};

#endif // __CONNECTION_MONITOR_HEADER_H__