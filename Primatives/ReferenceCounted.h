// REFERENCECOUNTED.h: interface for the CREFERENCECOUNTED class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_REFERENCECOUNTED_H__E2CFC026_CEDF_11D1_A27F_006097A05989__INCLUDED_)
#define _REFERENCECOUNTED_H__E2CFC026_CEDF_11D1_A27F_006097A05989__INCLUDED_

#include "..\primatives\import.h"

class PRIMATIVES_EXPORT CReferenceCounted
{
public:
   CReferenceCounted()
      :m_ulReferenceCount( 1 )
   {
   }

public:
   unsigned long AddRef()
   {
      return ++m_ulReferenceCount;
   }

   void NoRef()
   {
      m_ulReferenceCount = 0;
   }

   unsigned long Release()
   {
      unsigned long ulRet = --m_ulReferenceCount;
      if ( m_ulReferenceCount <= 0 )
      {
         delete this;
      }
      return ulRet;
   }

private:
   unsigned long  m_ulReferenceCount;
};

#endif
