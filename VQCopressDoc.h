// VQCopressDoc.h : interface of the CVQCopressDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VQCOPRESSDOC_H__8C50B7E1_57FF_4E28_8F85_02E9B758249C__INCLUDED_)
#define AFX_VQCOPRESSDOC_H__8C50B7E1_57FF_4E28_8F85_02E9B758249C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CVQCopressDoc : public CDocument
{
protected: // create from serialization only
	CVQCopressDoc();
	DECLARE_DYNCREATE(CVQCopressDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVQCopressDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVQCopressDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVQCopressDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VQCOPRESSDOC_H__8C50B7E1_57FF_4E28_8F85_02E9B758249C__INCLUDED_)
