// =====================================================================================
// 
//       Filename:  SednaDBInterface.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  19/10/09 16:40:43
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  SEDNADBINTERFACE_CLASS_INC
#define  SEDNADBINTERFACE_CLASS_INC

#include <string>
#include "libsedna.h"

namespace GLOOPER_TEST
{
   class SednaDBInterface
   {
   public:

      SednaDBInterface(const std::string&, 
	    const std::string&, const std::string&,
	    const std::string& = "",
	    const bool = true);

      void open_connection();

      void set_autocommit(const bool);

      void begin_transaction();

      void commit_transaction();

      void close_connection();

      void insert(const std::string&, const std::string&);

      void send_query(const std::string&);

      const std::string default_context() const; 

   private:

      SednaConnection conn;

      std::string db_name;

      std::string db_user;

      std::string db_user_pwd;

      std::string default_root_node;

   };
}

#endif   // ----- #ifndef SEDNADBINTERFACE_CLASS_INC  -----
