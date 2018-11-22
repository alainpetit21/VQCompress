#ifndef _DX_RENDER_H_
#define _DX_RENDER_H_

#include <ddraw.h>
#include <math.h>

typedef struct tagT_DXRENDER
{
	RECT				m_DxRendererRect;
	RECT				m_DxViewRect;
	DDBLTFX				m_DxBlitFX;
	DDSURFACEDESC		m_DxDdsd;
	DDSCAPS				m_DxDdscaps;
	DDPIXELFORMAT		m_DxDdpf;
	HWND				m_DxFrameHWnd;
	IDirectDrawSurface*	m_pDxBB;
	IDirectDrawSurface*	m_pDxFB;
	IDirectDraw*		m_pDxDD;
	IDirectDrawClipper*	m_pDxClipper;
	IDirectDrawClipper*	m_pDxWindowClipper;
	int					m_DxPixelFormat;
	int					m_DxWindowClientX0;
	int					m_DxWindowClientY0;
	int					m_bDxFS;
	int					m_nDxWidth;
	int					m_nDxHeight;
	int					m_bDxInitiated;
}T_DXRENDER;


extern T_DXRENDER*			DX_Init					(HWND p_hwnd, int width, int height);
extern int					DX_Exit					(T_DXRENDER* p_dx, unsigned char bReleaseDD);
extern IDirectDrawSurface*	DX_GetFrontBuffer		(T_DXRENDER* p_dx);
extern IDirectDrawSurface*	DX_GetBackBuffer		(T_DXRENDER* p_dx);
extern int					DX_NewBitmap			(T_DXRENDER* p_dx, IDirectDrawSurface **ppBitmap, unsigned int nWidth, unsigned int nHeight);
extern int					DX_NewBitmapSystem		(T_DXRENDER* p_dx, IDirectDrawSurface **ppBitmap, unsigned int nWidth, unsigned int nHeight);
extern int					DX_FreeBitmap			(T_DXRENDER* p_dx, IDirectDrawSurface **ppBitmap);
extern int					DX_GetWidth				(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap);
extern int					DX_GetHeight			(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap);
extern int					DX_PutBitmap_i			(T_DXRENDER* p_dx, IDirectDrawSurface *pDestBitmap, int nDx, int nDy, int nDw, int nDh, int nMode, IDirectDrawSurface *pSrcBitmap, int nSx, int nSy, int nSw, int nSh);
extern IDirectDrawSurface*	DX_BeginScene			(T_DXRENDER* p_dx);
extern void					DX_EndScene				(T_DXRENDER* p_dx, IDirectDrawSurface** p_bit);
extern int					DX_FillBitmap			(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap, long lColor);
extern int					DX_FillRect_i			(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap, int left, int top, int right, int bottom, long lColor);
extern void					DX_DrawRect_i			(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap, int left, int top, int right, int bottom, long lColor);
extern unsigned short*		DX_LockSurface			(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap);
extern unsigned short*		DX_LockBB				(T_DXRENDER* p_dx);
extern void					DX_PutPixel_locked		(T_DXRENDER* p_dx, unsigned long *pImage, unsigned int nX, unsigned int nY, long lColor);
extern void					DX_DrawLine_locked		(T_DXRENDER* p_dx, unsigned long *pImage, long X0, long Y0, long X1, long Y1, long width, long lColor);
extern void					DX_DrawLineOctant0		(T_DXRENDER* p_dx, unsigned long *pImage, long lColor, unsigned int X0, unsigned int Y0, unsigned int DeltaX, unsigned int DeltaY, int XDirection);
extern void					DX_DrawLineOctant1		(T_DXRENDER* p_dx, unsigned long *pImage, long lColor, unsigned int X0, unsigned int Y0, unsigned int DeltaX, unsigned int DeltaY, int XDirection);
extern void					DX_DrawCircle_locked	(T_DXRENDER* p_dx, unsigned long *pImage, int p_ptX, int p_ptY, int radius, long lColor);
extern void					DX_UnLockSurface		(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap, unsigned short *pImage);
extern void					DX_UnLockBB				(T_DXRENDER* p_dx, unsigned short *pImage);
extern void					DX_DrawLine				(T_DXRENDER* p_dx, IDirectDrawSurface *pBitmap, float fX1, float fY1, float fX2, float fY2, long lColor);
extern IDirectDrawClipper*	DX_AttachClipper		(T_DXRENDER* p_dx, IDirectDrawSurface* surface, int RectCount, LPRECT ClipList);
extern DWORD				DX_DDColorMatch			(T_DXRENDER* p_dx, IDirectDrawSurface *pdds, COLORREF rgb);
extern HRESULT				DX_DDSetColorKey		(T_DXRENDER* p_dx, IDirectDrawSurface *pdds, COLORREF rgb);

#endif // _DX_RENDER_H_