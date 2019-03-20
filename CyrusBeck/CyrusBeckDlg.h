// CCyrusBeckDlg.h : Declaration of the CCyrusBeckDlg

#ifndef __CYRUSBECKDLG_H_
#define __CYRUSBECKDLG_H_

#include "resource.h"       // main symbols
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CCyrusBeckDlg
class CCyrusBeckDlg : 
	public CAxDialogImpl<CCyrusBeckDlg>
{
public:
	CCyrusBeckDlg()
      :m_LineType(none),
       m_bHasEdge(false),
       m_bHasIntersect(false),
       m_bInDrawMode(false)
	{
      m_edgeStart.x = 0;
      m_edgeStart.y = 0;
      m_edgeEnd.x = 0;
      m_edgeEnd.y = 0;
      m_intersectStart.x = 0;
      m_intersectStart.y = 0;
      m_intersectEnd.x = 0;
      m_intersectEnd.y = 0;
      m_intersectPt.x=0;
      m_intersectPt.y=0;
	}

	~CCyrusBeckDlg()
	{
	}
   
   void compute();

   void CCyrusBeckDlg::drawLine(POINT startPt, POINT endPt)
   {
      RECT rcInvalid;
      getInvaldateLine(startPt, endPt, rcInvalid);
      InvalidateRect( &rcInvalid, true );
   }

   void getInvaldateLine(POINT startPt, POINT endPt, RECT &rcInvalid)
   {

      if(startPt.x<endPt.x)
      {
         rcInvalid.left=startPt.x;
         rcInvalid.right=endPt.x;
      }
      else
      {
         rcInvalid.left=endPt.x;
         rcInvalid.right=startPt.x;
      }

      if(startPt.y<endPt.y)
      {
         rcInvalid.top=startPt.y;
         rcInvalid.bottom=endPt.y;
      }
      else
      {
         rcInvalid.top=endPt.y;
         rcInvalid.bottom=startPt.y;
      }

      InflateRect( &rcInvalid, 3, 3 );
   }

   void drawCircle(POINT &pt)
   {
      RECT rc;
      rc.left=pt.x-2;
      rc.top=pt.y-2;
      rc.right=pt.x+2;
      rc.bottom=pt.y+2;
      InvalidateRect(&rc,TRUE);
   }

   enum ELineToDraw { none, edge, intersect };
	enum { IDD = IDD_CCYRUSBECKDLG };

BEGIN_MSG_MAP(CCyrusBeckDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDC_RESET, OnReset)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 1;  // Let the system set the focus
	}

	LRESULT OnLButtonDown(UINT /* nMsg */, WPARAM /* wParam */,	LPARAM /* lParam */, BOOL& /* lResult */);
   LRESULT OnLButtonUp(UINT /* nMsg */, WPARAM /* wParam */,	LPARAM /* lParam */, BOOL& /* lResult */);

   LRESULT OnReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
      m_LineType=none;
      m_bInDrawMode=false;
      m_bHasEdge=false;
      m_bHasIntersect=false;
      m_edgeStart.x = 0;
      m_edgeStart.y = 0;
      m_edgeEnd.x = 0;
      m_edgeEnd.y = 0;
      m_intersectStart.x = 0;
      m_intersectStart.y = 0;
      m_intersectEnd.x = 0;
      m_intersectEnd.y = 0;
      m_intersectPt.x=0;
      m_intersectPt.y=0;
      Invalidate(TRUE);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

   LRESULT OnPaint(UINT /* nMsg */, WPARAM /* wParam */,	LPARAM /* lParam */, BOOL& /* lResult */)
   {
	   PAINTSTRUCT ps;

	   HDC hdc = ::BeginPaint(m_hWnd, &ps);
   
	   RECT rc;
	   GetClientRect(&rc);

      if(m_bHasEdge==true)
      {
         MoveToEx(hdc, m_edgeStart.x, m_edgeStart.y, 0);
         LineTo(hdc, m_edgeEnd.x, m_edgeEnd.y);
      }

      if(m_bHasIntersect==true)
      {
         MoveToEx(hdc, m_intersectStart.x, m_intersectStart.y, 0);
         LineTo(hdc, m_intersectEnd.x, m_intersectEnd.y);
      }

      if(m_intersectPt.x!=0&&m_intersectPt.y!=0)
      {
         Ellipse(hdc,m_intersectPt.x-2,m_intersectPt.y-2,m_intersectPt.x+2,m_intersectPt.y+2);
      }

      ::EndPaint(m_hWnd, &ps);
	   
      return 0;
   }

   LRESULT OnMouseMove(UINT /* nMsg */, WPARAM /* wParam */,	LPARAM /* lParam */, BOOL& /* lResult */);

private:
   bool m_bHasEdge;
   POINT m_edgeStart;
   POINT m_edgeEnd;
   bool m_bHasIntersect;
   POINT m_intersectStart;
   POINT m_intersectEnd;
   POINT m_intersectPt;
   ELineToDraw m_LineType;
   HCURSOR m_hOldCuror;
   bool m_bInDrawMode;
};

#endif //__CYRUSBECKDLG_H_
