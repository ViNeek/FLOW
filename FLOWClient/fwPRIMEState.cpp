#include "fwPRIMEState.h"

#include "fwPeerState.h"

PRIMEState::PRIMEState() {

}

PRIMEState::~PRIMEState() {

}

void PRIMEState::addDiffParent(PeerState* p) {
    //wxMutexLocker lock(m_DiffGuard);
    m_DiffParents.push_back(p);
}

void PRIMEState::addSwarmChild(PeerState* p) {
    wxMutexLocker lock(m_SwarmChildrenGuard);
    m_SwarmChildren.push_back(p);
}

void PRIMEState::addDiffChild(PeerState* p) {
    wxMutexLocker lock(m_DiffChildrenGuard);
    m_DiffChildren.push_back(p);
}

void PRIMEState::addSwarmParent(PeerState* p) {
    wxMutexLocker lock(m_SwarmParentsGuard);
    m_SwarmParents.push_back(p);
}

std::vector<PeerState*> PRIMEState::getDiffSnapshot() {
    //wxMutexLocker lock(m_DiffGuard);
    return m_DiffParents;
}