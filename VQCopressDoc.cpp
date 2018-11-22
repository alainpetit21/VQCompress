// VQCopressDoc.cpp : implementation of the CVQCopressDoc class
//

#include "stdafx.h"
#include "VQCopress.h"

#include "VQCopressDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVQCopressDoc

IMPLEMENT_DYNCREATE(CVQCopressDoc, CDocument)

BEGIN_MESSAGE_MAP(CVQCopressDoc, CDocument)
	//{{AFX_MSG_MAP(CVQCopressDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVQCopressDoc construction/destruction

CVQCopressDoc::CVQCopressDoc()
{
	// TODO: add one-time construction code here

}

CVQCopressDoc::~CVQCopressDoc()
{
}

BOOL CVQCopressDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVQCopressDoc serialization

void CVQCopressDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CVQCopressDoc diagnostics

#ifdef _DEBUG
void CVQCopressDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVQCopressDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVQCopressDoc commands
