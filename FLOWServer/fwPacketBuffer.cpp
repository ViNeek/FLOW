#include "fwPacketBuffer.h"
#include "fwPacketQueue.h"

PacketBuffer::PacketBuffer(){
    m_DiffPacketQueue = new PacketQueue;
    m_PlayoutPacketQueue = new PacketQueue;
    m_SwarmPacketQueue = new PacketQueue;
}

PacketBuffer::~PacketBuffer() {
    /*
    if(m_DiffPacketQueue) delete m_DiffPacketQueue;
    if(m_PlayoutPacketQueue) delete m_PlayoutPacketQueue;
    if(m_SwarmPacketQueue) delete m_SwarmPacketQueue;
    */
}
/*
bool decompare(Chunk* c1, Chunk* c2)
{
    return (c1->id < c2->id);
}*/

bool PacketBuffer::PushPacket(AVPacket* pkt) {
//m_DiffPacketQueue->put(pkt);
    return true;
}

void PacketBuffer::lock() {
    m_BufferGuard.Lock();
}

void PacketBuffer::unlock() {
    m_BufferGuard.Unlock();
}

void PacketBuffer::PopChunk(AVPacket* p) {
    //m_PlayoutPacketQueue->get(p);
}

void PacketBuffer::SwapQueues() {
    PacketQueue* tmp;
    
    tmp = m_PlayoutPacketQueue;
    m_PlayoutPacketQueue = m_SwarmPacketQueue;
    m_SwarmPacketQueue = m_DiffPacketQueue;
    m_DiffPacketQueue = tmp;

    //m_DiffPacketQueue->
}