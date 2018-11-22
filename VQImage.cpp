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

#include "VQImage.h"
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
static CVQImage			*gpCur;
static CVBitmap			*gpCurSrc;
static void				(*gpCurOnPass)(int);

int WINAPI MainCompress(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow);

// functions to convert image data to vectors and back
template<typename TQuant>
void PixelsToQuant(RGBTRIPLE *pPixels, TQuant& t, int w, int h)
{
int i,j;
	for(j= 0; j < h; j++) {
		for(i= 0; i < w; i++) {
			t.data[i*3+0] = float(pPixels[j*w+i].rgbtRed);
			t.data[i*3+1] = float(pPixels[j*w+i].rgbtGreen);
			t.data[i*3+2] = float(pPixels[j*w+i].rgbtBlue);
		}
	}
}

template<typename TQuant>
void QuantToPixels(const TQuant& t, RGBTRIPLE *pPixels, int w, int h)
{
int i,j;
	for(j= 0; j < h; j++) {
		for(i= 0; i < w; i++) {
			pPixels[j*w+i].rgbtRed   = t.data[i*3 + 0] + 0.5f;
			pPixels[j*w+i].rgbtGreen = t.data[i*3 + 1] + 0.5f;
			pPixels[j*w+i].rgbtBlue  = t.data[i*3 + 2] + 0.5f;
		}
	}
}

CVQImage::CVQImage()
:m_pBits(0), m_pCodebook(0),pCurGlaq(0), m_bFinished(0), m_bReadyToDraw(0)
{
}

CVQImage::~CVQImage()
{
	delete m_pBits;
	delete m_pCodebook;
}

HRESULT CVQImage::Create(
	CVBitmap		*pSrc,
	unsigned char	bThreaded,	
	unsigned long	dwCBSize,
	void (*OnPass)(int))
{
	m_dwQuantSizeX  = 1;
	m_dwQuantSizeY  = 1;
	m_dwSizeofQuant = 1;

	gpCur				= this;	
	gpCurSrc			= pSrc;
	gpCur->m_dwCBSize	= dwCBSize;
	gpCurOnPass			= OnPass;

	if(bThreaded){
		_beginthread((void (__cdecl *)(void *))MainCompress,0,NULL);
	}else{
		CreateQuant1x1x256(gpCurSrc, gpCurOnPass);	//Start With only quantize palettes
	}

	return S_OK;
}

// parametrized on two things: vector implementation (x87/3DNow/SSE)
// and index size (byte/word)

HRESULT CVQImage::CreateQuant1x1x256(const CVBitmap *pSrc, void (*OnPass)(int))
{
	delete m_pBits;
	delete m_pCodebook;

	m_dwWidth   = pSrc->GetWidth();
	m_dwWidthQ  = pSrc->GetWidth()  / m_dwQuantSizeX;
	m_dwHeight  = pSrc->GetHeight();
	m_dwHeightQ = pSrc->GetHeight() / m_dwQuantSizeY;

	unsigned char *pBits;
	m_pBits = pBits = new unsigned char[m_dwWidthQ * m_dwHeightQ];

	m_dwSizeofQuant = sizeof(unsigned char);

	RGBTRIPLE *pCodebook;
	m_pCodebook = pCodebook = new RGBTRIPLE[m_dwQuantSizeX * m_dwQuantSizeY * m_dwCBSize];

	unsigned long x,y;

	// get image data into TQuant array for easier pseudorandom walk
	TQuant_Palette *pQuants = new TQuant_Palette[m_dwWidthQ * m_dwHeightQ];
	TQuant_Palette *pQuant  = pQuants;

	RGBTRIPLE *pData = pSrc->GetData();

	for(y=0; y<m_dwHeightQ; y++) {
		for(x=0; x<m_dwWidthQ; x++) {
			PixelsToQuant(pData, *pQuant++, 1, 1);
			pData+= m_dwQuantSizeX;
		}
	}

	unsigned long dwPass;
	GLAQuant<TQuant_Palette> glaq(m_dwCBSize);

	pCurGlaq= &glaq;

	glaq.Init();

	dwPass = 0;

	unsigned long dwUpdated = 0;
	std::list<unsigned long> dwLastList;
	dwLastList.resize(8, m_dwCBSize);

	m_bReadyToDraw= TRUE;
	do {
		// feed the entire image through the quantizer
		unsigned long dwIdx;
		for(dwIdx= 0;dwIdx < m_dwWidthQ * m_dwHeightQ; dwIdx++) {
			glaq.Feed( pQuants[dwIdx] );
		}

		// let the quantizer know it's over
		glaq.UpdateCB();


		dwLastList.pop_front();
		dwLastList.push_back( glaq.m_dwUpdated );

		// exit conditions
		if( glaq.m_dwUpdated == 0 ) {
			break;
		}

		if( *(std::max_element(dwLastList.begin(), dwLastList.end())) < m_dwCBSize / 100 ) {
			break;
		}

		if( *(std::max_element(dwLastList.begin(), dwLastList.end())) == *(std::min_element(dwLastList.begin(), dwLastList.end())) ) {
			break;
		}
		// read codebook from quantizer
		for(unsigned long dwCode = 0; dwCode < m_dwCBSize; dwCode++) {
			QuantToPixels(glaq[dwCode], &pCodebook[dwCode *  m_dwQuantSizeX * m_dwQuantSizeY], 1, 1);
		}
		// quantize image
		for(x=0; x<m_dwWidthQ*m_dwHeightQ; x++) {
			pBits[x] = glaq.GetIndex(pQuants[x]);
		}
		if(OnPass) OnPass(++dwPass);
	}while( dwPass < 128 );

	glaq.SortCB();
	if(OnPass) OnPass(++dwPass);

	// read codebook from quantizer
	for(unsigned long dwCode = 0; dwCode < m_dwCBSize; dwCode++) {
		QuantToPixels(glaq[dwCode], &pCodebook[dwCode *  m_dwQuantSizeX * m_dwQuantSizeY], 1, 1);
	}

	// quantize image
	for(x=0; x<m_dwWidthQ*m_dwHeightQ; x++) {
		pBits[x] = glaq.GetIndex(pQuants[x]);
	}

	pCurGlaq= 0;
	m_bFinished= 1;
	return S_OK;
}

HRESULT CVQImage::Decompress1x1x256(CVBitmap *& pDest)
{
	pDest = new CVBitmap();
	pDest->Create(m_dwWidthQ * m_dwQuantSizeX, m_dwHeightQ * m_dwQuantSizeY);

	int x,y;

	unsigned char *pBits = (unsigned char *)m_pBits;
	RGBTRIPLE *pDestBits = pDest->GetData();
	
	for(y=0; y<m_dwHeightQ; y++) {
		for(x=0; x<m_dwWidthQ; x++) {
			memcpy(pDestBits, (RGBTRIPLE *)m_pCodebook + (*pBits) * m_dwQuantSizeX * m_dwQuantSizeY, m_dwQuantSizeX * sizeof(RGBTRIPLE));
			pBits++;
			pDestBits += m_dwQuantSizeX;
		}
	}

	return S_OK;
}

// ---- load/save stuff

#pragma pack(push)
#pragma pack(4)

// this header potentially supports "future extensions" of the format

struct TVQImageHdr {
	unsigned long dwMagic;
	unsigned long dwCBSize;
	unsigned long dwSizeofCode;
	unsigned long dwSizeofPixel;
	unsigned long dwQuantSizeX, dwQuantSizeY;
	unsigned long dwWidth, dwHeight;
	unsigned long dwWidthQ, dwHeightQ;

	static unsigned long s_dwMagic;

	TVQImageHdr() {}
	TVQImageHdr(const CVQImage *pBmp) {
		dwMagic = TVQImageHdr::s_dwMagic;
		dwCBSize = pBmp->m_dwCBSize;
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

unsigned long TVQImageHdr::s_dwMagic = 'vqbm';

#pragma pack(pop)

// load/save functions from pathname...
HRESULT SaveVQImage(CVFile *pFile, CVQImage * pBmp)
{
	TVQImageHdr hdr(pBmp);
	HRESULT hr;
	
	assert(pBmp->GetBits());
	assert(pBmp->GetCodebook());
	
	hr = pFile->Write(&hdr, sizeof(hdr));
	if( hr != S_OK ) {
		return hr;
	}
	
	hr = pFile->Write(
		pBmp->GetCodebook(), 
		sizeof(RGBTRIPLE) * pBmp->m_dwQuantSizeX * pBmp->m_dwQuantSizeY * pBmp->m_dwCBSize);
	if( hr != S_OK ) {
		return hr;
	}
	
	hr = pFile->Write(
		pBmp->GetBits(),
		pBmp->m_dwSizeofQuant * pBmp->m_dwHeightQ * pBmp->m_dwWidthQ );
	if( hr != S_OK ) {
		return hr;
	}
	
	return S_OK;
}

HRESULT LoadVQImage(CVFile *pFile, CVQImage *& pBmp)
{
	TVQImageHdr hdr;
	HRESULT hr;
	
	hr = pFile->Read(&hdr, sizeof(hdr));
	if( hr != S_OK ) {
		return hr;
	}
	if( hdr.dwMagic != TVQImageHdr::s_dwMagic ) {
		return S_FALSE;
	}
	pBmp = new CVQImage;
	
	pBmp->m_dwCBSize  = hdr.dwCBSize;
	pBmp->m_dwHeight  = hdr.dwHeight;
	pBmp->m_dwHeightQ = hdr.dwHeightQ;
	pBmp->m_dwWidth  = hdr.dwWidth;
	pBmp->m_dwWidthQ = hdr.dwWidthQ;
	pBmp->m_dwQuantSizeX = hdr.dwQuantSizeX;
	pBmp->m_dwQuantSizeY = hdr.dwQuantSizeY;
	pBmp->m_dwSizeofQuant = hdr.dwSizeofCode;
	
	pBmp->m_pCodebook = new RGBTRIPLE [ pBmp->m_dwQuantSizeX * pBmp->m_dwQuantSizeY * pBmp->m_dwCBSize ];
	hr = pFile->Read(
		pBmp->m_pCodebook,
		sizeof(RGBTRIPLE) * pBmp->m_dwQuantSizeX * pBmp->m_dwQuantSizeY * pBmp->m_dwCBSize);
	if( hr != S_OK ) {
		return hr;
	}
	
	pBmp->m_pBits = new BYTE[pBmp->m_dwWidthQ * pBmp->m_dwHeightQ * pBmp->m_dwSizeofQuant];
	hr = pFile->Read(
		pBmp->m_pBits,
		pBmp->m_dwSizeofQuant * pBmp->m_dwHeightQ * pBmp->m_dwWidthQ );
	if( hr != S_OK ) {
		return hr;
	}
	
	return S_OK;
}


HRESULT LoadVQImage(char *pchFilename, CVQImage *& pBmp)
{
	std::auto_ptr<CVFile> pFile(FileOpen(pchFilename, CVFile::omRead));
	if( pFile.get() == 0 ) {
		return S_FALSE;
	}

	LoadVQImage(pFile.get(), pBmp);
	return S_OK;
}

HRESULT SaveVQImage(char *pchFilename, CVQImage * pBmp)
{
	std::auto_ptr<CVFile> pFile(FileOpen(pchFilename, CVFile::omWrite));
	if( pFile.get() == 0 ) {
		return S_FALSE;
	}

	SaveVQImage(pFile.get(), pBmp);
	return S_OK;
}

// ...and to/from CVFile*

int WINAPI 
MainCompress(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) 
{
	gpCur->CreateQuant1x1x256(gpCurSrc, gpCurOnPass);	//Start With only quantize palettes

	return 1;
}
