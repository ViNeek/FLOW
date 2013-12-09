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

    std::vector<PeerState*>                      m_DiffPeers;
    std::map< wxString , PeerState* >	    m_ConnectedPeers;
    wxMutex			    m_PeerGuard;
    wxMutex			    m_DiffGuard;

    void addDiffChild(PeerState* p);
    void addPeer(wxString key, PeerState* p);
    std::vector<wxIPV4address> getDiffSnapshot();
};

#endif //__PRIME_STATE_HEADER_H__