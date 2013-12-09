#ifndef __PRIME_STATE_HEADER_H__
#define __PRIME_STATE_HEADER_H__

#include "../fwUtilities.h"

class JobQueue;
class MainFrame;
class PeerState;

class PRIMEState
{
public:
    PRIMEState();
    ~PRIMEState();

    std::vector<PeerState*>                      m_DiffParents;
    std::vector<PeerState*>                      m_SwarmParents;
    std::vector<PeerState*>                      m_SwarmChildren;
    std::vector<PeerState*>                      m_DiffChildren;

    wxMutex			        m_DiffChildrenGuard;
    wxMutex			        m_SwarmParentsGuard;
    wxMutex			        m_SwarmChildrenGuard;

    void addDiffParent(PeerState* p); 
    void addSwarmChild(PeerState* p);
    void addDiffChild(PeerState* p);
    void addSwarmParent(PeerState* p);

    std::vector<PeerState*> getDiffSnapshot();
};

#endif //__PRIME_STATE_HEADER_H__