// =====================================================================================
// 
//       Filename:  Position.struct.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/06/09 11:10:32
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================


#ifndef  _GLT_POSITION_STRUCT_INC
#define  _GLT_POSITION_STRUCT_INC

#include <algorithm>
#include <cmath>

#include "boost/logic/tribool.hpp"

namespace GLOOPER_TEST
{
   struct Position
   {
      unsigned long q;

      boost::logic::tribool is_long;

      Position& operator += (unsigned long);

      Position& operator -= (unsigned long);

      int sign() const;

      Position();

      Position(double);

      operator double () const;

      bool empty() const;

   };

}

#endif   // ----- #ifndef _GLT_POSITION_STRUCT_INC  -----
