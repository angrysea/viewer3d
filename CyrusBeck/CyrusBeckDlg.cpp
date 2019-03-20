// CCyrusBeckDlg.cpp : Implementation of CCCyrusBeckDlg
#include "stdafx.h"
#include "CyrusBeckDlg.h"
#include "..\primatives\Vector.h"

/////////////////////////////////////////////////////////////////////////////
// CCyrusBeckDlg
LRESULT CCyrusBeckDlg::OnLButtonDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& lResult)
{
   POINT startPt, endPt;
   switch(m_LineType)
   {
   case none:
      m_LineType=edge;

   case edge:
      if(m_bHasEdge!=true)
      {
         startPt.x=m_edgeStart.x;
         startPt.y=m_edgeStart.y;
         endPt.x=m_edgeEnd.x;
         endPt.y=m_edgeEnd.y;
         m_edgeStart.x=0;
         m_edgeStart.y=0;
         m_edgeEnd.x=0;
         m_edgeEnd.y=0;
         drawLine(startPt, endPt);
         m_edgeStart.x = LOWORD(lParam);
         m_edgeStart.y = HIWORD(lParam);
         m_bHasEdge=true;
         break;
      }
      m_LineType=intersect;

   case intersect:
      startPt.x=m_intersectStart.x;
      startPt.y=m_intersectStart.y;
      endPt.x=m_intersectEnd.x;
      endPt.y=m_intersectEnd.y;
      m_intersectStart.x=0;
      m_intersectStart.y=0;
      m_intersectEnd.x=0;
      m_intersectEnd.y=0;
      drawLine(startPt, endPt);

      m_intersectStart.x = LOWORD(lParam);
      m_intersectStart.y = HIWORD(lParam);
      m_bHasIntersect=true;
      m_LineType=intersect;
      break;

   default:
      return 0l;
   }

   m_bInDrawMode=true;
   m_hOldCuror = SetCursor(LoadCursor(NULL, IDC_SIZEALL));
   SetCapture();

   return 0l;
}

LRESULT CCyrusBeckDlg::OnLButtonUp(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& lResult)
{
   if(m_bInDrawMode==true)
   {
      switch(m_LineType)
      {
      case edge:
         m_edgeEnd.x = LOWORD(lParam);
         m_edgeEnd.y = HIWORD(lParam);
         drawLine(m_edgeStart, m_edgeEnd);
         break;

      case intersect:
         m_intersectEnd.x = LOWORD(lParam);
         m_intersectEnd.y = HIWORD(lParam);
         drawLine(m_intersectStart, m_intersectEnd);
         compute();
         break;

      case none:
      default:
         return 0l;
      }

      m_LineType=none;
      m_bInDrawMode=false;
      m_hOldCuror = SetCursor(m_hOldCuror);
      ReleaseCapture();
   }
   
   return 0l;
}

LRESULT CCyrusBeckDlg::OnMouseMove(UINT /* nMsg */, WPARAM /* wParam */, LPARAM lParam, BOOL& /*lResult*/ )
{
   POINT endPt;
   if(m_bInDrawMode==true)
   {
      switch(m_LineType)
      {
      case edge:
         endPt.x=m_edgeEnd.x;
         endPt.y=m_edgeEnd.y;
         m_edgeEnd.x = LOWORD(lParam);
         m_edgeEnd.y = HIWORD(lParam);
         drawLine(m_edgeStart, endPt);
         drawLine(m_edgeStart, m_edgeEnd);
         break;

      case intersect:
         endPt.x=m_intersectEnd.x;
         endPt.y=m_intersectEnd.y;
         m_intersectEnd.x = LOWORD(lParam);
         m_intersectEnd.y = HIWORD(lParam);
         drawLine(m_intersectStart, endPt);
         drawLine(m_intersectStart, m_intersectEnd);
         break;

      case none:
      default:
         return 0l;
      }
   }

   return 0l;
}

void CCyrusBeckDlg::compute()
{
   drawCircle(m_intersectPt);

   CVector Pei(m_edgeStart.x,m_edgeStart.y,1);
   CVector endEdge(m_edgeEnd.x,m_edgeEnd.y,1);

   CVector P0(m_intersectStart.x,m_intersectStart.y,1);
   CVector P1(m_intersectEnd.x,m_intersectEnd.y,1);

   CVector Ni;
   Ni.crossProduct(Pei, endEdge);
   Ni.normalize();

   double locP0 = Ni.dotProduct(P0-Pei);
   double locP1 = Ni.dotProduct(P1-Pei);

   if(locP0<EPSILON&&locP1<EPSILON)
   {
      SetDlgItemText(IDC_LOCATION, "Inside");
   }
   else if(locP0>EPSILON&&locP1>EPSILON)
   {
      SetDlgItemText(IDC_LOCATION, "Outside");
   }
   else
   {
      SetDlgItemText(IDC_LOCATION, "Intersecting");
   }

   CVector Pt;

   CVector negNi;
   negNi=Ni*-1;
   Pt=P0+((P1-P0)*(Ni.dotProduct(P0-Pei)/negNi.dotProduct(P1-P0)));

   m_intersectPt.x=Pt.m_x;
   m_intersectPt.y=Pt.m_y;
   
   drawCircle(m_intersectPt);
}


