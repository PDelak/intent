// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <limits>
#include <iostream>
#include <filesystem>
#include <stack>
#include <sstream>

#include "AST.h"
#include "dparse.h"

#include "d.h"
#include "util.h"
#include "lua.hpp"
#include "lauxlib.h"
#include "dparse_tree.h"

#include "Type.h"
#include "ASTRtti.h"
#include "ASTPrinter.h"
#include "NodeLinker.h"
#include "mkdparse.h"
#include "PtablesReader.h"
#include "TypeParser.h"
#include "BuiltinReductions.h"
#include "TypesetMerger.h"
#include "DynamicReductions.h"
#include "MatchVisitor.h"
#include "AST2Code.h"
#include "MatchReferences.h"

void tests()
{  
  assert(get_type_instance<type>() == get_type_instance<type>());
  assert(get_type_instance<type>() != get_type_instance<string_type>());
  using namespace Intent::FrontEnd;
  std::tr2::sys::path p = std::tr2::sys::current_path().append("..\\tests\\");
  {
    std::vector<char> test = readFile(p.string() + "test1.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("test1.dl", false, output);
  }  
  {
    std::vector<char> test = readFile(p.string() + "test2.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("test2.dl", false, output);
  }  
  {
    std::vector<char> test = readFile(p.string() + "block1.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("block1.dl", false, output);
  }
  {
    std::vector<char> test = readFile(p.string() + "block2.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("block2.dl", false, output);
  }
  
  {
    std::vector<char> test = readFile(p.string() + "loops.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("loops.dl", false, output);
  } 
  
  {
    std::vector<char> test = readFile(p.string() + "function.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("function.dl", false, output);
  } 
  
}

void testTypes()
{
  string_type s;
  string_type s2;
  assert(s == s2);
  type* t1 = new string_type;
  type* t2 = new string_type;
  TypePtr pt1(new string_type);
  TypePtr pt2(new string_type);
  assert(equal(t1,t2));
  assert(equal(pt1, pt2));
}

void testAstRtti()
{
  NodePtr node(new ReturnStatement(0));
  assert(is<ReturnStatement>(node));
}

void DParseTablesTests(const std::string& filename, const std::unordered_map<std::string, reductionf>& builtinRMappings)
{
  using namespace Intent::FrontEnd;
  D_ParserTablesReader parser;
  auto path = std::tr2::sys::current_path().append("\\");

  auto holder = parse(path.string().append("intent.g.d_parser.c"), 
                  parser, 
                  builtinRMappings);

  std::cout << "\ncompilation success\n";
  
  auto p = new_D_Parser(holder->parserTables, sizeof(D_ParseNode_User));
  auto buffer = readFile(filename);

  const char* b = &buffer[0];
  const char* e = &buffer[0] + buffer.size();
 
  if (dparse(p, const_cast<char*>(b), 
             std::distance(b, e)) && !p->syntax_errors) 
  {
    printf("success\n");
    MatchVisitor matchVisitor;
    programPtr->accept(&matchVisitor);
  }
  else
    printf("failure\n");
  
}

void testTypeParser()
{

  using namespace Intent::FrontEnd;
  type_parser tp;
  
  std::vector<char> buffer = readFile("dsl3.dl");

  const char* b = &buffer[0];
  const char* e = &buffer[0] + buffer.size();
  std::string typeset;
  try {
    size_t pos = tp.parse(b, e, typeset);
    std::cout << "pos : " << pos << std::endl;
    std::cout << typeset << std::endl;
    std::string remainingBuf;
    std::copy(b + pos, e, std::back_inserter(remainingBuf));
    std::cout << remainingBuf << std::endl;
  }

  catch (const SyntaxError& se) {
    std::cerr << "compilation error : " << se.what() << std::endl;
  }
  catch (const std::runtime_error& re) {
    std::cerr << re.what() << std::endl;
  }

}


void testFindingReferences() {}

int main()
{
  return 0;
}
