// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <memory>
#include <sstream>

#include "TypesetMerger.h"

typeset_merger::typeset_merger()
{
  addRule("syntax", TYPE);
  addRule("grammar", TYPESET);
  addRule("property!", PROPERTY);
  addRule("=", ASSIGNMENT);
  addRule("([@a-zA-Z][a-zA-Z0-9_]+)", TEXTORID);
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

/*
every reduction function contains a comment that identifies
reduction. each | (or operator) increases index
but only outside group operator ()

for instance : a | b generate { a_0 } and { b_0}
but  (a | b) will generate { a_0 }
*/
void typeset_merger::parseType(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool eof, std::string& typeset)
{
  size_t index = 0;
  bool group = false;
  do {
    getNextToken(results, eof);
    typeset += " " + results.str() + " ";

  } while (results.id != SEMICOLON && results.id != CLOSE_BRACKET && results.id != EOF);
  typeset += "\n";
}

size_t typeset_merger::parse(const char* begin, const char* end, std::string& typeset)
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
      typeset += results.str();
      typeset += " ";
      std::string name = getToken<TEXTORID>(results, eof, "expected identifier of a type");
      typeset += name;
      getToken<ASSIGNMENT>(results, eof, "expected '='");
      typeset += " = ";
      parseType(name, results, eof, typeset);
    }

  } while (results.id != CLOSE_BRACKET);
  pos = results.end - begin;
  return pos;
}