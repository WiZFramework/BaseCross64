/*
Physics Effects Copyright(C) 2012 Sony Computer Entertainment Inc.
All rights reserved.

Physics Effects is open software; you can redistribute it and/or
modify it under the terms of the BSD License.

Physics Effects is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the BSD License for more details.

A copy of the BSD License is distributed with
Physics Effects under the filename: physics_effects_license.txt
*/

#pragma once
#include "stdafx.h"

#include "pfx_common.h"

//J パフォーマンス測定する場合はPFX_USE_PERFCOUNTERを定義
//J ブックマークを使用する場合はPFX_USE_BOOKMARKを定義

//E Define SCE_PFX_USE_PERFCOUNTER to check performance
//E Define SCE_PFX_USE_BOOKMARK to use bookmark


#define SCE_PFX_MAX_PERF_STR	32
#define SCE_PFX_MAX_PERF_COUNT	20

//#define SCE_PFX_USE_PERFCOUNTER
//#define SCE_PFX_USE_BOOKMARK

namespace sce {
namespace PhysicsEffects {
#ifdef SCE_PFX_USE_PERFCOUNTER

class PfxPerfCounter
{
private:
	int   m_count,m_strCount;
	char  m_str[SCE_PFX_MAX_PERF_COUNT][SCE_PFX_MAX_PERF_STR];
	float m_freq;

	SCE_PFX_PADDING(1,4)
#ifdef _WIN32
	LONGLONG  m_cnt[SCE_PFX_MAX_PERF_COUNT*2];
#endif

	void count(int i)
	{
#ifdef _WIN32
		QueryPerformanceCounter( (LARGE_INTEGER *)&m_cnt[i] );
#endif
	}

public:
	PfxPerfCounter()
	{
#ifdef _WIN32
		LARGE_INTEGER sPerfCountFreq;
		QueryPerformanceFrequency(&sPerfCountFreq);
		m_freq = (float)sPerfCountFreq.QuadPart;
#endif
		resetCount();
	}

	~PfxPerfCounter()
	{
		//printCount();
	}

	void countBegin(const char *name)
	{
		SCE_PFX_ASSERT(m_strCount < SCE_PFX_MAX_PERF_COUNT);
		strncpy(m_str[m_strCount],name,SCE_PFX_MAX_PERF_STR-1);
		m_str[m_strCount][SCE_PFX_MAX_PERF_STR-1] = 0x00;
		m_strCount++;
		count(m_count++);
	}
	
	void countEnd()
	{
		count(m_count++);
	}

	void resetCount()
	{
		m_strCount = 0;
		m_count = 0;
	}

	float getCountTime(int i)
	{
	return (float)(m_cnt[i+1]-m_cnt[i]) / m_freq * 1000.0f;
	}

	void printCount()
	{
		if(m_count%2 != 0) countEnd();
		SCE_PFX_PRINTF("*** PfxPerfCounter results ***\n");
		float total = 0.0f;
		for(int i=0;i+1<m_count;i+=2) {
			total += getCountTime(i);
		}
		for(int i=0;i+1<m_count;i+=2) {
			SCE_PFX_PRINTF(" -- %s %fms(%.2f%%)\n",m_str[i>>1],getCountTime(i),getCountTime(i)/total*100.0f);
		}
		SCE_PFX_PRINTF(" -- Total %fms\n",total);
	}
};

#else /* SCE_PFX_USE_PERFCOUNTER */

class PfxPerfCounter
{
public:
	PfxPerfCounter() {}
	~PfxPerfCounter() {}
	void countBegin(const char *name) {(void) name;}
	void countEnd() {}
	void resetCount() {}
	float getCountTime(int i) {(void)i;return 0.0f;}
	void printCount() {}
};

#endif /* SCE_PFX_USE_PERFCOUNTER */

#define pfxInsertBookmark(bookmark)

#ifdef SCE_PFX_USE_BOOKMARK
	#define SCE_PFX_PUSH_MARKER_HUD(name,col)
	#define SCE_PFX_PUSH_MARKER(name)
	#define SCE_PFX_POP_MARKER()
#else
	#define SCE_PFX_PUSH_MARKER_HUD(name,col)
	#define SCE_PFX_PUSH_MARKER(name)
	#define SCE_PFX_POP_MARKER()
#endif

} //namespace PhysicsEffects
} //namespace sce

