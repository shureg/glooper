// =====================================================================================
// 
//       Filename:  TableDBInterface.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/08/10 18:09:45
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "db/TableDBInterface.class.h"
#include "boost/lambda/bind.hpp"
#include "boost/lambda/lambda.hpp"
#include "boost/foreach.hpp"
#include "boost/make_shared.hpp"
#include <algorithm>
#include <fstream>
#include <utility>

using GLOOPER_TEST::TableDBInterface;

using namespace XML_SERIALISATION;

using namespace std;

TableDBInterface::outfile_ptr::outfile_ptr(const outfile_ptr& ofp)
{
   boost::shared_ptr<ofstream> tmp;
   ofs = tmp;
}

void TableDBInterface::outfile_ptr::assign(const std::string& fname)
{
   using boost::make_shared;

   if(ofs.get() == 0)
      ofs = make_shared<ofstream>(fname.c_str());
   else
   {
      ofs->close();
      ofs->open(fname.c_str());
   }
}

void TableDBInterface::outfile_ptr::close()
{
   if(ofs.get() == 0)
      LOG(WARNING, boost::format(
	       "TableDBInterface::outfile_ptr::close() - "\
	       "ofstream at %0x has not been initialised\n")
	    % this
	 );
   else
      ofs->close();
}

ofstream& TableDBInterface::outfile_ptr::get()
{
   if(ofs.get() == 0)
      LOG(EXCEPTION, boost::format(
	       "TableDBInterface::outfile_ptr::get() - "\
	       "ofstream at %0x has not been initialised\n")
	    % this
	    );
   else
      return *ofs;
}

TableDBInterface::TableDBInterface(
      const char* lua_cfg_file, const char* outdir):
   lua_cfg_file(lua_cfg_file), outdir(outdir), transaction(false)
{}

TableDBInterface::~TableDBInterface()
{
/*   for(map<string,ofstream*>::iterator 
	 f = outfiles.begin(); f != outfiles.end(); ++f)
   {
      f->second->close();
      delete f->second;
   }*/
}

/*
TableDBInterface::TableDBInterface(const TableDBInterface& tdb)
{
   lua_cfg_file = tdb.lua_cfg_file;
   
   L = tdb.L;

   outdir = tdb.outdir;

   transaction = tdb.transaction;

   header_rows = tdb.header_rows;

   contexts = tdb.contexts;

   row_fields = tdb.row_fields;

   memoized_header_pos = tdb.memoized_header_pos;

   input_headers = tdb.input_headers;

   external_fields = tdb.external_fields;

   outfiles.clear();

   for(map<string,ofstream*>::const_iterator
	 of = tdb.outfiles.begin(); of != tdb.outfiles.end(); ++of)
      outfiles.insert(pair<string,ofstream*>(
	       of->first, new ofstream)
	    );
}
*/

void TableDBInterface::init(const vector<string>& external_headers,
      const string& external_dbid)
{
   //Process external headers and set up external fields structure
   //to reflect the number of external headers passed in
   
   LOG(TRACE, boost::format(
	    "TableDBInterface::init - "
	    "initialising object at %0x from file %s\n")
	 % this % lua_cfg_file
	 );

   string ext_hdrs;
   ext_hdrs.clear();

   for( vector<string>::const_iterator
	 eh = external_headers.begin(); eh != external_headers.end(); ++eh)
      ext_hdrs += ( (*eh) + string(",") );

   external_fields.resize(external_headers.size(),"");

   // Begin processing the Lua config file
   L = luaL_newstate();
   luaL_openlibs(L);

   if( luaL_dofile(L, lua_cfg_file) != 0 )
      LOG(EXCEPTION, boost::format(
	       "TableDBInterface::init - cannot open Lua config file %s "\
	       "- Lua error: %s")
	    % lua_cfg_file % (lua_tostring(L,-1))
	    );

   lua_getglobal(L,"tables");

   size_t N_tbl = lua_objlen(L,-1);

   size_t N_ctxt, N_row;

   string table_id, field_name, header_row, in_header, out_header, 
	  context_lbl, hdr_outfile;

   bool has_context;

   map< string, vector<string> >::iterator hdr_i;

   list<string> xpath;

   //start iterating through the top-most table
   for(size_t t_ix = 1; t_ix <= N_tbl; ++t_ix)
   {
      lua_rawgeti(L,-1,t_ix);
      if( !lua_istable(L,-1) )
	 LOG(EXCEPTION, boost::format(
		  "%s: Entry tables[%d] is not a table\n")
	       % lua_cfg_file % t_ix
	       );
      
      //Get the abbreviated table id
      lua_pushstring(L,"table_id");
      lua_rawget(L,-2);
      table_id = lua_tostring(L,-1);
      lua_pop(L,1);

      //Get the data row field name
      lua_pushstring(L,"field_name");
      lua_rawget(L,-2);
      field_name = lua_tostring(L,-1);
      lua_pop(L,1);
      row_fields.insert(field_name);

      //Process the headers field
      lua_pushstring(L,"headers");
      lua_rawget(L,-2);
      N_row = lua_objlen(L,-1);

      header_row.clear();
      hdr_i = input_headers.insert(input_headers.begin(),
	    pair< string, vector<string> >(
	       table_id,vector<string>(N_row) )
	    );

      //Go through the headers elements
      for(size_t h_ix = 1; h_ix <= N_row; ++h_ix)
      {
	 lua_rawgeti(L,-1,h_ix);
	 // If the header element is a string set both header names to its
	 // value
	 if ( lua_isstring(L,-1) )
	 {
	    out_header = lua_tostring(L,-1);
	    in_header = out_header;
	 }
	 // If it is a table, set the output header to the first element,
	 // and the input header to the second
	 else
	 {
	    lua_rawgeti(L,-1,1);
	    out_header = lua_tostring(L,-1);
	    lua_pop(L,1);

	    lua_rawgeti(L,-1,2);
	    in_header = lua_tostring(L,-1);
	    lua_pop(L,1);
	 }

	 header_row += out_header;
	 if(h_ix < N_row)
	    header_row += string(",");

	 hdr_i->second[h_ix-1] = in_header;

	 lua_pop(L,1); //pop the header element
      }

      header_rows.insert(pair<string,string>( table_id, header_row )
	    );

      lua_pop(L,1); //pop the headers table

      has_context = false;

      // Process the contexts table
      lua_pushstring(L,"contexts");
      lua_rawget(L,-2);

      // If contexts table does not exist, populate the default context

      xpath.clear();

      if( lua_isnil(L,-1) )
      {
	 xpath.push_back(field_name);
	 contexts.insert(pair<string, xpath_info>( 
		  field_name, xpath_info(xpath, table_id))
	       );
      }
      else
      {
	 has_context = true;

	 N_ctxt = lua_objlen(L,-1);

	 // Go through the context elements
	 for(size_t c_ix = 1; c_ix <= N_ctxt; ++c_ix)
	 {
	    xpath.clear();

	    lua_rawgeti(L,-1,c_ix);

	    // If the context element is a string, assume it is default
	    // context label
	    if( lua_isstring(L,-1) )
	       context_lbl = lua_tostring(L,-1);
	    // If it is a table, proceed to extract context and label fields
	    else
	    {
	       lua_pushstring(L,"label");
	       lua_rawget(L,-2);
	       context_lbl = lua_tostring(L,-1);
	       lua_pop(L,1); //pop the label field value

	       lua_pushstring(L,"container");
	       lua_rawget(L,-2);

	       //Determine whether the context field is single value 
	       //or a table and proceed accordingly
	       if(lua_isstring(L,-1))
		  xpath.push_back(lua_tostring(L,-1));
	       else
	       {
		  for(size_t xp_ix = 1; xp_ix <= lua_objlen(L,-1); ++xp_ix)
		  {
		     lua_rawgeti(L,-1,xp_ix);
		     xpath.push_back(lua_tostring(L,-1));
		     lua_pop(L,1); //pop the context path element
		  }
	       }

	       lua_pop(L,1); //pop the context field value

	    }

	    lua_pop(L,1); //pop the contexts table elements

	    xpath.push_back(field_name);

	    contexts.insert(pair<string, xpath_info>(
		     xpath.front(), xpath_info(xpath,table_id,context_lbl) )
		  );
	 }
      }

      lua_pop(L,1); //pop the contexts table

      // add the table_id - filestream pair to the corresponding mapping
      outfiles.insert(pair<string,outfile_ptr>(table_id, outfile_ptr()));

      // record the header file for the current table_id
      hdr_outfile = (boost::format("%s/%s/%s.%s.header.csv")
	    % outdir % table_id % table_id % external_dbid).str();

      LOG(TRACE,boost::format("Writing header file for table %s to %s\n")
	    % table_id % hdr_outfile);

      if(has_context) header_row += string(",context");

      ofstream hfs(hdr_outfile.c_str());
      hfs << ext_hdrs << header_row << endl;
      hfs.close();

      lua_pop(L,1); //pop the table element

   }

   lua_pop(L,1); //pop the tables table

   lua_close(L);
}

void TableDBInterface::transaction_begin(
      const vector<string>& transaction_external_fields)
{
   LOG(TRACE,boost::format(
	    "TableDBInterface::transaction_begin() - "\
	    "Beginning transaction with external fields vector size %d\n")
	 % (transaction_external_fields.size())
	 );

   string outfile_stem;
   outfile_stem.clear();

   for(size_t i = 0; i < transaction_external_fields.size(); ++i)
   {
      external_fields[i] = transaction_external_fields[i];
      outfile_stem += ( external_fields[i] + string(".") );
   }

   LOG(TRACE,boost::format(
	    "TableDBInterface::transaction_begin() - "\
	    "Using output filestem %s\n") % outfile_stem
	 );

   std::string outfile;

   for(map<string,outfile_ptr>::iterator
	 f = outfiles.begin(); f != outfiles.end(); ++f)
   {
      outfile = (boost::format("%s/%s/%s.%scsv") 
	    % outdir % (f->first) % (f->first) % outfile_stem).str();
      f->second.close();
      
      LOG(TRACE,boost::format(
	    "TableDBInterface::transaction_begin() - "\
	    "Opening file stream for file %s at address %0x "\
	    "in object %0x\n")
	    % outfile % &(f->second) % this
	    );
      f->second.assign(outfile);

      if(! f->second.get().good() )
	 LOG(WARNING,boost::format(
	    "TableDBInterface::transaction_begin() - "\
	    "Something is wrong with ofstream at address %0x "\
	    "in object %0x\n")
	       % &(f->second) % this
	       );
   }

   transaction = true;
}

void TableDBInterface::transaction_end()
{
   LOG(TRACE,boost::format(
	    "TableDBInterface::transaction_end() - "\
	    "ending transaction\n")
	 );

   for(map<string,outfile_ptr>::iterator
	 f = outfiles.begin(); f != outfiles.end(); ++f)
   {
      if(! f->second.get().good() )
	 LOG(WARNING,boost::format(
	    "TableDBInterface::transaction_end() - "\
	    "Something is wrong with ofstream at address %0x "\
	    "in object %0x\n")
	       % &(f->second) % this
	       );

      f->second.close();

      LOG(TRACE,boost::format(
	    "TableDBInterface::transaction_end() - "\
	    "Close filestream at address %0x "\
	    "in object %0x\n")
	     % &(f->second) % this
	    );
   }
   transaction = false;
}

void TableDBInterface::process(const XmlField& x)
{
   string top_name = x.get_name();

   const XmlField* leaf = 0;
   const XmlField* cont = 0;

   // Get all context paths with top level name matching the current
   // field name
   pair<cont_dict::const_iterator, cont_dict::const_iterator>
      candidates = contexts.equal_range(top_name);

   // Go through all context path candidates
   for( cont_dict::const_iterator
	 xpi = candidates.first; xpi != candidates.second; ++xpi)
   {
      //Check whether the current field is in a context's top level
      if( xpi->second.path.front() == top_name )
	 // Check whether the top_most field is in fact a data row field
	 // If so, process it
	 if( row_fields.count( top_name ) > 0 )
	    process_row_data(x,
		  xpi->second.table_id, xpi->second.context_label);
	 // Otherwise validate the path candidate. Upon success process the
	 // data row field(s) at the end of the path
	 else
	 {
	    cont = 0;
	    leaf = 0;

	    list<string>::const_iterator sub_path_begin =
	       ++(xpi->second.path.begin() );
	    list<string>::const_iterator sub_path_end =
	       --(xpi->second.path.end());

	    if(sub_path_begin == sub_path_end)
	       cont = &x;
	    else
	       cont = x.validate_path(sub_path_begin,sub_path_end);
	    
	    if( cont != 0 )
	    {
	       leaf = cont->validate_path(sub_path_end,
		     xpi->second.path.end());

	       if( leaf != 0 )
	       {
		  pair<XmlField::const_field_iterator, 
		     XmlField::const_field_iterator> data_row_range =
			cont->get_field_range(leaf->get_name());

		  typedef pair<string, XmlField> fld_pair;

		  BOOST_FOREACH( 
			const fld_pair& fld, data_row_range)
		  {
		     process_row_data(fld.second,
			   xpi->second.table_id, 
			   xpi->second.context_label);
		  }
		  
		  /*
		  std::for_each(data_row_range.first, data_row_range.second,
			bind(&TableDBInterface::process_row_data, this,
			   cref( ret<XmlField>(bind(
				    &pair<string,XmlField>::second,_1)) ),
			   cref(xpi->second.table_id),
			   cref(xpi->second.context_label) )
			);*/
	       }
	    }
	 }
   }

}

vector<string>& TableDBInterface::get_external_fields()
{
   return external_fields;
}

void TableDBInterface::process_row_data(
      const XmlField& f, 
      const std::string& table_id, const std::string& context_label)
{
   string field_name = f.get_name();

   string field_type;

   if( f.field_exists("type") )
      field_type = f["type"];
   else
      field_type = field_name;

   LOG(TRACE, boost::format(
	    "TableDBinterface::process_row_data: "\
	    "starting to process field %s for table %s "\
	    "in context \"%s\"\n")
	 % field_type % table_id % context_label
	 );

   const XmlField::attribute_map& attributes = f.get_attributes();

   //Find the corresponding row header in the raw headers dictionary
   map<string, vector<string> >::iterator this_header = 
      input_headers.find(table_id);

   if( this_header == input_headers.end() )
      LOG(EXCEPTION, boost::format(
	       "TableDBInterface::process_row_data - could not find "\
	       "header set with table_id %s in the input headers map\n")
	    % table_id
	    );

   header_pos_map::iterator where =
      memoized_header_pos.find(table_id);

   //If a memoized header order does not exist for the field label,
   //create it
   if (where == memoized_header_pos.end() )
   {
      //Set the memoized order position to the newly created member
      vector<size_t> dummy;
      where = memoized_header_pos.insert( memoized_header_pos.begin(),
	    header_pos_map::value_type
	    (table_id, dummy)
	    );

      //Go through the attributes of a field one by one
      //and record their position (relative to the header beginning)
      //in the appropriate memoizing structure
      for(XmlField::attribute_map::const_iterator
	    a = attributes.begin(); a != attributes.end(); ++a)
      {
	 vector<string>::const_iterator pos =
	    find(this_header->second.begin(), this_header->second.end(),
		  a->first);

	 if(pos == this_header->second.end())
	    LOG(EXCEPTION, boost::format(
		     "TableDBInterface::process_row_data - could not "\
		     "find attribute %s in header table %s\n")
		  % (a->first) % (this_header->first)
		  );

	 where->second.push_back(
	       pos - this_header->second.begin()
	       );
      }
   }

   //Having obtained the position of a memoized header order mapping
   //pre-allocate a vector to hold the values of the attributes in
   //the required order
   vector<string> values( this_header->second.size(), "" );

   //Go through the attributes one by one and through the memoized
   //header row positions (both containers should have the same
   //number of elements) and assign each attributes value to
   //a position in the value vector corresponding to attribute's position
   //in the memoized header mapping
   XmlField::attribute_map::const_iterator a;
   vector<size_t>::const_iterator i;
   for(
	 a = attributes.begin(),
	 i = where->second.begin();
	 a != attributes.end(); ++a, ++i)
   {
      values[*i] = a->second;
   }

   if(context_label != "" and !context_label.empty())
      values.push_back(context_label);

   record_value_row(values,table_id);
}

void TableDBInterface::record_value_row(const vector<string>& values,
      const string& table_id)
{
   map<string,outfile_ptr>::iterator where = outfiles.find(table_id);
   vector<string>::const_iterator last_val = values.end()-1;

   using boost::lambda::bind;
   using boost::lambda::_1;
   using boost::lambda::constant;

   LOG(TRACE, boost::format(
	    "TableDBInterface::record_value_row - "\
	    "recording row of table %s to filestream %0x "\
	    "in object %0x\n")
	 % table_id % (&(where->second)) % this
	 );

   std::for_each(external_fields.begin(), external_fields.end(),
	 where->second.get() << boost::lambda::_1 << constant(','));

   std::for_each(values.begin(), last_val,
	 where->second.get() << boost::lambda::_1 << constant(',') );

   where->second.get() << *last_val << endl; 
}

void TableDBInterface::print(ostream& os) const
{
   os << "Contexts:" << endl;
   for(multimap<string,xpath_info>::const_iterator
	 ci = contexts.begin(); ci != contexts.end(); ++ci)
   {
      os << ci->first << ":" << endl;
      os << '\t' << "path: [";
      for(list<string>::const_iterator
	    p = ci->second.path.begin(); p != ci->second.path.end(); ++p)
	 os << *p << ", ";
      os << "]" << endl;
      os << '\t' << "table_id: " << ci->second.table_id << endl;
      os << '\t' << "context_label: " << ci->second.context_label << endl;
   }
}
