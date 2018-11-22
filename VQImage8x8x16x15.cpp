#pragma inline_depth(255)
#pragma warning(disable: 4244 4018)

#include <algorithm>
#include <functional>
#include <utility>
#include <list>
#include <memory>
#include <process.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "VQImage8x8x16x15.h"
#include "GLAQuant.h"
#include "QuantVect.h"

// 128 prime numbers used for pseudorandom traversal of image, needed for Neuquant

static DWORD g_dwPrimes[128] = {
	1987, 1993, 1997, 1999, 2003, 2011, 2017, 2027,
	2089, 2099, 2111, 2113, 2129, 2131, 2137, 2141,
	2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281,
	2347, 2351, 2357, 2371, 2377, 2381, 2383, 2389,
	2459, 2467, 2473, 2477, 2503, 2521, 2531, 2539,
	2617, 2621, 2633, 2647, 2657, 2659, 2663, 2671,
	2713, 2719, 2729, 2731, 2741, 2749, 2753, 2767,
	2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897,
	2969, 2971, 2999, 3001, 3011, 3019, 3023, 3037,
	3119, 3121, 3137, 3163, 3167, 3169, 3181, 3187,
	3257, 3259, 3271, 3299, 3301, 3307, 3313, 3319,
	3373, 3389, 3391, 3407, 3413, 3433, 3449, 3457,
	3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559,
	3631, 3637, 3643, 3659, 3671, 3673, 3677, 3691,
	3767, 3769, 3779, 3793, 3797, 3803, 3821, 3823,
	3907, 3911, 3917, 3919, 3923, 3929, 3931, 3943
};
static CVQImage8x8x16x15	*gpCur;
static CVBitmap				*gpCurSrc;
static void					(*gpCurOnPass)(int);

int WINAPI MainCompress8x8x16x15(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow);

CVQImage8x8x16x15::CVQImage8x8x16x15()
:m_pBits(0), m_pCodebook(0),pCurGlaq(0), m_bReadyToDraw(0)
{
}

CVQImage8x8x16x15::~CVQImage8x8x16x15()
{
	delete m_pBits;
	delete m_pCodebook;
}

HRESULT CVQImage8x8x16x15::Create(
	CVBitmap *pSrc,
	unsigned char bThreaded,
	unsigned long dwCBSize1,
	unsigned long dwCBSize2,
	unsigned long dwBlockW,
	unsigned long dwBlockH,
	void (*OnPass)(int))
{
	m_dwQuantSizeX  = 1;
	m_dwQuantSizeY  = 1;
	m_dwSizeofQuant = 1;

	gpCur				= this;
	gpCur->m_dwCBSize1	= dwCBSize1;
	gpCur->m_dwCBSize2	= dwCBSize2;
	gpCurSrc			= pSrc;
	gpCurOnPass	= OnPass;
	m_dwBlocW	= dwBlockW;
	m_dwBlocH	= dwBlockH;
		
	m_allChopedSrc= new CVBitmap[((pSrc->m_dwWidth+(m_dwBlocW-1)) / m_dwBlocW)*((pSrc->m_dwHeight+(m_dwBlocH-1)) / m_dwBlocH)];
	m_allChopedRes= new CVQImage[((pSrc->m_dwWidth+(m_dwBlocW-1)) / m_dwBlocW)*((pSrc->m_dwHeight+(m_dwBlocH-1)) / m_dwBlocH)];

	if(bThreaded){
		_beginthread((void (__cdecl *)(void *))MainCompress8x8x16x15,0,NULL);
	}else{
		CreateQuant8x8x16x15(gpCurSrc, gpCurOnPass);	//Start With only quantize palettes
	}

//	MainCompress8x8x16x15(0, 0, 0, 0);
	return S_OK;
}

HRESULT CVQImage8x8x16x15::CreateQuant8x8x16x15(const CVBitmap *pSrc, void (*OnPass)(int))
{
int i,j, k, l;

	m_dwWidth   = pSrc->GetWidth();
	m_dwWidthQ  = pSrc->GetWidth()  / m_dwQuantSizeX;
	m_dwHeight  = pSrc->GetHeight();
	m_dwHeightQ = pSrc->GetHeight() / m_dwQuantSizeY;

	m_bReadyToDraw= TRUE;

	for(j=0; j < (m_dwHeight/m_dwBlocW); ++j){
		for(i=0; i < (m_dwWidth/m_dwBlocW); ++i){
			m_allChopedSrc[j*(m_dwWidth/m_dwBlocW)+i].Create(m_dwBlocW, m_dwBlocW);
		}
	}


	for(j=0; j < (m_dwHeight/m_dwBlocH); ++j){
		for(i=0; i < (m_dwWidth/m_dwBlocW); ++i){
			for(k=0; k < m_dwBlocH; ++k){
				for(l=0; l < m_dwBlocW; ++l){
					m_allChopedSrc[j*(m_dwWidth/m_dwBlocW)+i].m_pData[k*m_dwBlocW+l]= pSrc->m_pData[((j*m_dwBlocH)+k)*pSrc->m_dwWidth+((i*m_dwBlocW)+l)];
				}
			}
		}
	}
	
	for(j=0; j < (m_dwHeight/m_dwBlocH); ++j){
		for(i=0; i < (m_dwWidth/m_dwBlocW); ++i){
			m_allChopedRes[j*(m_dwWidth/m_dwBlocW)+i].Create(&m_allChopedSrc[j*(m_dwWidth/m_dwBlocW)+i], 0, m_dwCBSize1, OnPass);
			while(!m_allChopedRes[j*(m_dwWidth/m_dwBlocW)+i].m_bFinished);
		}
	}
	
	delete m_pCodebook;

	RGBTRIPLE *pCodebook;
	m_pCodebook = pCodebook = new RGBTRIPLE[m_dwCBSize1*m_dwCBSize2];

	unsigned long x,y;

	// get image data into TQuant array for easier pseudorandom walk
	TQuant_Screen	*pQuants= new TQuant_Screen[(m_dwWidth/m_dwBlocW)*(m_dwHeight/m_dwBlocH)];
	TQuant_Screen	*pQuant = pQuants;
	CVQImage		*pImage	= m_allChopedRes;

	for(y=0; y < (m_dwHeight/m_dwBlocH); y++){
		for(x=0; x < (m_dwWidth/m_dwBlocW); x++){
			for(i=0; i < m_dwCBSize1; ++i){
				pQuant->data[i*3+0] = float(((RGBTRIPLE*)pImage->m_pCodebook)[i].rgbtRed);
				pQuant->data[i*3+1] = float(((RGBTRIPLE*)pImage->m_pCodebook)[i].rgbtGreen);
				pQuant->data[i*3+2] = float(((RGBTRIPLE*)pImage->m_pCodebook)[i].rgbtBlue);
			}
			++pQuant;
			++pImage;
		}
	}

	unsigned long dwPass;
	GLAQuant<TQuant_Screen> glaq(m_dwCBSize2);

	pCurGlaq= &glaq;

	glaq.Init();

	dwPass = 0;

	unsigned long dwUpdated = 0;

	do {
		// feed the entire image through the quantizer
		unsigned long dwIdx;
		for(dwIdx= 0;dwIdx < (m_dwWidth/m_dwBlocW)*(m_dwHeight/m_dwBlocH); dwIdx++) {
			glaq.Feed( pQuants[dwIdx] );
		}

		// let the quantizer know it's over
		glaq.UpdateCB();


		// exit conditions
		if( glaq.m_dwUpdated == 0 ) {
			break;
		}
	}while( dwPass < 128 );
/*
	// read codebook from quantizer
	for(unsigned long dwCode = 0; dwCode < dwCBSize; dwCode++) {
		QuantToPixels(glaq[dwCode], &pCodebook[dwCode *  m_dwQuantSizeX * m_dwQuantSizeY], 1, 1);
	}

	// quantize image
	for(x=0; x<m_dwWidthQ*m_dwHeightQ; x++) {
		pBits[x] = glaq.GetIndex(pQuants[x]);
	}
*/
	// read codebook from quantizer
	for(j=0; j < m_dwCBSize2; ++j){
		for(i=0; i < m_dwCBSize1; ++i){
			pCodebook[j*m_dwCBSize1+i].rgbtRed	= glaq[j].data[i*3+0];
			pCodebook[j*m_dwCBSize1+i].rgbtGreen= glaq[j].data[i*3+1];
			pCodebook[j*m_dwCBSize1+i].rgbtBlue	= glaq[j].data[i*3+2];
		}
	}
	pImage	= m_allChopedRes;

	for(y=0; y < (m_dwHeight/m_dwBlocH); y++){
		for(x=0; x < (m_dwWidth/m_dwBlocW); x++){
			TQuant_Screen	tempVec;
			for(i=0; i < m_dwCBSize1; ++i){
				tempVec.data[i*3+0] = float(((RGBTRIPLE*)pImage->m_pCodebook)[i].rgbtRed);
				tempVec.data[i*3+1] = float(((RGBTRIPLE*)pImage->m_pCodebook)[i].rgbtGreen);
				tempVec.data[i*3+2] = float(((RGBTRIPLE*)pImage->m_pCodebook)[i].rgbtBlue);
			}
			long newIdx= glaq.GetIndex(tempVec);

			for(i=0; i < m_dwCBSize1; ++i){
				((RGBTRIPLE*)pImage->m_pCodebook)[i].rgbtRed	=pCodebook[newIdx*m_dwCBSize1+i].rgbtRed;
				((RGBTRIPLE*)pImage->m_pCodebook)[i].rgbtGreen	=pCodebook[newIdx*m_dwCBSize1+i].rgbtGreen;
				((RGBTRIPLE*)pImage->m_pCodebook)[i].rgbtBlue	=pCodebook[newIdx*m_dwCBSize1+i].rgbtBlue;
			}

			++pImage;
		}
	}

	pCurGlaq= 0;
	return S_OK;
}

HRESULT CVQImage8x8x16x15::Decompress8x8x16x15(CVBitmap *& pDest)
{
	pDest = new CVBitmap();
	pDest->Create(m_dwWidthQ * m_dwQuantSizeX, m_dwHeightQ * m_dwQuantSizeY);

	int x,y;

	unsigned char *pBits = (unsigned char *)m_pBits;
	RGBTRIPLE *pDestBits = pDest->GetData();
	
	for(y=0; y<m_dwHeightQ; y++) {
		for(x=0; x<m_dwWidthQ; x++) {
			memcpy(pDestBits, (RGBTRIPLE *)m_pCodebook + (*pBits) * m_dwQuantSizeX * m_dwQuantSizeY, m_dwQuantSizeX * sizeof(RGBTRIPLE));
			memcpy(pDestBits+(pDest->GetWidth()), ((RGBTRIPLE *)m_pCodebook + (*pBits) * m_dwQuantSizeX * m_dwQuantSizeY) + m_dwQuantSizeX, m_dwQuantSizeX * sizeof(RGBTRIPLE));
			pBits++;
			pDestBits += m_dwQuantSizeX;
		}
		pDestBits += pDest->GetWidth();
	}

	return S_OK;
}

// ---- load/save stuff

#pragma pack(push)
#pragma pack(4)

// this header potentially supports "future extensions" of the format

struct TVQImage8x8x16x15Hdr {
	unsigned long dwMagic;
	unsigned long dwCBSize1;
	unsigned long dwCBSize2;
	unsigned long dwSizeofCode;
	unsigned long dwSizeofPixel;
	unsigned long dwQuantSizeX, dwQuantSizeY;
	unsigned long dwWidth, dwHeight;
	unsigned long dwWidthQ, dwHeightQ;

	static unsigned long s_dwMagic;

	TVQImage8x8x16x15Hdr() {}
	TVQImage8x8x16x15Hdr(const CVQImage8x8x16x15 *pBmp) {
		dwMagic = TVQImage8x8x16x15Hdr::s_dwMagic;
		dwCBSize1 = pBmp->m_dwCBSize1;
		dwCBSize2 = pBmp->m_dwCBSize2;
		dwSizeofCode = 1;
		dwSizeofPixel = 1;
		dwWidth      = pBmp->m_dwWidth;
		dwWidthQ     = pBmp->m_dwWidthQ;
		dwHeight     = pBmp->m_dwHeight;
		dwHeightQ    = pBmp->m_dwHeightQ;
		dwQuantSizeX = pBmp->m_dwQuantSizeX;
		dwQuantSizeY = pBmp->m_dwQuantSizeY;
	}
};

unsigned long TVQImage8x8x16x15Hdr::s_dwMagic = 'vqbm';

#pragma pack(pop)

// load/save functions from pathname...
HRESULT SaveVQImage(CVFile *pFile, const CVQImage8x8x16x15 * pBmp)
{
	return S_OK;
}

HRESULT LoadVQImage(CVFile *pFile, CVQImage8x8x16x15 *& pBmp)
{
	return S_OK;
}


HRESULT LoadVQImage(const char *pchFilename, CVQImage8x8x16x15 *& pBmp)
{
	return S_OK;
}

HRESULT SaveVQImage(const char *pchFilename, const CVQImage8x8x16x15 * pBmp)
{
	return S_OK;
}

// ...and to/from CVFile*

int WINAPI 
MainCompress8x8x16x15(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) 
{
	gpCur->CreateQuant8x8x16x15(gpCurSrc, gpCurOnPass);	//Start With only quantize palettes

	return 1;
}
