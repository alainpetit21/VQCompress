#if !defined(AFX_DXWINDOW_H__72277A5A_6A60_41A7_920F_9A3DAEEFA8E4__INCLUDED_)
#define AFX_DXWINDOW_H__72277A5A_6A60_41A7_920F_9A3DAEEFA8E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DxWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDxWindow dialog

class CDxWindow : public CDialog
{
// Construction
public:
	CDxWindow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDxWindow)
	enum { IDD = IDD_DX_WINDOW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDxWindow)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDxWindow)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXWINDOW_H__72277A5A_6A60_41A7_920F_9A3DAEEFA8E4__INCLUDED_)
