#include "ChunkBuffer.h"

ChunkBuffer::ChunkBuffer(){
	m_Size = 0;
	m_TotalTime = 0;
	m_TimeInterval = 0;
	m_LastTS = 0;
}
ChunkBuffer::~ChunkBuffer() {

}

void ChunkBuffer::lock() {
	m_MutexBuffer.Lock();
}

void ChunkBuffer::unlock() {
	m_MutexBuffer.Unlock();
}

bool decompare(Chunk* c1, Chunk* c2)
{
	return (c1->id < c2->id);
}

int ChunkBuffer::GetSize() {
	return m_Buffer.size();
}

bool ChunkBuffer::PushChunk(Chunk* c) {
	wxMutexLocker lock(m_MutexBuffer);
	if( ( 1000000L - (m_TotalTime + 33400 ) ) > m_TimeInterval){
		Chunk* newChunk = new Chunk;
		*newChunk = *c;
		newChunk->data = new uint8_t[c->size];
		memcpy(newChunk->data, c->data, c->size);
		m_TotalTime += 33400;
		m_LastTS = c->timestamp;
		m_Buffer.push_back(newChunk);
		m_Size++;
		std::sort(m_Buffer.begin(), m_Buffer.end(), decompare);

		return true;
	}

	Chunk* newChunk = new Chunk;
	*newChunk = *c;
	newChunk->data = new uint8_t[c->size];
	memcpy(newChunk->data, c->data, c->size);
	m_TotalTime += 33400;
	m_LastTS = c->timestamp;
	m_Buffer.push_back(newChunk);
	m_Size++;
	std::sort(m_Buffer.begin(), m_Buffer.end(), decompare);

	return false;
	/*
	//if( (m_TotalTime + (c->timestamp - m_LastTS) > 1000000L) ){
	if( (m_TotalTime + 33400) > 1000000L ){
		if(!m_Buffer.empty()) {
			int64_t goneTS = 0;
			if(m_Buffer.size() >= 2) {
				goneTS = m_Buffer.at(1)->timestamp - m_Buffer.at(0)->timestamp;
				m_TotalTime -= goneTS;
			} else {
				m_TotalTime = 0;
			}
			m_Buffer.pop_front();
		}
	}

	Chunk* newChunk = new Chunk;
	*newChunk = *c;
	newChunk->data = new uint8_t[c->size];
	memcpy(newChunk->data, c->data, c->size);
	m_TotalTime += 33400;//c->timestamp - m_LastTS;
	m_LastTS = c->timestamp;
	m_Buffer.push_back(newChunk);
	std::sort(m_Buffer.begin(), m_Buffer.end(), decompare);
	m_Size++;*/
}

void ChunkBuffer::SetTimeInterval(int32_t inter) {
	m_TimeInterval = inter;
}

Chunk* ChunkBuffer::PopChunk() {
	//wxMutexLocker lock(m_MutexBuffer);
	if(!m_Buffer.empty()) {
		Chunk* newChunk = new Chunk;
		Chunk *currentChunk = m_Buffer.at(0);
		int64_t goneTS = 0;

		if(m_Buffer.size() >= 2) {
			goneTS = m_Buffer.at(1)->timestamp - m_Buffer.at(0)->timestamp;
			m_TotalTime -= goneTS;
		} else {
			m_TotalTime = 0;
		}
		*newChunk = *currentChunk;
		newChunk->data = new uint8_t[currentChunk->size];
		memcpy(newChunk->data, currentChunk->data, currentChunk->size);
		m_Buffer.pop_front();

		currentChunk->Clear();
		delete currentChunk;

		return newChunk;
	}

	return NULL;
}