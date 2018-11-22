#ifndef __VQImage_H
#define __VQImage_H

#include <windows.h>
#include "bmp.h"
#include "NeuQuant.h"
#include "GLAQuant.h"
#include "QuantVect.h"

class CVQImage {
public:
	void *m_pBits;
	void *m_pCodebook;
	unsigned long m_bFinished;		// codebook size
	
	unsigned long m_dwCBSize;		// codebook size
	
	unsigned long m_dwWidth;		// in pixels
	unsigned long m_dwHeight;		// in pixels
	unsigned long m_dwWidthQ;		// in quants
	unsigned long m_dwHeightQ;		// in quants
	unsigned long m_dwQuantSizeX;	// quant dimensions
	unsigned long m_dwQuantSizeY;	// quant dimensions
	unsigned long m_dwSizeofQuant;
	GLAQuant<TQuant_Palette> *pCurGlaq;
	BOOL m_bReadyToDraw;
	
	CVQImage();
	~CVQImage();
	
	// this handles the compression
	
	HRESULT Create(CVBitmap *pSrc, unsigned char bThreaded, unsigned long dwCBSize, void (*OnPass)(int));
	HRESULT Decompress1x1x256(CVBitmap *& pDest);
	
	void *GetBits()     const { return m_pBits; }
	void *GetCodebook() const { return m_pCodebook; }
	void *GetQuantAddress(DWORD x, DWORD y);

	HRESULT CreateQuant1x1x256(const CVBitmap *pSrc, void (*OnPass)(int));
};

// load/save

HRESULT LoadVQImage(char *pchFilename, CVQImage *&pBmp);
HRESULT LoadVQImage(CVFile *pFile, CVQImage *& pBmp);
HRESULT SaveVQImage(char *pchFilename, CVQImage *pBmp);
HRESULT SaveVQImage(CVFile *pFile, CVQImage * pBmp);

#endif