// DxWindow.cpp : implementation file
//

#include "stdafx.h"
#include "VQCopress.h"
#include "DxWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDxWindow dialog


CDxWindow::CDxWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CDxWindow::IDD, pParent), 
	m_dx(0)
{
	//{{AFX_DATA_INIT(CDxWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDxWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDxWindow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDxWindow, CDialog)
	//{{AFX_MSG_MAP(CDxWindow)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDxWindow message handlers
/*
unsigned char OnFrameWindow(tagT_WIN32FRAME	*p_this, unsigned long lData)
{
T_DXRENDER *pDx= (T_DXRENDER *)lData;

__try{
	if(!pDx->m_bDxInitiated)	
		return 1;

	//Draw
	IDirectDrawSurface* bb= DX_BeginScene(pDx);
	DX_FillBitmap(pDx, bb, 0x0);

	unsigned short *pPixel= DX_LockBB(pDx);


	//Render CodeBook
	if(CVQCopressView::cela->m_vqImage->m_pCodebook){
		RGBTRIPLE* palette= (RGBTRIPLE*)CVQCopressView::cela->m_vqImage->m_pCodebook;
		
		for(int j= 0; j < 16; ++j){
			for(int i= 0; i < 16; ++i){
				long color= (palette->rgbtRed<<16)|(palette->rgbtGreen<<8)|(palette->rgbtBlue<<0);
				for(int k= 0; k < 8; ++k){
					for(int l= 0; l < 8; ++l){
						DX_PutPixel_locked(pDx, (unsigned long*)pPixel, i*8+l, j*8+k, color);
					}
				}
				palette++;
			}			
		}
	}

	DX_UnLockBB(pDx, pPixel);
	DX_EndScene(pDx, &bb);
}__except(puts("in filter"), 1){
	puts("in except");
}

	return 1;
}
*/

BOOL CDxWindow::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL ret= CDialog::Create(IDD, pParentWnd);
	m_dx	= DX_Init(GetSafeHwnd(), 256, 256);
	
	return ret;
}

int CDxWindow::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}
