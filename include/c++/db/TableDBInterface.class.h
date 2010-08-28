// =====================================================================================
// 
//       Filename:  TableDBInterface.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/08/10 18:07:25
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  TABLEDBINTERFACE_CLASS_INC
#define  TABLEDBINTERFACE_CLASS_INC

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "xml_serialisation/XmlSerialisableObject.class.h"

#include <list>
#include <string>
#include <set>
#include <map>
#include <iostream>
#include "boost/ptr_container/ptr_map.hpp"

namespace GLOOPER_TEST
{
   class TableDBInterface
   {
   public:

      struct xpath_info
      {
	 std::list<std::string> path;
	 std::string table_id;
	 std::string context_label;

	 xpath_info(
	       const std::list<std::string>& path,
	       const std::string& table_id,
	       const std::string& context_label = ""):
	    path(path), table_id(table_id),
	    context_label(context_label){}
      };

      class outfile_ptr
      {
      public:

	 outfile_ptr(){}

	 outfile_ptr(const outfile_ptr&);

	 void assign(const std::string&);

	 void close();

	 std::ofstream& get();

      private:

	 boost::shared_ptr<std::ofstream> ofs;

      };

      TableDBInterface(const char* lua_cfg_file, const char* outdir);

      ~TableDBInterface();

      void init(const std::vector<std::string>&, const std::string&);

      void process(const XML_SERIALISATION::XmlField& x);

      void transaction_begin(const std::vector<std::string>&);

      void transaction_end();

      std::vector<std::string>& get_external_fields();

      void print(std::ostream&) const;

   protected:

      const char* lua_cfg_file;

      lua_State* L;

      std::string outdir;

      bool transaction;

      std::map<std::string, std::string> header_rows;

      std::multimap<std::string, xpath_info > contexts;

      typedef std::multimap<std::string, xpath_info> cont_dict;

      std::set< std::string > row_fields;

      std::map< std::string, 
	 std::vector< size_t  > > 
	    memoized_header_pos;

      typedef std::vector< size_t  >
	 header_pos;

      typedef std::map< std::string, header_pos > header_pos_map;

      std::map<std::string, std::vector<std::string> > input_headers;

      std::vector<std::string> external_fields;

      std::map<std::string, outfile_ptr> outfiles;

      void process_row_data(const XML_SERIALISATION::XmlField&, 
	    const std::string&, const std::string& = "");

      void record_value_row(const std::vector<std::string>&, 
	    const std::string&);
   };
}

#endif   // ----- #ifndef TABLEDBINTERFACE_CLASS_INC  -----
