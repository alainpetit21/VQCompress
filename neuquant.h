// (c) Haemimont Multimedia, 2000
// code provided for educational/noncommercial purposes only
// questions/comments to assen@haemimont.bg, subject "VQ" please

#pragma once

#pragma inline_depth(255)

#include <algorithm>

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Neuquant-like Quantizer

#ifdef ABS
#	undef ABS
#endif

#ifdef MIN
#	undef MIN
#endif

#ifdef MAX
#	undef MAX
#endif

#ifdef CLAMP
#	undef CLAMP
#endif

#define ABS(x)			__inline_abs(x)
#define MAX(x,y)		__inline_max(x,y)
#define MIN(x,y)		__inline_min(x,y)
#define CLAMP(x, a, b)	__inline_clamp(x, a, b)

template <typename T>
inline T __inline_abs(T x) { 
	return (x >= 0) ? x : -x; 
}

template <typename T>
inline T __inline_clamp(T x, T a, T b) {
	if( x < a ) return a;
	if( x < b ) return x;
	return b;
}

template <typename T>
inline T __inline_max(T a, T b) {
	if( a < b ) return b;
	return a;
}

template <typename T>
inline T __inline_min(T a, T b) {
	if( a < b ) return a;
	return b;
}

inline float FloatRnd()
{
	return ((rand()%16384)/16383.0f);
}

// T is the vector class and must support the following operations:
// - construction from float between 0.0f and 1.0f
// - float Distance(const T& t1, const T& t2)
// - void  Adjust(T& current, const T& goal, float alpha)

#define NQT_FALLOFF_EPS		(1/128.0f)

template<typename _T>
struct NeuQuantT {
	typedef _T T;

	float m_fFO, m_fFOAlpha; 
	unsigned m_nRadius;

	unsigned m_nCBSize;

	T *m_pCodebook;
	float *m_pDist;

//	__int64 m_dwFlops;

	NeuQuantT(unsigned nQuants, int nRadius = 8, float fFO = 1.0f, float fFOAlpha = 0.4f)
		:m_nCBSize(nQuants & (~3)), m_nRadius(nRadius), m_fFO(fFO), m_fFOAlpha(fFOAlpha)/*, m_dwFlops(0)*/
	{
		m_pCodebook = (T*)malloc(m_nCBSize * sizeof(T));
		m_pDist = (float *)malloc(m_nCBSize * sizeof(float));
	}

	~NeuQuantT()
	{
		free(m_pCodebook);	m_pCodebook= 0;
		free(m_pDist);		m_pDist= 0;
	}

	void Init() 
	{
		for(unsigned i=0; i<m_nCBSize; i++) {
			m_pCodebook[i] = T((float)i/(m_nCBSize-1));
		}
	}

	void Init(T* tStartCond) 
	{
		std::copy(tStartCond, tStartCond+m_nQuants, m_pCodebook);
	}

	unsigned GetIndex(const T& a)
	{
		unsigned nIdx;//, nBestIdx;
		float *pDist = m_pDist;
		T *pCode = m_pCodebook;

		for(nIdx=0; nIdx<m_nCBSize; nIdx++) {
			*pDist++ = Distance(a, *pCode++);
		}
//		m_dwFlops += m_nCBSize * (int)(T::DistFlops);
		return std::min_element(m_pDist, m_pDist+m_nCBSize) - m_pDist;
	}

	const T& operator[](unsigned nIdx)
	{
		return m_pCodebook[nIdx];
	}

	void Feed(const T& a) 
	{
		unsigned nIdx, nBestIdx = GetIndex(a);
		unsigned nLeft, nRight;

		nLeft  = MIN( m_nRadius, nBestIdx );
		nRight = MIN( m_nRadius, m_nCBSize - nBestIdx - 1 );

		Adjust(m_pCodebook[nBestIdx], a, m_fFO);
//		m_dwFlops += (int)(T::AdjFlops);

		float fFalloff;
		fFalloff = m_fFO * m_fFOAlpha;
		for(nIdx = 1; nIdx <= nLeft; nIdx++) {
			if( fFalloff < NQT_FALLOFF_EPS ) {
				break;
			}
			Adjust(m_pCodebook[nBestIdx - nIdx], a, fFalloff);
//			m_dwFlops += (int)(T::AdjFlops);
			fFalloff *= m_fFOAlpha;
		}
		fFalloff = m_fFO * m_fFOAlpha;
		for(nIdx = 1; nIdx <= nRight; nIdx++) {
			if( fFalloff < NQT_FALLOFF_EPS ) {
				break;
			}
			Adjust(m_pCodebook[nBestIdx + nIdx], a, fFalloff);
//			m_dwFlops += (int)(T::AdjFlops);
			fFalloff *= m_fFOAlpha;
		}
	}

	void Print()
	{
		for(unsigned nIdx = 0; nIdx < m_nCBSize; nIdx++) {
			::Print(m_pCodebook[nIdx]);
		}
	}

};

