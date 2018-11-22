// VQCopressView.cpp : implementation of the CVQCopressView class
//

#include "stdafx.h"
#include "VQCopress.h"

#include "VQCopressDoc.h"
#include "VQCopressView.h"

#include "VQImage.h"
#include "NeuQuant.h"
#include "GLAQuant.h"
#include "QuantVect.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVQCopressView* CVQCopressView::cela;
unsigned char gInvalidate= 0;

///////////////////////////
//ON FRAME DX
///////////////////////////
void OnPassesCompress(int dwPass)
{
CString str= "";
GLAQuant<TQuant_Palette> *pglaq= CVQCopressView::cela->m_vqImage->pCurGlaq;

	if(pglaq){
		str.Format("GLA Pass %d... Updated Vectors: %lu ,Used Vectors: %lu \r\n", dwPass, pglaq->m_dwUpdated, pglaq->m_dwUsed);
	}
	
	CVQCopressView::cela->m_strMessages+= str;
	gInvalidate=1;
}
void OnPassesCompress2(int dwPass)
{
}

/////////////////////////////////////////////////////////////////////////////
// CVQCopressView

IMPLEMENT_DYNCREATE(CVQCopressView, CFormView)

BEGIN_MESSAGE_MAP(CVQCopressView, CFormView)
	//{{AFX_MSG_MAP(CVQCopressView)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_GO, OnGo)
	ON_BN_CLICKED(IDC_GO2, OnGo2)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVQCopressView construction/destruction

CVQCopressView::CVQCopressView()
	: CFormView(CVQCopressView::IDD),
	m_bFirst(TRUE),
	m_vqImage(0),
	m_vqImage8x8x16x15(0),
	m_bmpImage(0)
{
	//{{AFX_DATA_INIT(CVQCopressView)
	m_strFilenameBmp = _T("");
	m_nCBSize = 256;
	m_strMessages = _T("");
	m_nCBSize2 = 15;
	m_nCBSize3 = 16;
	m_nBlockW = 8;
	m_nBlockH = 8;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	CVQCopressView::cela= this;
}

CVQCopressView::~CVQCopressView()
{
}

void CVQCopressView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVQCopressView)
	DDX_Text(pDX, IDC_FILENAME_BMP, m_strFilenameBmp);
	DDX_Text(pDX, IDC_CBSIZE, m_nCBSize);
	DDX_Text(pDX, IDC_MESSAGES, m_strMessages);
	DDX_Text(pDX, IDC_CBSIZE2, m_nCBSize2);
	DDX_Text(pDX, IDC_CBSIZE3, m_nCBSize3);
	DDX_Text(pDX, IDC_BLOCKW, m_nBlockW);
	DDX_Text(pDX, IDC_BLOCKH, m_nBlockH);
	//}}AFX_DATA_MAP
}

BOOL CVQCopressView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CVQCopressView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CVQCopressView printing

BOOL CVQCopressView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVQCopressView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVQCopressView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CVQCopressView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CVQCopressView diagnostics

#ifdef _DEBUG
void CVQCopressView::AssertValid() const
{
	CFormView::AssertValid();
}

void CVQCopressView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CVQCopressDoc* CVQCopressView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVQCopressDoc)));
	return (CVQCopressDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVQCopressView message handlers

void CVQCopressView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if(m_bFirst){
		m_bFirst= FALSE;
		HWND temp;
		GetDlgItem(IDC_DX_PALETTE, &temp);	m_dxPalette= DX_Init(temp, 256, 256);
		GetDlgItem(IDC_DX_PALETTE2, &temp);	m_dxPalette2= DX_Init(temp, 256, 256);
		GetDlgItem(IDC_DX_SOURCE, &temp);	m_dxOriginal= DX_Init(temp, 256, 256);
		GetDlgItem(IDC_DX_SOURCE2, &temp);	m_dxOriginal2= DX_Init(temp, 256, 256);
		GetDlgItem(IDC_DX_DEST, &temp);		m_dxResultat= DX_Init(temp, 256, 256);
		GetDlgItem(IDC_DX_DEST2, &temp);	m_dxResultat8x8x16x15= DX_Init(temp, 256, 256);
	}
}

void CVQCopressView::OnBrowse() 
{
	CFileDialog fd(TRUE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");
	
	UpdateData(TRUE);
	if(IDOK == fd.DoModal()){
		m_strFilenameBmp= fd.GetPathName();
		UpdateData(FALSE);
	}
}

void CVQCopressView::OnGo() 
{
	UpdateData(TRUE);
	m_strMessages="";

	if(m_vqImage)	{delete m_vqImage;	m_vqImage	= 0;}
	if(m_bmpImage)	{delete m_bmpImage;	m_bmpImage	= 0;}

	m_bmpImage	= new CVBitmap;
	m_vqImage	= new CVQImage;

	if(S_OK != LoadBmp(m_strFilenameBmp.operator LPCTSTR(), m_bmpImage)){
		if(m_vqImage)	{delete m_vqImage;	m_vqImage	= 0;}
		if(m_bmpImage)	{delete m_bmpImage;	m_bmpImage	= 0;}
		return;
	}

	m_vqImage->Create(m_bmpImage, 1, m_nCBSize, OnPassesCompress);
}


void CVQCopressView::OnGo2() 
{
	UpdateData(TRUE);
	m_strMessages="";
	
	if(m_vqImage8x8x16x15)	{delete m_vqImage8x8x16x15;	m_vqImage8x8x16x15	= 0;}
	if(m_bmpImage)			{delete m_bmpImage;	m_bmpImage	= 0;}
	
	m_bmpImage			= new CVBitmap;
	m_vqImage8x8x16x15	= new CVQImage8x8x16x15;
	
	if(S_OK != LoadBmp(m_strFilenameBmp.operator LPCTSTR(), m_bmpImage)){
		if(m_vqImage8x8x16x15)	{delete m_vqImage8x8x16x15;	m_vqImage8x8x16x15	= 0;}
		if(m_bmpImage)			{delete m_bmpImage;	m_bmpImage	= 0;}
		return;
	}
	
	m_vqImage8x8x16x15->Create(m_bmpImage, 1, m_nCBSize2, m_nCBSize3, m_nBlockW, m_nBlockH, OnPassesCompress2);
}

void CVQCopressView::OnIdle()
{
IDirectDrawSurface* bb;
unsigned short *pPixel;

	/////////////
	//Draw Palete
	if(!m_dxPalette->m_bDxInitiated)	
		return;

	bb= DX_BeginScene(m_dxPalette);
	DX_FillBitmap(m_dxPalette, bb, 0x0);

	pPixel= DX_LockBB(m_dxPalette);


	//Render CodeBook
	if(CVQCopressView::cela->m_vqImage && CVQCopressView::cela->m_vqImage->m_pCodebook){
		RGBTRIPLE* palette= (RGBTRIPLE*)CVQCopressView::cela->m_vqImage->m_pCodebook;
		int cptColor= m_nCBSize;
		
		for(int j= 0; j < 16; ++j){
			for(int i= 0; i < 16; ++i){
				long color= (palette->rgbtRed<<16)|(palette->rgbtGreen<<8)|(palette->rgbtBlue<<0);
				for(int k= 0; k < 16; ++k){
					for(int l= 0; l < 16; ++l){
						DX_PutPixel_locked(m_dxPalette, (unsigned long*)pPixel, i*16+l, j*16+k, color);
					}
				}
				if(cptColor)
					if(cptColor--)
						palette++;
			}			
		}
	}

	DX_UnLockBB(m_dxPalette, pPixel);
	DX_EndScene(m_dxPalette, &bb);

	/////////////
	//Draw Bmp Image Original 1
	if(!m_dxOriginal->m_bDxInitiated)	
		return;

	bb= DX_BeginScene(m_dxOriginal);
	DX_FillBitmap(m_dxOriginal, bb, 0x0);

	pPixel= DX_LockBB(m_dxOriginal);
	if(CVQCopressView::cela->m_bmpImage){
		RGBTRIPLE* pData= CVQCopressView::cela->m_bmpImage->GetData();
		int w= CVQCopressView::cela->m_bmpImage->GetWidth();
		int h= CVQCopressView::cela->m_bmpImage->GetHeight();
		float rX= w/256.0f, rY= h/256.0f;
		float cX, cY;
		int i, j;

		for(j= 0, cY= 0; j < 256; ++j, cY+= rY){
			for(i= 0, cX= 0; i < 256; ++i, cX+= rX){
				int ciX= (int)cX;
				int ciY= (int)cY;
				long color= (pData[ciY*w+ciX].rgbtRed<<16)|(pData[ciY*w+ciX].rgbtGreen<<8)|(pData[ciY*w+ciX].rgbtBlue<<0);
				DX_PutPixel_locked(m_dxOriginal, (unsigned long*)pPixel, i, j, color);
			}			
		}
	}

	DX_UnLockBB(m_dxOriginal, pPixel);
	DX_EndScene(m_dxOriginal, &bb);

	/////////////
	//Draw Bmp Image Original 2
	if(!m_dxOriginal2->m_bDxInitiated)	
		return;
	
	bb= DX_BeginScene(m_dxOriginal2);
	DX_FillBitmap(m_dxOriginal2, bb, 0x0);
	
	pPixel= DX_LockBB(m_dxOriginal2);
	if(CVQCopressView::cela->m_bmpImage){
		RGBTRIPLE* pData= CVQCopressView::cela->m_bmpImage->GetData();
		int w= CVQCopressView::cela->m_bmpImage->GetWidth();
		int h= CVQCopressView::cela->m_bmpImage->GetHeight();
		float rX= w/256.0f, rY= h/256.0f;
		float cX, cY;
		int i, j;
		
		for(j= 0, cY= 0; j < 256; ++j, cY+= rY){
			for(i= 0, cX= 0; i < 256; ++i, cX+= rX){
				int ciX= (int)cX;
				int ciY= (int)cY;
				long color= (pData[ciY*w+ciX].rgbtRed<<16)|(pData[ciY*w+ciX].rgbtGreen<<8)|(pData[ciY*w+ciX].rgbtBlue<<0);
				DX_PutPixel_locked(m_dxOriginal2, (unsigned long*)pPixel, i, j, color);
			}			
		}
	}
	
	DX_UnLockBB(m_dxOriginal2, pPixel);
	DX_EndScene(m_dxOriginal2, &bb);
	
	/////////////
	//Draw Vq Image
	if(!m_dxResultat->m_bDxInitiated)	
		return;

	bb= DX_BeginScene(m_dxResultat);
	DX_FillBitmap(m_dxResultat, bb, 0x0);

	pPixel= DX_LockBB(m_dxResultat);
	if(CVQCopressView::cela->m_vqImage && 
	CVQCopressView::cela->m_vqImage->GetCodebook() &&
	CVQCopressView::cela->m_vqImage->GetBits()){
		RGBTRIPLE* pCodeBook= (RGBTRIPLE*) CVQCopressView::cela->m_vqImage->GetCodebook();
		unsigned char* pIdx	= (unsigned char*) CVQCopressView::cela->m_vqImage->GetBits();
		int w= CVQCopressView::cela->m_vqImage->m_dwWidth;
		int h= CVQCopressView::cela->m_vqImage->m_dwHeight;
		int wQ= CVQCopressView::cela->m_vqImage->m_dwWidthQ;
		int hQ= CVQCopressView::cela->m_vqImage->m_dwHeightQ;
		float rX= w/256.0f, rY= h/256.0f;
		float cX, cY;
		int i, j;

		for(j= 0, cY= 0; j < 256; ++j, cY+= rY){
			for(i= 0, cX= 0; i < 256; ++i, cX+= rX){
				int ciX= (int)cX;
				int ciY= (int)cY;
				long Idx= pIdx[ciY*wQ+ciX];
				long color= (pCodeBook[Idx].rgbtRed<<16)|(pCodeBook[Idx].rgbtGreen<<8)|(pCodeBook[Idx].rgbtBlue<<0);
				DX_PutPixel_locked(m_dxResultat, (unsigned long*)pPixel, i, j, color);
			}			
		}
	}

	DX_UnLockBB(m_dxResultat, pPixel);
	DX_EndScene(m_dxResultat, &bb);

	/////////////
	//Draw Vq Image 8x8x15x16
	if(!m_dxResultat8x8x16x15->m_bDxInitiated)	
		return;
	
	bb= DX_BeginScene(m_dxResultat8x8x16x15);
	DX_FillBitmap(m_dxResultat8x8x16x15, bb, 0x0);
	
	pPixel= DX_LockBB(m_dxResultat8x8x16x15);
	if(CVQCopressView::cela->m_vqImage8x8x16x15 && CVQCopressView::cela->m_vqImage8x8x16x15->m_bReadyToDraw){
		CVQImage8x8x16x15	*pt		= CVQCopressView::cela->m_vqImage8x8x16x15;

		int i,j;
		int w= pt->m_dwWidth;
		int h= pt->m_dwHeight;
		int wQ= pt->m_dwWidthQ;
		int hQ= pt->m_dwHeightQ;
		float rX= w/256.0f, rY= h/256.0f;
		float cX, cY;

		for(j= 0, cY= 0; j < 256; ++j, cY+= rY){
			for(i= 0, cX= 0; i < 256; ++i, cX+= rX){
				int				ciX			= (int)cX;
				int				ciY			= (int)cY;
				CVQImage		*ptImage	= &pt->m_allChopedRes[(ciY/pt->m_dwBlocH)*(pt->m_dwWidth/pt->m_dwBlocW)+(ciX/pt->m_dwBlocW)];
				RGBTRIPLE		*pCodeBook	= (RGBTRIPLE*) ptImage->GetCodebook();
				unsigned char	*pIdx		= (unsigned char*) ptImage->GetBits();

				if(pCodeBook && pIdx && ptImage->m_bFinished){ 
					long Idx	= pIdx[(ciY%pt->m_dwBlocH)*pt->m_dwBlocW+(ciX%pt->m_dwBlocW)];
					long color	= (pCodeBook[Idx].rgbtRed<<16)|(pCodeBook[Idx].rgbtGreen<<8)|(pCodeBook[Idx].rgbtBlue<<0);
					DX_PutPixel_locked(m_dxResultat, (unsigned long*)pPixel, i, j, color);
				}
			}			
		}
	}
	
	DX_UnLockBB(m_dxResultat8x8x16x15, pPixel);
	DX_EndScene(m_dxResultat8x8x16x15, &bb);

	/////////////
	//Draw Palete
	if(!m_dxPalette2->m_bDxInitiated)	
		return;

	bb= DX_BeginScene(m_dxPalette2);
	DX_FillBitmap(m_dxPalette2, bb, 0x0);

	pPixel= DX_LockBB(m_dxPalette2);


	//Render CodeBook
	if(CVQCopressView::cela->m_vqImage8x8x16x15 && CVQCopressView::cela->m_vqImage8x8x16x15->m_pCodebook){
		RGBTRIPLE* palette= (RGBTRIPLE*)CVQCopressView::cela->m_vqImage8x8x16x15->m_pCodebook;
		int cptColor= m_nCBSize2*m_nCBSize3;
		
		for(int j= 0; j < m_nCBSize3; ++j){
			for(int i= 0; i < m_nCBSize2; ++i){
				long color= (palette->rgbtRed <<16)|(palette->rgbtGreen<<8)|(palette->rgbtBlue<<0);
//				for(int k= 0; k < 16; ++k){
					for(int l= 0; l < 16; ++l){
						DX_PutPixel_locked(m_dxPalette2, (unsigned long*)pPixel, i*16+l, j/**16+k*/, color);
					}
//				}
				if(cptColor)
					if(cptColor--)
						palette++;
			}			
		}
	}

	DX_UnLockBB(m_dxPalette2, pPixel);
	DX_EndScene(m_dxPalette2, &bb);


	

	/////////////
	//Draw CTL
	if(gInvalidate){
		CVQCopressView::cela->UpdateData(FALSE);
		gInvalidate= FALSE;
	}

}


void CVQCopressView::OnSave() 
{
CString chaine= m_strFilenameBmp + ".vq.bmp";
CVBitmap *pBitmap;

	CVQCopressView::cela->m_vqImage->Decompress1x1x256(pBitmap);

	SaveBmp((char*)chaine.operator LPCTSTR(), pBitmap);
}
