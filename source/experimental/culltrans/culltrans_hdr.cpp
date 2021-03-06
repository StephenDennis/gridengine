/*___INFO__MARK_BEGIN__*/
/*************************************************************************
 * 
 *  The Contents of this file are made available subject to the terms of
 *  the Sun Industry Standards Source License Version 1.2
 * 
 *  Sun Microsystems Inc., March, 2001
 * 
 * 
 *  Sun Industry Standards Source License Version 1.2
 *  =================================================
 *  The contents of this file are subject to the Sun Industry Standards
 *  Source License Version 1.2 (the "License"); You may not use this file
 *  except in compliance with the License. You may obtain a copy of the
 *  License at http://gridengine.sunsource.net/Gridengine_SISSL_license.html
 * 
 *  Software provided under this License is provided on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 *  WITHOUT LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS,
 *  MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE, OR NON-INFRINGING.
 *  See the License for the specific provisions governing your rights and
 *  obligations concerning the Software.
 * 
 *   The Initial Developer of the Original Code is: Sun Microsystems, Inc.
 * 
 *   Copyright: 2001 by Sun Microsystems, Inc.
 * 
 *   All Rights Reserved.
 * 
 ************************************************************************/
/*___INFO__MARK_END__*/
// culltrans_hdr.cpp
// write Header file

#include <map>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include "culltrans_repository.h"
#include "culltrans.h"

#ifdef HAVE_STD
using namespace std;
#endif

// global variables stores key element of list
static vector<Elem>::iterator key;
static const string indent("   ");

// writeHeader
static void writeHeader(ofstream& hdr, map<string, List>::iterator& elem) {
   vector<Elem>::iterator it;
   map<string, List>::iterator list = lists.end();

   hdr << "// " << elem->second.name << "_implementation.h" << endl;
   hdr << "// this file is automatically generated. DO NOT EDIT" << endl << endl;
   hdr << "#ifndef " << elem->second.name << "_implementation_h" << endl;
   hdr << "#define " << elem->second.name << "_implementation_h" << endl << endl;
   hdr << "#include <pthread.h>" << endl;
   hdr << "#include \"OB/CORBA.h\"" << endl;
   hdr << "#include \"basic_types.h\"" << endl;
   hdr << "#include \"Master_impl.h\"" << endl;
   for(it=elem->second.elems.begin(); it != elem->second.elems.end(); ++it) {
      if(it->type == lListT && (list = lists.find(it->listType)) != lists.end())
         hdr << "#include \"" << list->second.name << ".h\"" << endl;
   }
   hdr << "#include \"" << elem->second.name << "_skel.h\"" << endl;

   hdr << "extern \"C\" {" << endl;
   hdr << "#include \"" << elem->second.file << "\"" << endl;
   hdr << "}" << endl;
}

// writeClassHeader
static void writeClassHeader(ofstream& hdr, map<string, List>::iterator& elem) {
   hdr << endl << "class GE_" << elem->second.name << "_implementation" << endl;
   hdr << indent << indent << ": virtual public GE_" << elem->second.name << "_skel {" << endl;
   
   // forbid copy c'tor and assignment op
   hdr << indent << "private:" << endl;
   hdr << indent << indent << "GE_" << elem->second.name << "_implementation(const GE_" << elem->second.name << "_implementation&);" << endl;
   hdr << indent << indent << "GE_" << elem->second.name << "_implementation& operator=(const GE_" << elem->second.name << "_implementation&);" << endl << endl;

   // c'tor and d'tor now
   hdr << indent << "public:" << endl;
   hdr << indent << indent << "GE_" << elem->second.name << "_implementation(const ";
   if(key->type == lUlongT)
      hdr << "GE_sge_ulong ";
   else
      hdr << "char* ";
   hdr << "_key, CORBA_ORB_var o);" << endl;
   hdr << indent << indent << "GE_" << elem->second.name << "_implementation(const ";
   if(key->type == lUlongT)
      hdr << "GE_sge_ulong ";
   else
      hdr << "char* ";
   hdr << "_key, const time_t& tm, CORBA_ORB_var o);" << endl;
   hdr << indent << indent << "virtual ~GE_" << elem->second.name << "_implementation();" << endl;
}

// writeFunctions
static bool writeFunctions(ofstream& hdr, vector<Elem>::iterator& it, map<string, List>::iterator& elem) {
   string buffer = "";
   string buffer3 = "virtual ";
   string buffer2 = "virtual GE_sge_ulong set_" + it->name + "(";
   map<string, List>::iterator list = lists.end();

   if(it->type == lListT) {
      list = lists.find(it->listType);
      if(list != lists.end()) {
         if(!it->object)
            buffer2 += "const ";
         buffer += "GE_" + list->second.name;
         buffer2 += "GE_" + list->second.name;
         buffer3 += "GE_" + list->second.name;
         if(!it->object) {
            buffer += "Seq";
            buffer2 += "Seq";
            buffer3 += "Seq";
         }
         buffer += " ";
         if(it->object)
            buffer2 += "* ";
         else
            buffer2 += "& ";
         buffer3 += "* ";
      }
      else {
         cerr << "Could not find " << it->listType << "." << endl;
         return false;
      }
   }
   else if(it->type == lStringT) {
      buffer += "GE_sge_string ";
      buffer2 += "const char* ";
      buffer3 += "GE_sge_string ";
   }
   else {
      buffer += multiType2sgeType[it->type];
      buffer += ' ';
      buffer2 += multiType2sgeType[it->type];
      buffer2 += ' ';
      buffer3 += multiType2sgeType[it->type];
      buffer3 += ' ';
   }

   hdr << indent << indent << "// " << buffer << it->name << ";" << endl;
   hdr << indent << indent << buffer3 << "get_" << it->name << "(CORBA_Context* ctx)" << ((it->idlonly || it->key || list->second.interf)?" = 0;":";") << endl;
   if(!it->readonly)
      hdr << indent << indent << buffer2 << "val, CORBA_Context* ctx)" << ((it->idlonly || it->key || list->second.interf)?" = 0;":";") << endl;
   hdr << endl;

   return true;
}

// writeClassFooter
static void writeClassFooter(ofstream& hdr, map<string, List>::iterator& elem) {
   // state functions
   hdr << indent << indent << "virtual GE_contentSeq* get_content(CORBA_Context* ctx);" << endl;
   hdr << indent << indent << "virtual GE_sge_ulong   set_content(const GE_contentSeq& state, CORBA_Context* ctx);" << endl << endl;
   
   // add & destroy functions
   hdr << indent << indent << "virtual void               add(CORBA_Context* ctx) = 0;" << endl;
   hdr << indent << indent << "virtual void               destroy(CORBA_Context* ctx) = 0;" << endl << endl;

   // write protected stuff for interfaces
   if(elem->second.interf) {
      hdr << indent << "protected:" << endl << indent << indent;
      if(key->type == lUlongT)
         hdr << "GE_sge_ulong    ";
      else
         hdr << "CORBA_String_var    ";
      hdr << "key;" << endl;
      hdr << indent << indent << "lListElem*                self;" << endl;
      hdr << indent << indent << "lListElem*                newSelf;" << endl;
      hdr << indent << indent << "virtual lListElem*        getSelf() = 0;" << endl;
      hdr << indent << indent << "CORBA_ORB_var             orb;" << endl;
      hdr << indent << indent << "time_t                    creation;" << endl;
      hdr << indent << indent << "GE_sge_ulong          lastEvent;" << endl;
      hdr << indent << indent << "int                       apiListType;" << endl;

      hdr << indent << "friend class GE_Master_impl;" << endl << endl;
   }
   hdr << "};" << endl << endl;
}


// writeHdr
// writes out the implementation header for a given interface or struct
bool writeHdr(map<string, List>::iterator& elem) {
   cout << "Creating implementation header for " << elem->second.name << endl;
   
   // open output file
   vector<Elem>::iterator it;
   string file(elem->second.name);
   file += "_implementation.h";
   ofstream hdr(file.c_str());
   if(!hdr) {
      cerr << "Could not open output file for " << elem->second.name << endl;
      return false;
   }

   // nothing to do if it's not an interface
   // just write something into the file
   if(!elem->second.interf)
      return true;

   // write header and #includes
   writeHeader(hdr, elem);

   // get key element
   key = elem->second.elems.end();
   for(it=elem->second.elems.begin(); it != elem->second.elems.end(); ++it)
      if(it->key) {
         key = it;
         break;
      }
   if(key == elem->second.elems.end()) {
      cerr << "No key element specified for interface " << elem->second.name << endl;
      return false;
   }

   // write class header
   writeClassHeader(hdr, elem);

   // write class declaration
   for(it=elem->second.elems.begin(); it != elem->second.elems.end(); ++it)
      if(!writeFunctions(hdr, it, elem))
         return false;

   // write class footer
   writeClassFooter(hdr, elem);

   hdr << "#endif // _" << elem->second.name << "_implementation.h" << endl;
   hdr.close();
   
   return true;   
}
