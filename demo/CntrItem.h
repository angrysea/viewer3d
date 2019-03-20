// CntrItem.h : interface of the CDemoCntrItem class
//

#if !defined(AFX_CNTRITEM_H__B5CBCD44_EB69_11D2_80FF_006097A05989__INCLUDED_)
#define AFX_CNTRITEM_H__B5CBCD44_EB69_11D2_80FF_006097A05989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDemoDoc;
class CDemoView;

class CDemoCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CDemoCntrItem)

// Constructors
public:
	CDemoCntrItem(CDemoDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CDemoDoc* GetDocument()
		{ return (CDemoDoc*)COleClientItem::GetDocument(); }
	CDemoView* GetActiveView()
		{ return (CDemoView*)COleClientItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoCntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CDemoCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__B5CBCD44_EB69_11D2_80FF_006097A05989__INCLUDED_)
