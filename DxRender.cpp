#include "DxRender.h"

T_DXRENDER	gArRenderer[16];
int			gCptRenderer=0;

T_DXRENDER*
DX_Init(HWND p_hwnd, int width, int height)
{
T_DXRENDER	*p_dx;
RECT		ScreenRect= {0,0,width,height};

	
	if(gCptRenderer > 16)
		_asm{int 3};
		
	p_dx= &gArRenderer[gCptRenderer];
	p_dx->m_nDxWidth	= width;
	p_dx->m_nDxHeight	= height;
	p_dx->m_DxFrameHWnd	= p_hwnd;

	memset(&p_dx->m_DxDdsd,0,sizeof(p_dx->m_DxDdsd));
	memset(&p_dx->m_DxDdpf,0,sizeof(p_dx->m_DxDdpf));
	memset(&p_dx->m_DxBlitFX, 0, sizeof(p_dx->m_DxBlitFX));
	p_dx->m_DxDdsd.dwSize = sizeof(p_dx->m_DxDdsd);
	p_dx->m_DxDdpf.dwSize = sizeof(p_dx->m_DxDdpf);
	p_dx->m_DxBlitFX.dwSize = sizeof(p_dx->m_DxBlitFX);

	DirectDrawCreate(NULL, &p_dx->m_pDxDD, NULL);
	IDirectDraw_SetCooperativeLevel(p_dx->m_pDxDD, p_dx->m_DxFrameHWnd, DDSCL_NORMAL);
	p_dx->m_DxDdsd.dwFlags				= DDSD_CAPS;
	p_dx->m_DxDdsd.ddsCaps.dwCaps		= DDSCAPS_PRIMARYSURFACE;
	p_dx->m_DxDdsd.dwBackBufferCount	= 0;
	IDirectDraw_CreateSurface(p_dx->m_pDxDD, &p_dx->m_DxDdsd, &p_dx->m_pDxFB,NULL);
	IDirectDrawSurface_GetPixelFormat(p_dx->m_pDxFB, &p_dx->m_DxDdpf);
	p_dx->m_DxPixelFormat = p_dx->m_DxDdpf.dwRGBBitCount;

	DX_NewBitmap(p_dx, &p_dx->m_pDxBB, p_dx->m_nDxWidth, p_dx->m_nDxHeight);
	DX_FillBitmap(p_dx, p_dx->m_pDxBB,0x0);

	p_dx->m_pDxClipper = DX_AttachClipper(p_dx, p_dx->m_pDxBB,1,&ScreenRect);
	p_dx->m_DxWindowClientX0= -ScreenRect.left;
	p_dx->m_DxWindowClientY0= -ScreenRect.top;
	IDirectDraw_CreateClipper(p_dx->m_pDxDD, 0, &p_dx->m_pDxWindowClipper,NULL);
	IDirectDrawClipper_SetHWnd(p_dx->m_pDxWindowClipper, 0, p_dx->m_DxFrameHWnd);
	IDirectDrawSurface_SetClipper(p_dx->m_pDxFB, p_dx->m_pDxWindowClipper);

	gCptRenderer++;
	p_dx->m_bDxInitiated= 1;
	return p_dx;
}

int 
DX_Exit(T_DXRENDER* p_dx, unsigned char bReleaseDD)
{
	if (p_dx->m_pDxClipper)		IDirectDrawClipper_Release(p_dx->m_pDxClipper);
	if (p_dx->m_pDxWindowClipper)	IDirectDrawClipper_Release(p_dx->m_pDxWindowClipper);
	if (p_dx->m_pDxBB)				IDirectDrawSurface_Release(p_dx->m_pDxBB);
	if (p_dx->m_pDxFB)				IDirectDrawSurface_Release(p_dx->m_pDxFB);
	if (p_dx->m_pDxDD)				IDirectDraw_Release(p_dx->m_pDxDD);
	return(1);
}

IDirectDrawSurface*	
DX_GetFrontBuffer(T_DXRENDER* p_dx)
{
	return p_dx->m_pDxFB;
}

IDirectDrawSurface*	
DX_GetBackBuffer(T_DXRENDER* p_dx)
{
	return p_dx->m_pDxBB;
}

int 
DX_NewBitmap(T_DXRENDER* p_dx, IDirectDrawSurface **ppBitmap, unsigned int nWidth, unsigned int nHeight)
{
	HRESULT res;
	DDSURFACEDESC TempDescription;

	TempDescription.dwSize = sizeof(TempDescription);
	TempDescription.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	TempDescription.dwWidth = nWidth;
	TempDescription.dwHeight = nHeight;

//	TempDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;

//	res= IDirectDraw_CreateSurface(p_dx->m_pDxDD, &TempDescription,ppBitmap,NULL);
 
//	if(res == DDERR_OUTOFVIDEOMEMORY){
//		MessageBox(0, "Hmmm .... Ran out of Video Memory ... What is this cheesy video card that you have?\n\r Anyway, I will use some system memory instead. ", 0, 0);
		TempDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
		res= IDirectDraw_CreateSurface(p_dx->m_pDxDD, &TempDescription,ppBitmap,NULL);
		if(res == DDERR_OUTOFMEMORY){
			MessageBox(0, "Hmmm ... Man your in deep shit, no more video RAM, no more system RAM. Take this gun and shoot yourself!", 0, 0);
		}
//	}
	DX_DDSetColorKey(p_dx, (*ppBitmap), RGB(0, 0, 0));

	return (1);
}

int 
DX_NewBitmapSystem(T_DXRENDER* p_dx, IDirectDrawSurface **ppBitmap, unsigned int nWidth, unsigned int nHeight)
{
	HRESULT res;
	DDSURFACEDESC TempDescription;

	TempDescription.dwSize = sizeof(TempDescription);
	TempDescription.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	TempDescription.dwWidth = nWidth;
	TempDescription.dwHeight = nHeight;

	TempDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;

	res= IDirectDraw_CreateSurface(p_dx->m_pDxDD, &TempDescription,ppBitmap,NULL);
	if(res == DDERR_OUTOFMEMORY){
		MessageBox(0, "Hmmm ... Man your in deep shit, no more system RAM. Take this gun and shoot yourself!", 0, 0);
	}
	DX_DDSetColorKey(p_dx, (*ppBitmap), RGB(0, 0, 0));

	return (1);
}

int 
DX_FreeBitmap(T_DXRENDER* p_dx, IDirectDrawSurface **ppBitmap)
{
	IDirectDrawSurface_Release(*ppBitmap);
	(*ppBitmap)= NULL;
	return (1);
}
int 
DX_GetWidth(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap)
{
	p_dx->m_DxDdsd.dwFlags = DDSD_WIDTH;
	IDirectDrawSurface_GetSurfaceDesc(pBitmap, &p_dx->m_DxDdsd);

	return (int)p_dx->m_DxDdsd.dwWidth;
}

int
DX_GetHeight(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap)
{
	p_dx->m_DxDdsd.dwFlags = DDSD_HEIGHT;
	IDirectDrawSurface_GetSurfaceDesc(pBitmap, &p_dx->m_DxDdsd);

	return (int)p_dx->m_DxDdsd.dwHeight;
}

int 
DX_PutBitmap_i(T_DXRENDER* p_dx, IDirectDrawSurface *pDestBitmap, int nDx, int nDy, int nDw, int nDh, int nMode, IDirectDrawSurface *pSrcBitmap, int nSx, int nSy, int nSw, int nSh)
{
	HRESULT res;
	RECT dstRect={nDx, nDy, nDx+nDw, nDy+nDh};
	RECT srcRect={nSx, nSy, nSx+nSw, nSy+nSh};
	if(!pDestBitmap)	pDestBitmap= p_dx->m_pDxBB;

	if(nMode == 1)
		res= IDirectDrawSurface_Blt(pDestBitmap, &dstRect, pSrcBitmap, &srcRect, DDBLT_WAIT, NULL);
	else
		res= IDirectDrawSurface_Blt(pDestBitmap, &dstRect, pSrcBitmap, &srcRect, DDBLT_WAIT|DDBLT_KEYSRC, NULL);
	return 0;
}

IDirectDrawSurface* 
DX_BeginScene(T_DXRENDER* p_dx)
{
	return p_dx->m_pDxBB;
}

void 
DX_EndScene(T_DXRENDER* p_dx, IDirectDrawSurface** p_bit)
{
	RECT dest;
	GetWindowRect(p_dx->m_DxFrameHWnd, &dest);
	dest.left	+= p_dx->m_DxWindowClientX0;
	dest.top	+= p_dx->m_DxWindowClientY0;
	dest.right	= dest.left+ p_dx->m_nDxWidth;
	dest.bottom = dest.top + p_dx->m_nDxHeight;
	IDirectDrawSurface_Blt(p_dx->m_pDxFB, &dest, p_dx->m_pDxBB, NULL, DDBLT_WAIT,NULL);
}

int 
DX_FillBitmap(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap, long lColor)
{
	if(!pBitmap) pBitmap= p_dx->m_pDxBB;
	memset(&p_dx->m_DxBlitFX, 0x0, sizeof(p_dx->m_DxBlitFX));
	p_dx->m_DxBlitFX.dwSize = sizeof(p_dx->m_DxBlitFX);
	p_dx->m_DxBlitFX.dwFillColor = lColor;
	IDirectDrawSurface_Blt(pBitmap, NULL,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT, &p_dx->m_DxBlitFX);
	return (1);
}

int 
DX_FillRect_i(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap, int left, int top,	int right, int bottom, long lColor)
{
	RECT rec= {left, top, right, bottom};
	p_dx->m_DxBlitFX.dwFillColor = lColor;
	IDirectDrawSurface_Blt(pBitmap, &rec,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&p_dx->m_DxBlitFX);
	return (1);
}

unsigned short*
DX_LockSurface(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap)
{
	IDirectDrawSurface_Lock(pBitmap, NULL, &p_dx->m_DxDdsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	return (unsigned short*)p_dx->m_DxDdsd.lpSurface;
}

unsigned short*
DX_LockBB(T_DXRENDER* p_dx)
{
	IDirectDrawSurface_Lock(p_dx->m_pDxBB, NULL, &p_dx->m_DxDdsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	return (unsigned short*)p_dx->m_DxDdsd.lpSurface;
}

void 
DX_PutPixel_locked(T_DXRENDER* p_dx, unsigned long *pImage, unsigned int nX, unsigned int nY, long lColor)
{
	if((nX < 0)|| (nX > p_dx->m_DxDdsd.dwWidth) || (nY < 0)|| (nY > p_dx->m_DxDdsd.dwHeight))	return;
	
	*(pImage + (nY*(p_dx->m_DxDdsd.lPitch>>2)) + nX)=	(unsigned long)lColor;
}

void 
DX_DrawLineOctant0(T_DXRENDER* p_dx, unsigned long *pImage, long lColor, unsigned int X0, unsigned int Y0, unsigned int DeltaX, unsigned int DeltaY, int XDirection)
{
	int DeltaYx2;
	int DeltaYx2MinusDeltaXx2;
	int ErrorTerm;

	DeltaYx2 = DeltaY * 2;
	DeltaYx2MinusDeltaXx2 = DeltaYx2 - (int)(DeltaX * 2);
	ErrorTerm = DeltaYx2 - (int)DeltaX;

	DX_PutPixel_locked(p_dx, pImage, X0, Y0, lColor);
	while(DeltaX--){
		if(ErrorTerm >= 0){
			Y0++;
			ErrorTerm += DeltaYx2MinusDeltaXx2;
		}else{
			ErrorTerm += DeltaYx2;
		}
		X0 += XDirection;
		DX_PutPixel_locked(p_dx, pImage, X0, Y0, lColor);
	}
}

void 
DX_DrawLineOctant1(T_DXRENDER* p_dx, unsigned long *pImage, long lColor, unsigned int X0, unsigned int Y0, unsigned int DeltaX, unsigned int DeltaY, int XDirection)
{
	int DeltaXx2;
	int DeltaXx2MinusDeltaYx2;
	int ErrorTerm;

	DeltaXx2 = DeltaX * 2;
	DeltaXx2MinusDeltaYx2 = DeltaXx2 - (int)(DeltaY * 2);
	ErrorTerm = DeltaXx2 - (int) DeltaY;

	DX_PutPixel_locked(p_dx, pImage, X0, Y0, lColor);
	while(DeltaY--){
		if(ErrorTerm >= 0){
			X0 += XDirection;
			ErrorTerm += DeltaXx2MinusDeltaYx2;
		}else{
			ErrorTerm += DeltaXx2;
		}
		Y0++;
		DX_PutPixel_locked(p_dx, pImage, X0, Y0, lColor);
	}
}

void 
DX_DrawLine_locked(T_DXRENDER* p_dx, unsigned long *pImage, long X0, long Y0, long X1, long Y1, long width, long lColor)
{
   int		DeltaX
	   ,	DeltaY;
   int		Temp;

   if(Y0 > Y1){
      Temp = Y0;
      Y0 = Y1;
      Y1 = Temp;
	  
      Temp = X0;
      X0 = X1;
      X1 = Temp;
   }

   DeltaX = X1 - X0;
   DeltaY = Y1 - Y0;

   if(DeltaX > 0){
      if(DeltaX > DeltaY){
         DX_DrawLineOctant0(p_dx, pImage, lColor, X0, Y0, DeltaX, DeltaY, 1);
      }else{
         DX_DrawLineOctant1(p_dx, pImage, lColor, X0, Y0, DeltaX, DeltaY, 1);
      }
   }else{
      DeltaX= -DeltaX;
      if(DeltaX > DeltaY){
         DX_DrawLineOctant0(p_dx, pImage, lColor, X0, Y0, DeltaX, DeltaY, -1);
      } else {
         DX_DrawLineOctant1(p_dx, pImage, lColor, X0, Y0, DeltaX, DeltaY, -1);
      }
   }
}

void 
DX_DrawCircle_locked(T_DXRENDER* p_dx, unsigned long *pImage, int p_ptX, int p_ptY, int radius, long lColor)
{
	float i;
	float curX;
	float curY;

	i=255;
	while(i--){
		curX= p_ptX + ((float)cos(i))*radius;
		curY= p_ptY + ((float)sin(i))*radius;
		DX_PutPixel_locked(p_dx, pImage, (int)curX, (int)curY, lColor);
	}
}

void
DX_UnLockSurface(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap, unsigned short *pImage)
{
	IDirectDrawSurface_Unlock(pBitmap, pImage);
}

void
DX_UnLockBB(T_DXRENDER* p_dx, unsigned short *pImage)
{
	IDirectDrawSurface_Unlock(p_dx->m_pDxBB, pImage);
}

IDirectDrawClipper*
DX_AttachClipper(T_DXRENDER* p_dx, IDirectDrawSurface* surface, int RectCount, LPRECT ClipList)
{
	int Count;
	LPDIRECTDRAWCLIPPER Clipper;
	LPRGNDATA RegionData;

	IDirectDraw_CreateClipper(p_dx->m_pDxDD, 0, &Clipper, NULL);

	RegionData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+(RectCount*sizeof(RECT)));
	memcpy(RegionData->Buffer, ClipList, sizeof(RECT)*RectCount);

	RegionData->rdh.dwSize          = sizeof(RGNDATAHEADER);
	RegionData->rdh.iType           = RDH_RECTANGLES;
	RegionData->rdh.nCount          = RectCount;
	RegionData->rdh.nRgnSize        = RectCount*sizeof(RECT);

	RegionData->rdh.rcBound.left    =  64000;
	RegionData->rdh.rcBound.top     =  64000;
	RegionData->rdh.rcBound.right   = -64000;
	RegionData->rdh.rcBound.bottom  = -64000;

	for (Count = 0; Count < RectCount; Count++){
		if (ClipList[Count].left	< RegionData->rdh.rcBound.left)		RegionData->rdh.rcBound.left	= ClipList[Count].left;
		if (ClipList[Count].right	> RegionData->rdh.rcBound.right)	RegionData->rdh.rcBound.right	= ClipList[Count].right;
		if (ClipList[Count].top		< RegionData->rdh.rcBound.top)		RegionData->rdh.rcBound.top		= ClipList[Count].top;
		if (ClipList[Count].bottom	> RegionData->rdh.rcBound.bottom)	RegionData->rdh.rcBound.bottom	= ClipList[Count].bottom;
	};

	if (FAILED(IDirectDrawClipper_SetClipList(Clipper, RegionData,0))){
		free(RegionData);
		return(NULL);
	};

	if (FAILED(IDirectDrawSurface_SetClipper(surface, Clipper))){
		free(RegionData);
		return(NULL);
	};

	free(RegionData);
	return Clipper;
};

DWORD 
DX_DDColorMatch(T_DXRENDER* p_dx, IDirectDrawSurface *pdds, COLORREF rgb)
{
    COLORREF rgbT;
    HDC hdc;
    DWORD dw = CLR_INVALID;
    DDSURFACEDESC ddsd;
    HRESULT hres;

    if (rgb != CLR_INVALID && IDirectDrawSurface_GetDC(pdds, &hdc) == DD_OK){
        rgbT = GetPixel(hdc, 0, 0);             // save current pixel value
        SetPixel(hdc, 0, 0, rgb);               // set our value
        IDirectDrawSurface_ReleaseDC(pdds, hdc);
    }
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = IDirectDrawSurface_Lock(pdds, NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING);

    if (hres == DD_OK){
        dw  = *(DWORD *)ddsd.lpSurface;                     // get DWORD
        dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;  // mask it to bpp
        IDirectDrawSurface_Unlock(pdds, NULL);
    }
    if (rgb != CLR_INVALID && IDirectDrawSurface_GetDC(pdds, &hdc) == DD_OK){
        SetPixel(hdc, 0, 0, rgbT);
        IDirectDrawSurface_ReleaseDC(pdds, hdc);
    }

    return dw;
}

HRESULT 
DX_DDSetColorKey(T_DXRENDER* p_dx, IDirectDrawSurface *pdds, COLORREF rgb)
{
    DDCOLORKEY          ddck;

    ddck.dwColorSpaceLowValue  = DX_DDColorMatch(p_dx, pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return IDirectDrawSurface_SetColorKey(pdds, DDCKEY_SRCBLT, &ddck);
}
