// =====================================================================================
// 
//       Filename:  table_db.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/08/10 14:44:45
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "db/TableDBInterface.class.h"
#include "agent/LuaComplexAgent.class.h"
#include "trading/Trade.class.h"
#include "trading/Order.class.h"
#include "info/SimpleInfoGenerator.class.h"
#include "callback_log/LOG.h"
#include "callback_log/FileCallbackLog.class.h"
#include "boost/assign.hpp"
#include <iostream>
#include <string>
#include <exception>

using namespace std;
using namespace GLOOPER_TEST;
using namespace boost::assign;

int main(int argc, char** argv)
{
   try
   {
      if(argc < 3) throw "At least 2 command-line arguments must be given";

      FileCallbackLog F("tdb.log","root",TRACE);

      const char* cfg_file = argv[1];
      const char* outdir = argv[2];

      TableDBInterface tdb(cfg_file,outdir);

      tdb.init(list_of("test_id"),"test");

      tdb.transaction_begin(list_of("0"));

      Trade tr(4500,100.56,0,1);

      tdb.process(tr);

      tdb.process(lca);

      tdb.transaction_end();

   }
   catch(const char* c)
   {
      cerr << "Exception: " << c << endl;
      return EXIT_FAILURE;
   }
   catch(const std::string& s)
   {
      cerr << "Exception: " << s << endl;
      return EXIT_FAILURE;
   }
   catch(const std::exception& e)
   {
      cerr << "Exception: " << e.what() << endl;
      return EXIT_FAILURE;
   }
   catch(...)
   {
      cerr << "Exception...\n";
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
