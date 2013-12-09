#ifndef __TrackerPeerState__H__
#define __TrackerPeerState__H__


/* Defining some constants for Visual Studio's Debugger. */
#ifdef _WIN32


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG


#endif  // _WIN32
/* End of Debugger Constants. */



#include <iostream>
#include <string>

/* This class contains network information about a peer. */
class TrackerPeerState {
public:
	/* To be used in main project. */

	//jrtplib::RTPSession*	m_SessionStream;
	//ConnectionMonitor*	m_SessionMonitor;
	//wxIPV4address			m_Addr;
	//int16_t				m_StreamPort;
	//int16_t				m_TCPPort;


    std::string		m_Addr, m_PeerID;					// IPv4 Address, Hash of IP concatenated with RepPort
    short			m_StreamPort, m_TCPPort, m_RepPort;	// RTPPort,TCPPort,UDPPort for reports
    double			m_UBW, m_DBW;						// UploadBandWidth , DownloadBandWidth
    

    TrackerPeerState(std::string ip, std::string id, short up, short tp, short rp, double ubw, double dbw) {
        m_Addr		 = ip;
		m_PeerID	 = id;
        m_TCPPort	 = tp;
        m_StreamPort = up;
		m_RepPort	 = rp;
        m_UBW		 = ubw;
        m_DBW		 = dbw;
    }



	bool equals(TrackerPeerState* node){
		if(this->m_Addr		  != node->m_Addr	 ||
		   this->m_PeerID     != node->m_PeerID  ||
		   this->m_TCPPort	  != node->m_TCPPort ||
		   this->m_RepPort	  != node->m_RepPort ||
		   this->m_StreamPort != node->m_StreamPort)

			return false;


		return true;
	}

	std::string getIPAddress() { return m_Addr; }

	std::string getPeerID() { return m_PeerID; }

	short getTCPPort() { return m_TCPPort; }

	short getStreamPort() { return m_StreamPort; }

	short getReportPort() { return m_RepPort; }

};

#endif /* defined(__TrackerPeerState__H__) */
