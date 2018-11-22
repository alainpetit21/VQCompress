#pragma once

#include <algorithm>

// Generalized Lloyd Algorithm Quantizer

// T is the vector class and must support the following operations:
// - construction from float between 0.0f and 1.0f
// - float Distance(const T& t1, const T& t2)
// - construction from int

#define VECT_COMPARE_EPS 0.3f

template<typename _T>
struct GLAQuant {
	typedef _T T;

	T *m_pCodebook; // this will store the codebook
	T *m_pCentroid; // this will store the centroids during the codebook update								  
	T *m_pDiamVect; // this keeps the most distant vector in each cell
	float *m_pDiameter; // this will store the diameters
	DWORD *m_pHits; // this will keep the number of vectors in each cell										 

	DWORD m_dwCBSize; // codebook size
	float *m_pDist;   // temp for finding the closest codebook vector

	DWORD m_dwUsed, m_dwUpdated;

	GLAQuant(DWORD dwCBSize) 
		: m_dwCBSize(dwCBSize)
	{
		// this is not really necessary, but is assumed
		// by the unrolled loop in GetIndex()
//		assert( (dwCBSize & 0x3) == 0 );

		// ALLOC macro is used instead of new
		// because of alignment requirements for SSE/3DNow

		m_pCodebook = (T*)malloc(m_dwCBSize * sizeof(T));
		m_pCentroid = (T*)malloc(m_dwCBSize * sizeof(T));
		m_pDiamVect = (T*)malloc(m_dwCBSize * sizeof(T));

		m_pHits     = (DWORD *)malloc(m_dwCBSize * sizeof(int));
		m_pDist     = (float *)malloc(m_dwCBSize * sizeof(float));
		m_pDiameter = (float *)malloc(m_dwCBSize * sizeof(float));

		std::fill_n(m_pCentroid, m_dwCBSize, T(0));
		std::fill_n(m_pDiamVect, m_dwCBSize, T(0));

		std::fill_n(m_pHits,   m_dwCBSize, 0);
		std::fill_n(m_pDist,     m_dwCBSize, 0);
		std::fill_n(m_pDiameter, m_dwCBSize, 0);

		m_dwUsed = m_dwUpdated = 0;
	}

	~GLAQuant(){
		free(m_pCodebook); m_pCodebook= 0;
		free(m_pCentroid); m_pCentroid= 0;
		free(m_pDiamVect); m_pDiamVect= 0;

		free(m_pHits);		m_pHits= 0;
		free(m_pDist);		m_pDist= 0;
		free(m_pDiameter);	m_pDiameter= 0;
	}

	void Init() 
	{
		// fill with all-"grey" vectors
		for(DWORD dw=0; dw<m_dwCBSize; dw++) {
			m_pCodebook[dw] = T((float)dw/(m_dwCBSize-1));
		}
	}

	// find the codebook index of the vector closest to a; returns the distance in fDist
	inline DWORD GetIndex(const T& a, float& fDist)
	{
		DWORD dwIdx;
		float *pDist = m_pDist;
		T *pCode = m_pCodebook;

		// loop unrolled for squeezing a bit of performance
		for(dwIdx=0; dwIdx<m_dwCBSize; dwIdx++) {
			*pDist++ = Distance(a, *pCode++);
		}

		dwIdx = std::min_element(m_pDist, m_pDist+m_dwCBSize) - m_pDist;
		fDist = m_pDist[dwIdx];
		return dwIdx;
	}

	// for when we don't need the distance
	inline DWORD GetIndex(const T& a) 
	{
		float fDummy;
		return GetIndex(a, fDummy);
	}

	// feed a vector through the quantization algorithm;
	// on each iteration this must be called for each vector in the input set
	void Feed(const T& a) 
	{
		float fDist;
		DWORD dwIdx;
		
		dwIdx = GetIndex(a, fDist);

		if( fDist > m_pDiameter[dwIdx] ) {
			m_pDiameter[dwIdx] = fDist;
			m_pDiamVect[dwIdx] = a;
		}

		m_pCentroid[dwIdx] += a;
		m_pHits[dwIdx] ++;
	}

	// this is called at the end of each iteration
	void UpdateCB() {
		DWORD dwIdx;
		m_dwUsed = m_dwUpdated = 0;//How many used

		// calculate centroids
		for(dwIdx=0; dwIdx<m_dwCBSize; dwIdx++) {
			if( m_pHits[dwIdx] > 0 ) {
				m_dwUsed++;
				m_pCentroid[dwIdx] *= 1.0f/float(m_pHits[dwIdx]);

				if( Distance(m_pCodebook[dwIdx], m_pCentroid[dwIdx]) > VECT_COMPARE_EPS ) {
					m_pCodebook[dwIdx] = m_pCentroid[dwIdx];
					m_dwUpdated ++;
				}
			}
		}

		// for each empty cell, split the greatest diameter cell
		for(dwIdx=0; dwIdx<m_dwCBSize; dwIdx++) {
			if( m_pHits[dwIdx] == 0 ) {
				// empty cell, find something to replace it with
				DWORD dwMaxDiam;
				dwMaxDiam = std::max_element(m_pDiameter, m_pDiameter+m_dwCBSize) - m_pDiameter;
				// the furthest vector of the split cell is used as code vector for the new cell
				m_pCodebook[dwIdx] = m_pDiamVect[dwMaxDiam];
				m_pDiameter[dwMaxDiam] = 0;
			}
		}

		std::fill_n(m_pCentroid, m_dwCBSize, T(0));
		std::fill_n(m_pDiamVect, m_dwCBSize, T(0));

		std::fill_n(m_pHits,     m_dwCBSize, 0);
		std::fill_n(m_pDist,     m_dwCBSize, 0);
		std::fill_n(m_pDiameter, m_dwCBSize, 0);
	}
	void SortCB()
	{
		int bSorted;

		do{
			// Assume the list is sorted
			bSorted= TRUE;
			for(int dwIdx=0; dwIdx < m_dwCBSize; dwIdx++) {

				// Compare this element with the one after it.
				if(m_pCodebook[dwIdx+1] > m_pCodebook[dwIdx])
				{
					T swap	= m_pCodebook[dwIdx];

					m_pCodebook[dwIdx]		= m_pCodebook[dwIdx+1];
					m_pCodebook[dwIdx+1]	= swap;
					bSorted= FALSE;
					break;
				}
			}
		} while (!bSorted);
	}
	const T& operator[](unsigned nIdx)
	{
		return m_pCodebook[nIdx];
	}
};

