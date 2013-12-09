#include "fwPeerState.h"
#include "fwJobQueue.h"
#include "fwInterface.h"

PeerState::PeerState() {
    m_Queue = NULL;
    m_Synced = true;
}

PeerState::PeerState(JobQueue* q, wxSocketBase* sock, wxIPV4address addr) {
    m_Queue = q;
    m_Addr = addr;
    m_Socket = sock;
    m_TCPPort = 0;
    m_Synced = false;
}

PeerState::~PeerState() {
	//Send RTPBye and clear session

	//Delete Session
}

static int nextRTPPort = 30000;

bool PeerState::equals(PeerState* node){
		if(this->m_Addr.IPAddress() != node->m_Addr.IPAddress() ||
			this->m_TCPPort != node->m_TCPPort ||
			this->m_StreamPort != node->m_StreamPort)

			return false;


		return true;
	}

std::string	PeerState::getIPAddress(){
	return "adsad";
}

short PeerState::getTCPPort(){
	return	m_TCPPort;
}

short PeerState::getStreamPort(){
	return m_StreamPort;
}