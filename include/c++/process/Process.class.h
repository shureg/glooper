// =====================================================================================
// 
//       Filename:  Process.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  13/10/09 14:13:32
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  PROCESS_CLASS_INC
#define  PROCESS_CLASS_INC

namespace GLOOPER_TEST
{
   class Process
   {
   public:

      virtual void evolve() = 0;

      virtual void simulation_config() = 0;

      virtual void batch_config() = 0;

      virtual void run_config() = 0;

      virtual void thread_config() = 0;

      Process* clone() const;

      Process* thread_space_clone() const;

      virtual ~Process(){}

   protected:

      virtual Process* real_clone() const = 0;
   };
}

#endif   // ----- #ifndef PROCESS_CLASS_INC  -----
