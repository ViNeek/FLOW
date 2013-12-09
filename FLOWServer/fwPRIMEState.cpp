#include "fwPRIMEState.h"

#include "fwPeerState.h"

PRIMEState::PRIMEState() {

}

PRIMEState::~PRIMEState() {

}

void PRIMEState::addDiffChild(PeerState* p) {
    wxMutexLocker lock(m_DiffGuard);
    m_DiffPeers.push_back(p);
}

void PRIMEState::addPeer(wxString key, PeerState* p) {
    wxMutexLocker lock(m_PeerGuard);
    m_ConnectedPeers.insert(std::pair<wxString,PeerState*>(key, p));
}

std::vector<wxIPV4address> PRIMEState::getDiffSnapshot() {
    wxMutexLocker lock(m_DiffGuard);
    std::vector<wxIPV4address> peers;
    for(int i = 0; i < m_DiffPeers.size(); i++ ) {
        peers.push_back(m_DiffPeers[i]->m_Addr);
    }
    return peers;
}