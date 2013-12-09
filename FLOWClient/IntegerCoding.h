#ifndef __INTEGER_CODING_HEADER_H__
#define __INTEGER_CODING_HEADER_H__

#include "Chunk.h"

#if defined(_WIN32)
#define FAST_EXEC __inline
#include <winsock2.h>
#else
#include <arpa/inet.h>
#define FAST_EXEC inline
#endif
#include <string.h>

#include <stdint.h>

static FAST_EXEC void int_cpy(uint8_t *p, int v)
{
  uint32 tmp;
  
  tmp = htonl(v);
  memcpy(p, &tmp, 4);
}

static FAST_EXEC void int16_cpy(uint8_t *p, uint16 v)
{
  uint16 tmp;

  tmp = htons(v);
  memcpy(p, &tmp, 2);
}

static FAST_EXEC uint32 int_rcpy(const uint8_t *p)
{
  uint32 tmp;
  
  memcpy(&tmp, p, 4);
  tmp = ntohl(tmp);

  return tmp;
}

static FAST_EXEC uint16 int16_rcpy(const uint8_t *p)
{
  uint16 tmp;

  memcpy(&tmp, p, 2);
  tmp = ntohs(tmp);
  return tmp;
}

#endif	// __INTEGER_CODING_HEADER_H__ 
