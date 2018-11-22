#ifndef __CVQImage8x8x16x15_H
#define __CVQImage8x8x16x15_H

#include <windows.h>
#include "bmp.h"
#include "NeuQuant.h"
#include "GLAQuant.h"
#include "QuantVect.h"
#include "VQImage.h"

class CVQImage8x8x16x15{
public:
	void *m_pBits;
	void *m_pCodebook;
	
	unsigned long m_dwBlocW;		// in pixels
	unsigned long m_dwBlocH;		// in pixels
	
	unsigned long m_dwCBSize1;		// codebook size
	unsigned long m_dwCBSize2;		// codebook size
	unsigned long m_dwWidth;		// in pixels
	unsigned long m_dwHeight;		// in pixels
	unsigned long m_dwWidthQ;		// in quants
	unsigned long m_dwHeightQ;		// in quants
	unsigned long m_dwQuantSizeX;	// quant dimensions
	unsigned long m_dwQuantSizeY;	// quant dimensions
	unsigned long m_dwSizeofQuant;
	GLAQuant<TQuant_Screen> *pCurGlaq;
	BOOL m_bReadyToDraw;

	CVBitmap	*m_allChopedSrc;
	CVQImage	*m_allChopedRes;
	
	CVQImage8x8x16x15();
	~CVQImage8x8x16x15();
	
	// this handles the compression
	
	HRESULT Create(CVBitmap *pSrc, unsigned char bThreaded, unsigned long dwCBSize1,unsigned long dwCBSize2,unsigned long dwBlockW,unsigned long dwBlockH,void (*OnPass)(int));
		
	HRESULT Decompress8x8x16x15(CVBitmap *& pDest);
	
	void *GetBits()     const { return m_pBits; }
	void *GetCodebook() const { return m_pCodebook; }
	void *GetQuantAddress(DWORD x, DWORD y);

	HRESULT CreateQuant8x8x16x15(const CVBitmap *pSrc, void (*OnPass)(int));
};

// load/save

HRESULT LoadCVQImage8x8x16x15(const char *pchFilename, CVBitmap *&pBmp);
HRESULT LoadCVQImage8x8x16x15(CVFile *pFile, CVBitmap *& pBmp);
HRESULT SaveCVQImage8x8x16x15(const char *pchFilename, const CVBitmap *pBmp);
HRESULT SaveCVQImage8x8x16x15(CVFile *pFile, const CVBitmap * pBmp);

#endif // __CVQImage8x8x16x15_H