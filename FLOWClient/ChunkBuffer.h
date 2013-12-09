#ifndef __CHUNK_BUFFER_HEADER_H__
#define __CHUNK_BUFFER_HEADER_H__

#include "../fwUtilities.h"
#include "Chunk.h"
#include <queue>

/*
	Generally, no more than Ä seconds should be allowed in the buffer
	We need to tell the decoding thread to wait if this happens
*/

class ChunkBuffer {
	std::deque<Chunk *> m_Buffer;
	int32_t				m_Size;
	int64_t				m_TotalTime;
	int32_t				m_TimeInterval;
	int64_t				m_LastTS;
	wxMutex				m_MutexBuffer; // protects buffer access
public:
  
						ChunkBuffer();
						~ChunkBuffer();

	bool				PushChunk(Chunk* c);
	void				SetTimeInterval(int32_t inter);
	Chunk*				PopChunk();
	int					GetSize();
	void				lock();
	void				unlock();
};

#endif // __CHUNK_BUFFER_HEADER_H__
