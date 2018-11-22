// VQCopressView.h : interface of the CVQCopressView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VQCOPRESSVIEW_H__0CC724A6_3DA3_4480_9C06_5E0888DAC75F__INCLUDED_)
#define AFX_VQCOPRESSVIEW_H__0CC724A6_3DA3_4480_9C06_5E0888DAC75F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VQImage.h"
#include "VQImage8x8x16x15.h"
#include "DxRender.h"


class CVQCopressView : public CFormView
{
protected: // create from serialization only
	CVQCopressView();
	DECLARE_DYNCREATE(CVQCopressView)

public:
	//{{AFX_DATA(CVQCopressView)
	enum { IDD = IDD_VQCOPRESS_FORM };
	CString	m_strFilenameBmp;
	int		m_nCBSize;
	int		m_nCBSize2;
	int		m_nCBSize3;
	int		m_nBlockW;
	int		m_nBlockH;
	CString	m_strMessages;
	//}}AFX_DATA

// Attributes
public:
	CVQCopressDoc* GetDocument();

// Operations
public:
	static CVQCopressView* cela;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVQCopressView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bFirst;
	virtual ~CVQCopressView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void OnIdle();
	CVQImage			*m_vqImage;
	CVBitmap			*m_bmpImage;
	CVQImage8x8x16x15	*m_vqImage8x8x16x15;
	T_DXRENDER			*m_dxOriginal;
	T_DXRENDER			*m_dxOriginal2;
	T_DXRENDER			*m_dxPalette;
	T_DXRENDER			*m_dxPalette2;
	T_DXRENDER			*m_dxResultat;
	T_DXRENDER			*m_dxResultat8x8x16x15;
// Generated message map functions
protected:
	//{{AFX_MSG(CVQCopressView)
	afx_msg void OnBrowse();
	afx_msg void OnGo();
	afx_msg void OnShowPal();
	afx_msg void OnGo2();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in VQCopressView.cpp
inline CVQCopressDoc* CVQCopressView::GetDocument()
   { return (CVQCopressDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VQCOPRESSVIEW_H__0CC724A6_3DA3_4480_9C06_5E0888DAC75F__INCLUDED_)





















