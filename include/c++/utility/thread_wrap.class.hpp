// =====================================================================================
// 
//       Filename:  thread_wrap.class.hpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  13/08/10 13:32:30
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  THREAD_WRAP_CLASS_INC
#define  THREAD_WRAP_CLASS_INC

#include "boost/thread.hpp"

namespace GLOOPER_TEST
{
   template<class C> class thread_wrap
   {
   public:

      thread_wrap(const C& prototype):
	 prototype(prototype)
      {}

      operator C&()
      {
	 if(holder.get() == 0)
	    holder.reset(new C(prototype));
	 return *holder;
      }

      operator const C&() const
      {
	 if(holder.get() == 0)
	    holder.reset(new C(prototype));
	 return *holder;
      }

      void reset(const C& new_value)
      {
	 holder.reset(new C(new_value));
      }

   protected:

      C prototype;

      mutable boost::thread_specific_ptr<C> holder;

   private:

      thread_wrap();
   };
}

#endif   // ----- #ifndef THREAD_WRAP_CLASS_INC  -----
