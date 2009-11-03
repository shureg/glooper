// =====================================================================================
// 
//       Filename:  SednaDBInterface.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  19/10/09 16:40:49
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "db/SednaDBInterface.class.h"
#include "callback_log/LOG.h"

using namespace GLOOPER_TEST;
using namespace std;
using namespace CALLBACK_LOG;

SednaDBInterface::SednaDBInterface(const string& _db_name, 
      const string& _db_user, const string& _db_user_pwd, const string&
      _default_root_node):
   db_name(_db_name),
   db_user(_db_user), db_user_pwd(_db_user_pwd),
   default_root_node(_default_root_node)
{
   struct SednaConnection tmp = SEDNA_CONNECTION_INITIALIZER;
   conn = tmp;
}

void SednaDBInterface::open_connection()
{
   int result = SEconnect(&conn,"localhost",db_name.c_str(),db_user.c_str(),
	 db_user_pwd.c_str());

   if(result != SEDNA_SESSION_OPEN)
      LOG(EXCEPTION,boost::format("Connection to Sedna DB %s for user %s "\
	       "could not be opened, with exit code %d and error message: %s\n")
	    % db_name % db_user % result % SEgetLastErrorMsg(&conn));
}

void SednaDBInterface::close_connection()
{
   int result = SEclose(&conn);
   
   if(result != SEDNA_SESSION_CLOSED)
      LOG(EXCEPTION,boost::format("Connection to Sedna DB %s for user %s "\
	       "could not be closed, with exit code %d and error message: %s\n")
	    % db_name % db_user % result % SEgetLastErrorMsg(&conn));
}

void SednaDBInterface::send_query(const string& query_string)
{
   int result = SEexecute(&conn,query_string.c_str());

   if(result < 0)
   {
      const char* raw_error_msg = SEgetLastErrorMsg(&conn);
      string error_msg;
      if(raw_error_msg == 0)
	 error_msg = "(unspecified)";
      else
      {
	 try
	 {
	    error_msg = (string) raw_error_msg;
	 }
	 catch(...)
	 {
	    LOG(EXCEPTION,boost::format("Sedna error message could not be "\
		     "converted to std::string\n"));
	 }
      }

      LOG(EXCEPTION,boost::format("Query string %s sent to Sedna DB %s "\
	       "for user %s "\
	       "could not be executed, with exit code %d and error message: %s\n")
	    % query_string % db_name % db_user % result % error_msg);
   }
}

void SednaDBInterface::insert(const string& value, const string& context)
{
   string insert_query = (boost::format("UPDATE\ninsert (\n%s) into %s")
      % value % context).str();

   send_query(insert_query);
}

const std::string SednaDBInterface::default_context() const
{
   boost::format tmp = 
      boost::format("doc(\"%s\")/%s") % db_name % default_root_node;

   return tmp.str();
}
