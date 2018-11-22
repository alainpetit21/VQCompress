#pragma once
#include <cassert>
#include <memory>
#include <algorithm>
#include <windows.h>

#include "file.h"

// a minimal 24-bit image class which can read/write Windows .BMP files

using std::auto_ptr;

class CVBitmap {
public:
	DWORD m_dwHeight;
	DWORD m_dwWidth;
	RGBTRIPLE *m_pData;

	friend HRESULT LoadBmp(const char *pchFilename, CVBitmap *& pRet);
	CVBitmap(): m_dwHeight(0), m_dwWidth(0), m_pData(0) {}
	void Create(DWORD dwWidth, DWORD dwHeight) {
		m_dwWidth = dwWidth;
		m_dwHeight = dwHeight;
		delete m_pData;
		m_pData = new RGBTRIPLE[dwWidth * dwHeight];
	}

	~CVBitmap() {
		delete m_pData;
	}

	DWORD      GetWidth () const { return m_dwWidth;  }
	DWORD      GetHeight() const { return m_dwHeight; }
	RGBTRIPLE *GetData  () const { return m_pData;    }

	void Draw(HDC hdc, int x, int y) {
		BYTE bi[1024 + sizeof(BITMAPINFO)];
		BITMAPINFO *bmi = (BITMAPINFO *) bi;

		memset(bmi, 0, sizeof(BITMAPINFOHEADER));
		bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi->bmiHeader.biWidth = GetWidth();
		bmi->bmiHeader.biHeight = -(int)GetHeight();
		bmi->bmiHeader.biPlanes = 1;
		bmi->bmiHeader.biBitCount = 24;
		bmi->bmiHeader.biSizeImage = 0;
		bmi->bmiHeader.biClrUsed = 0;
		bmi->bmiHeader.biClrImportant = 0;
		bmi->bmiHeader.biCompression = BI_RGB;
		SetDIBitsToDevice(hdc, 
			x, y, 
			GetWidth(), GetHeight(), 
			0, 0, 
			0, GetHeight(),
			(void *)m_pData, bmi, DIB_RGB_COLORS); 

	}
};

inline HRESULT LoadBmp(const char *pchFilename, CVBitmap *& pRet) {
	HRESULT hr;
	auto_ptr<CVFile> pFile(FileOpen(pchFilename));

	BITMAPFILEHEADER fh;

	hr = pFile->Read(&fh, sizeof(fh));
	if( hr != S_OK ) {
		return S_FALSE;
	}

	BITMAPINFOHEADER ih;

	hr = pFile->Read(&ih, sizeof(ih));
	if( hr != S_OK ) {
		return S_FALSE;
	}

	if( ih.biBitCount != 24 ) {
		return S_FALSE; // format not supported
	}
	if( ih.biCompression != BI_RGB ) {
		return S_FALSE; // format not supported
	}

	DWORD dwWidth  = ih.biWidth;
	DWORD dwHeight = ih.biHeight > 0 ? ih.biHeight : -ih.biHeight;

	auto_ptr<CVBitmap> pBmp(new CVBitmap);
	pBmp->Create(dwWidth, dwHeight);

	auto_ptr<RGBTRIPLE> pData(new RGBTRIPLE [dwWidth * dwHeight]);
	pFile->Seek(fh.bfOffBits, CVFile::smSet);
	pFile->Read(pData.get(), dwWidth * dwHeight * sizeof(RGBTRIPLE));

	if( ih.biHeight > 0 ) {
		pBmp->m_pData = new RGBTRIPLE[dwWidth * dwHeight];
		RGBTRIPLE *pSrc = pData.get() + (dwHeight-1)*dwWidth, 
				  *pDest = pBmp->m_pData;

		for(DWORD y = 0; y < dwHeight; y++) {
			std::copy(pSrc, pSrc+dwWidth, pDest);
			pSrc  -= dwWidth;
			pDest += dwWidth;
		}
	} else {
		pBmp->m_pData = pData.release();
	}

	pRet = pBmp.release();
	return S_OK; 
}

inline HRESULT SaveBmp(const char *pchFilename, const CVBitmap *pBmp) {
	assert( pBmp->GetData() != 0 );
	HRESULT hr;

	auto_ptr<CVFile> pFile(FileOpen(pchFilename, CVFile::omWrite));		
	if( pFile.get() == 0 ) {
		return S_FALSE;
	}

	BITMAPFILEHEADER fh;
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfType = 'MB';
	fh.bfOffBits = 
					sizeof(BITMAPFILEHEADER) + 
					sizeof(BITMAPINFOHEADER);

	fh.bfSize = 
				sizeof(BITMAPFILEHEADER) + 
				sizeof(BITMAPINFOHEADER) + 
				pBmp->GetWidth() * pBmp->GetHeight() * 3;

	BITMAPINFOHEADER ih;
	ih.biBitCount = 24;
	ih.biClrImportant = 0;
	ih.biClrUsed = 0;
	ih.biCompression = BI_RGB;
	ih.biHeight = LONG(pBmp->GetHeight());
	ih.biWidth  = pBmp->GetWidth();
	ih.biPlanes = 1;
	ih.biSize = sizeof(ih);
	ih.biSizeImage = 0;
	ih.biXPelsPerMeter = 0;
	ih.biYPelsPerMeter = 0;

	hr = pFile->Write(&fh, sizeof(fh));
	if( hr != S_OK ) {
		return hr;
	}
	hr = pFile->Write(&ih, sizeof(ih));
	if( hr != S_OK ) {
		return hr;
	}
	RGBTRIPLE *pSrc = pBmp->GetData() + (pBmp->GetHeight()-1)*pBmp->GetWidth();
	for(DWORD y=0; y<pBmp->GetHeight(); y++) {
		hr = pFile->Write(pSrc, pBmp->GetWidth()*3);
		if( hr != S_OK ) {
			return hr;
		}
		pSrc -= pBmp->GetWidth();
	}

	return S_OK;
}

