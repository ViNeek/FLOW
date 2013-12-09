#ifndef __PEER_STATE_HEADER_H__
#define __PEER_STATE_HEADER_H__

#include "../fwUtilities.h"

class JobQueue;
class MainFrame;
class PeerState;

typedef struct PeerMessage {
    PeerState* m_Peer;
    PRIME::PRIMEMessage* m_Msg;
} PeerMessage;

class PeerState
{
public:
	JobQueue*				m_Queue;
	wxIPV4address							m_Addr;
          wxSocketBase*                           m_Socket;
          wxString                                                                                  m_PeerName;
	uint16_t									m_StreamPort;
	uint16_t									m_TCPPort;
          uint16_t									m_RepPort;
          bool									m_Synced;
          double									m_DBW;
	double									m_UBW;

          wxTreeItemId                                                                              m_NodeId;
        
          PeerState();
        PeerState(JobQueue* q, wxSocketBase* sock, wxIPV4address addr);
        ~PeerState();

	bool equals(PeerState* node);
	std::string								getIPAddress();
	short									getTCPPort();
	short									getStreamPort();
};

#endif //__PEER_STATE_HEADER_H__