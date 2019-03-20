// CViewFustrum.h: interface for the CViewFustrum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CViewFustrum_H__F798F9B8_E5E5_11D2_80FE_006097A05989__INCLUDED_)
#define CViewFustrum_H__F798F9B8_E5E5_11D2_80FE_006097A05989__INCLUDED_

#include "..\primatives\import.h"

class CPolygon;

#define m_pFront  m_pWalls[0]
#define m_pBack   m_pWalls[1]
#define m_pTop    m_pWalls[2]
#define m_pBottom m_pWalls[3]
#define m_pLeft   m_pWalls[4]
#define m_pRight  m_pWalls[5]

class PRIMATIVES_EXPORT CViewFustrum  
{
public:
	CViewFustrum();
	virtual ~CViewFustrum();

   void init( double fDistance, double dNear = 0, double dFar = 0, int horz = 0, int vert = 0 );

   const CPolygon & getWall( int i ) const { return *m_pWalls[i]; }
   CPolygon const * getFront() const { return m_pWalls[0]; }
   CPolygon const * getBack() const { return m_pWalls[1]; }
   CPolygon const * getTop() const { return m_pWalls[2]; }
   CPolygon const * getBottom() const { return m_pWalls[3]; }
   CPolygon const * getLeft() const { return m_pWalls[4]; }
   CPolygon const * getRight() const { return m_pWalls[5]; }
   
   void transform( const CMatrix & m );

private:
   CPolygon * m_pWalls[6];

   friend PRIMATIVES_EXPORT ostream& operator << ( ostream& os, const CViewFustrum & f );
};

#endif // !defined(CViewFustrum_H__F798F9B8_E5E5_11D2_80FE_006097A05989__INCLUDED_)
