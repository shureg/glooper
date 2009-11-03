// =====================================================================================
// 
//       Filename:  Position.struct.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  15/06/09 20:58:12
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "trading/Position.struct.h"

using namespace GLOOPER_TEST;

Position::Position(): q(0), is_long(boost::logic::indeterminate)
{}

Position::Position(double _q)
{
   if(_q > 0)
      is_long = true;
   else if(_q < 0)
      is_long = false;
   else
      is_long = boost::logic::indeterminate;

   q = (unsigned long) std::fabs(_q);
}

Position::operator double() const
{
   return (double)(sign()) * (double)(q);
}

bool Position::empty() const
{
   return boost::logic::indeterminate(is_long);
}

Position& Position::operator += (unsigned long dq)
{
   if(is_long)
      q += dq;
   else if(!is_long)
   {
      unsigned long dq_min = std::min(q,dq);
      unsigned long dq_exc = std::max(dq-q,0ul);
      q -= dq_min;
      if(q==0)
      {
	 if(dq_exc==0)
	    is_long = boost::logic::indeterminate;
	 else
	 {
	    q += dq_exc;
	    is_long = true;
	 }
      }
   }
   else
   {
      q += dq;
      is_long = true;
   }

   return *this;
}

Position& Position::operator -= (unsigned long dq)
{
   if(is_long)
   {
      unsigned long dq_min = std::min(q,dq);
      unsigned long dq_exc = std::max(dq-q,0ul);
      q -= dq_min;
      if(q==0)
      {
	 if(dq_exc==0)
	    is_long = boost::logic::indeterminate;
	 else
	 {
	    q += dq_exc;
	    is_long = false;
	 }
      }
   }
   else if(!is_long)
      q += dq;
   else
   {
      q += dq;
      is_long = false;
   }

   return *this;
}

int Position::sign() const
{
   if(is_long) return 1;
   else if(!is_long) return -1;
   else return 0;
}
