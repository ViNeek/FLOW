#ifndef __STATE_HEADER_H__
#define __STATE_HEADER_H__

#include "../fwUtilities.h"
#include "ConnectionMonitor.h"

class ConnectionMonitor;

class DiffusionParent
{
public:
    DiffusionParent();

	wxSocketClient*			m_Socket;
	wxIPV4address			m_Addr;
};

class DiffusionChild
{
public:
    DiffusionChild();

	ConnectionMonitor*		m_Monitor;
	jrtplib::RTPSession*	m_Sess;
	int16_t					m_StreamPort;
};

class SwarmingParent
{
public:
    SwarmingParent();

	wxSocketClient*			m_Socket;
	wxIPV4address			m_Addr;
};
};

class SwarmingChild
{
public:
    SwarmingChild();

	ConnectionMonitor*		m_Monitor;
	jrtplib::RTPSession*	m_Sess;
	int16_t					m_StreamPort;
};

#endif //__STATE_HEADER_H__