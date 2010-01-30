// =====================================================================================
// 
//       Filename:  version.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  20/11/09 14:55:18
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  VERSION_INC
#define  VERSION_INC

#define LIB_VERSION 6.3.1

#define LIB_VERSION_STR TO_STR(LIB_VERSION)

#define TO_STR(s) TO_STR_HIDDEN(s)

#define TO_STR_HIDDEN(s) #s

#endif   // ----- #ifndef VERSION_INC  -----
