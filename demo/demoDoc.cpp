// demoDoc.cpp : implementation of the CDemoDoc class
//

#include "stdafx.h"
#include "demo.h"

#include "demoDoc.h"
#include "CntrItem.h"

#include "..\pipeline\Scene.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc

IMPLEMENT_DYNCREATE(CDemoDoc, COleDocument)

BEGIN_MESSAGE_MAP(CDemoDoc, COleDocument)
	//{{AFX_MSG_MAP(CDemoDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleDocument::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleDocument::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleDocument::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_OLE_EDIT_CONVERT, COleDocument::OnEditConvert)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleDocument::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, COleDocument::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, COleDocument::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDemoDoc, COleDocument)
	//{{AFX_DISPATCH_MAP(CDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IDemo to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {B5CBCD35-EB69-11D2-80FF-006097A05989}
static const IID IID_IDemo =
{ 0xb5cbcd35, 0xeb69, 0x11d2, { 0x80, 0xff, 0x0, 0x60, 0x97, 0xa0, 0x59, 0x89 } };

BEGIN_INTERFACE_MAP(CDemoDoc, COleDocument)
	INTERFACE_PART(CDemoDoc, IID_IDemo, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc construction/destruction

CDemoDoc::CDemoDoc()
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CDemoDoc::~CDemoDoc()
{
	AfxOleUnlockApp();
}

BOOL CDemoDoc::OnNewDocument()
{
	if(!COleDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDemoDoc serialization

void CDemoDoc::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class COleDocument enables serialization
	//  of the container document's COleClientItem objects.
	COleDocument::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc diagnostics

#ifdef _DEBUG
void CDemoDoc::AssertValid() const
{
	COleDocument::AssertValid();
}

void CDemoDoc::Dump(CDumpContext& dc) const
{
	COleDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc commands

void CDemoDoc::OnFileOpen() 
{
   CScene aScene;
   aScene.load(std::string("C:\\projects\\Destiny\\data\\Scene2.scn"));
   aScene.render();
   aScene.save(std::string("c:\\projects\\destiny\\data\\Test.data"));

/*
	CFileDialog fd( TRUE );

   fd.DoModal();

   CString strSceneName = fd.GetPathName();

   if( strSceneName.IsEmpty() == 0 )
   {
      std::string objectName( (const char*)strSceneName );
      CScene aScene;
      aScene.load( objectName );
      aScene.render();
      
      fd.DoModal();
      CString strFileName = fd.GetPathName();
      if( strFileName.IsEmpty() == 0 )
      {
         std::string fileName( (const char*)strFileName );
         aScene.save( fileName );

      }
   }
*/	
}
