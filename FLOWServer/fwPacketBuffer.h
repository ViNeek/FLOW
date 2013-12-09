#ifndef __CHUNK_BUFFER_HEADER_H__
#define __CHUNK_BUFFER_HEADER_H__

#include "../fwUtilities.h"

class PacketQueue;

/*
	Generally, no more than Ä seconds should be allowed in the buffer
	We need to tell the decoding thread to wait if this happens
*/

class PacketBuffer {
	PacketQueue* m_DiffPacketQueue;
          PacketQueue* m_PlayoutPacketQueue;
          PacketQueue* m_SwarmPacketQueue;
	wxMutex   m_BufferGuard; // protects buffer access
public:
  
	PacketBuffer();
	~PacketBuffer();

	bool PushPacket(AVPacket* pkt);
	void PopChunk(AVPacket* p);
          void SwapQueues();
	int GetSize();
	void lock();
	void unlock();
	void Clear();
};

#endif // __CHUNK_BUFFER_HEADER_H__
