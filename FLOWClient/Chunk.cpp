#include "Chunk.h"

void* Chunk::Serialize() {
	uint8_t* rawData = new uint8_t[CHUNK_HEADER_SIZE + size];
	
	memcpy(rawData,&id,sizeof(int32_t));
	memcpy(rawData+sizeof(int32_t),&timestamp,sizeof(int64_t));
	memcpy(rawData+sizeof(int32_t)+sizeof(int64_t),&size,sizeof(int32_t));
	memcpy(rawData+CHUNK_HEADER_SIZE,data,size);

	return rawData;
}

void Chunk::Deserialize(uint8_t* rawData) {
	memcpy(&id,rawData,sizeof(int32_t));
	memcpy(&timestamp,rawData+sizeof(int32_t),sizeof(int64_t));
	memcpy(&size,rawData+sizeof(int32_t)+sizeof(int64_t),sizeof(int32_t));
	data = new uint8_t[size];
	memcpy(data,rawData+CHUNK_HEADER_SIZE,size);
}

void Chunk::Clear() {
	if ( data ) delete [] data;
}