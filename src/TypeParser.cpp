// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <memory>
#include <sstream>

#include "TypeParser.h"

type_parser::type_parser()
{   
  addRule("syntax", TYPE);
  addRule("grammar", TYPESET);
  addRule("property!", PROPERTY);
  addRule("=", ASSIGNMENT);
  addRule("([@a-zA-Z][a-zA-Z0-9_]*)", TEXTORID);
  addRule("-?(0x[a-fA-F]+)?([0-9a-fA-F]+)", DIGIT);
  addRule("'([ $a-zA-Z0-9_!<>\\(\\)\\[\\]\\+\\-=\\/\\.\\*\\^\\\\\\\"{}&\\|;:\\,#]+)'", LITERAL);
  addRule("\\\"([@ $a-zA-Z0-9_!<>\\(\\)\\[\\]\\+\\-=\\/\\.\\*\\?\\^\\\\\\\"{}&\\|;:\\,#]+)\\\"", REGEX);
  addRule("\\+", ONE_MORE);
  addRule("\\|", OR);
  addRule("\\*", MANY);
  addRule(";", SEMICOLON);
  addRule("\\?", QUESTION_MARK);
  addRule("\\,", COMMA);
  addRule("\\[", OPEN_BRACKET);
  addRule("\\]", CLOSE_BRACKET);
  addRule("\\(", LEFT_PAREN);
  addRule("\\)", RIGHT_PAREN);
  addRule("\r|\n|\r\n", EOL);
  buildRules();
}

void addReduction(const std::string& name, size_t index, std::string& typeset)
{
  std::stringstream ss;
  ss << index;
  typeset += "{";
  typeset += "/*";
  typeset += name + "_";
  typeset += ss.str();
  typeset += "*/";
  typeset += "}";
}

void type_parser::addProperty(lexertl::recursive_match_results<const char* >& results, bool eof, std::string& typeset)
{
  getToken<LEFT_PAREN>(results, eof, "expected '('");
  getToken<TEXTORID>(results, eof, "expected identifier of a property");
  typeset += " ";
  typeset += "$" + results.str() + " ";
  getToken<COMMA>(results, eof, "expected ','");
  getToken<DIGIT>(results, eof, "expected digit");
  typeset += results.str() + " ";

  getToken<RIGHT_PAREN>(results, eof, "expected ')'");
}

//
//  every reduction function contains a comment that identifies
//  reduction. each | (or operator) increases index
//  but only outside group operator ()

//  for instance : p = a | b generate { p_0 } and { p_1} 
//  but p = (a | b) will generate { p_0 }
//

void type_parser::parseType(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool eof, std::string& typeset)
{
  size_t index = 0;
  bool group = false;
  do {
    getNextToken(results, eof);
    // 
    //  first there is a check against grouping 
    //  if so, a group flag is set to true
    //  reduction is generated for the whole group
    //     
    if (results.id == TEXTORID) symbols.insert(results.str());
    
    if (results.id == LEFT_PAREN) group = true;
    if (results.id == RIGHT_PAREN) group = false;
        
    if (results.id == SEMICOLON || (results.id == OR && !group)) addReduction(name, index, typeset);
      
    if (results.id == OR && !group) index++;

    if (results.id == PROPERTY) {
      addProperty(results, eof, typeset);
      continue;
    }
    typeset += " " + results.str() + " ";
           
  } while (results.id != SEMICOLON && results.id != CLOSE_BRACKET && results.id != EOF);

  std::stringstream ss;
  std::string msg = "expected ';' at end of ";
  msg += name;
  msg += " type definition";
  if(results.id != SEMICOLON) throw SyntaxError(msg.c_str());
  typeset += "\n";
}

size_t type_parser::parse(const char* begin, const char* end, std::string& typeset)
{
  lexertl::recursive_match_results<const char *> results(begin, end);
  bool eof = false;
  size_t pos = 0;
  getToken<TYPESET>(results, eof, "expected typeset");
  getToken<TEXTORID>(results, eof, "expected identifier of a typeset");
  getToken<ASSIGNMENT>(results, eof, "expected '='");
  getToken<OPEN_BRACKET>(results, eof, "expected '['");
  bool eatenType = false;
  do {

    getNextToken(results, eof);
    if (results.id == TYPE) {
      std::string name = getToken<TEXTORID>(results, eof, "expected identifier of a type");      
      typeset += name;
      getToken<ASSIGNMENT>(results, eof, "expected '='");      
      typeset += " : ";
      productions.insert(name);
      parseType(name, results, eof, typeset); 
    }
    else if(results.id != CLOSE_BRACKET){
      std::stringstream ss;
      ss << currentLine;
      std::string msg = "expected type keyword ";
      msg += "but got ";
      msg += results.str();
      msg += " in line : ";
      msg += ss.str();
      throw SyntaxError(msg.c_str());
    }
  
  } while (results.id != CLOSE_BRACKET);
  for (auto element : symbols) {
    auto it = productions.find(element);
    std::string msg = "production ";
    msg += element;
    msg += " has not been defined ";
    if (it == productions.end()) throw SyntaxError(msg.c_str());
  }

  pos = results.end - begin;
  return pos;
}