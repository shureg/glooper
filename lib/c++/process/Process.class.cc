// =====================================================================================
// 
//       Filename:  Process.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  13/10/09 14:13:39
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "process/Process.class.h"

using GLOOPER_TEST::Process;

Process* Process::clone() const
{
   return real_clone();
}
