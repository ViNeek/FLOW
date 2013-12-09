//
//  fwUtilities.h
//  FLOW
//
//  Created by Nick Vitsas on 9/11/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//
#ifndef __UTILITIES_HEADER_H__
#define __UTILITIES_HEADER_H__

#pragma warning(disable:4251)
#pragma warning(disable:4991)
#pragma warning(disable:4996)

#include <stdlib.h>
#include <iostream>

//wxWidgets related
#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/socket.h>
#include <wx/url.h>
#include <wx/sstream.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/thread.h>
#include <wx/dynarray.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/numdlg.h>
#include <wx/atomic.h>
#include <wx/rawbmp.h>

//FFmpeg related
#include <stdint.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

#ifdef __cplusplus

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif //INT64_C

extern "C" {
#include <libavutil/avutil.h>
#include <libavutil/mathematics.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/time.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#ifdef _MSC_VER
// MSVC isn't C99 compliant...
#ifdef AV_TIME_BASE_Q
#undef AV_TIME_BASE_Q
#endif
#define AV_TIME_BASE_Q  GetAVTimeBaseQ()

__inline AVRational GetAVTimeBaseQ()
{
    AVRational av = {1, AV_TIME_BASE};
    return av;
}

#define snprintf _snprintf

#endif // _MSC_VER
#endif // __cplusplus

//Containers
#include <queue>
#include <deque>
#include <vector>
#include <map>

#ifdef _WIN32
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#endif  // _WIN32

// PRIME protocol Messages
#include "PRIME.pb.h"

//Helper functions and structs
namespace Utilities {
	void SetStdOutToNewConsole();
	void GetBandwidthStatistics(double *ub, double *db);
};

typedef unsigned char BYTE;

typedef struct PRIMEPacket {
    
	PRIMEPacket(PRIME::PRIMEMessage& msg) {
		m_Length = msg.ByteSize();
		m_Raw = (BYTE *)malloc(m_Length + sizeof(int));
		msg.SerializeToArray(m_Raw + sizeof(int), m_Length);
		*((int *)m_Raw) = m_Length;
	}
    
	PRIMEPacket(PRIME::Reply& reply) {
		m_Length = reply.ByteSize();
		m_Raw = (BYTE *)malloc(m_Length + sizeof(int));
		reply.SerializeToArray(m_Raw + sizeof(int), m_Length);
		*((int *)m_Raw) = m_Length;
	}
    
	PRIMEPacket(PRIME::Report& report) {
		m_Length = report.ByteSize();
		m_Raw = (BYTE *)malloc(m_Length + sizeof(int));
		report.SerializeToArray(m_Raw + sizeof(int), m_Length);
		*((int *)m_Raw) = m_Length;
	}
    
	PRIMEPacket(PRIME::Request& req) {
		m_Length = req.ByteSize();
		m_Raw = (BYTE *)malloc(m_Length + sizeof(int));
		req.SerializeToArray(m_Raw + sizeof(int), m_Length);
		*((int *)m_Raw) = m_Length;
	}
    
	~PRIMEPacket() { if ( m_Raw ) free( m_Raw ); }
    
	int		m_Length;
          wxIPV4address       m_Destination;
	BYTE*	          m_Raw;
} PRIMEPacket;

#endif //__UTILITIES_HEADER_H__
