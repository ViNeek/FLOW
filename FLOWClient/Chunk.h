#ifndef __CHUNK_HEADER_H__
#define __CHUNK_HEADER_H__

#define CHUNK_HEADER_SIZE (sizeof(int32_t) + sizeof(int64_t) + sizeof(int32_t) )

#include <stdint.h>
/** 
 * @file chunk.h
 *
 * @brief Chunk structure.
 *
 * Describes the structure of the chunk.
 *
 */

/**
 * Structure describing a chunk. This is part of the 
 * public API
 */
class Chunk {
public:
   /**
    * Chunk ID. Should be unique in a stream, and is generally
    * an integer used as a sequence number.
    */
   int32_t			id;
   /**
    * Size of the data, in byte.
    */
   int32_t			size;
   /**
    * Pointer to a buffer containing the chunk payload.
    */
   uint8_t*		data;
   /**
    * Chunk timestamp (can be the timestamp of the first frame in
    * the chunk, the chunk generation time, or something else).
    */
   int64_t		timestamp;
   /**
    * Pointer to an opaque structure containing some system-dependent
    * (or scheduler-dependent) data: for example, it can contain some
    * information from the video header, the ``chunk importance'' (in case
    * of media awareness and/or if layered encoding is used), the scheduling
    * deadline of the chunk, and so on...
    */
   void*		attributes;
   /**
    * Size of the attributes, in byte.
    */
   int32_t		attributes_size;

   int32_t		media_type; // 1 is Video, 2 is Sound

   //Delete it afterwards
   void Clear();
   void* Serialize();
   void Deserialize(uint8_t* data);
};

#endif // __CHUNK_HEADER_H__
