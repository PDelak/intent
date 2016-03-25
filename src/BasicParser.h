// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INTENT_BASIC_PARSER_H
#define INTENT_BASIC_PARSER_H

#include <sstream>
#include <unordered_map>
#include <tuple>
#include "../lexertl/rules.hpp"
#include "../lexertl/state_machine.hpp"
#include "../lexertl/generator.hpp"
#include "../lexertl/lookup.hpp"
#include "../dparser/dparse_tables.h"
#include "Exceptions.h"


template<typename Elem, typename T>
class basic_parser 
{
public:
 
  basic_parser():currentLine(1) {}

  void addRule(const std::string& rule, Elem e) { rules.push(rule, e); }

  void getNextToken(lexertl::recursive_match_results<const char* >& results, bool& eof)
  {
    getNextTokenHelper(results, eof);
    while (static_cast<T*>(this)->isEOL(results, eof))
    {
      ++currentLine;
      getNextTokenHelper(results, eof);
      
    }
  }

  template<typename Iterator>
  void getNextTokenHelper(lexertl::recursive_match_results<Iterator>& results, bool& eof)
  {
    do
    {
      lexertl::lookup(sm, results);

      if (results.id == 0)
      {
        eof = true;
        break;
      }

    } while (results.id == results.npos());
  }
  
  template<Elem elem>
  std::string getToken(lexertl::recursive_match_results<const char* >& results, bool& eof, const std::string& err)
  {
    getNextToken(results, eof);
    if (results.id != elem) {
      std::string e = err.c_str();
      e += " ";
      e += "but got ";
      e += results.str();
      e += " line: ";
      std::stringstream ss;
      ss << currentLine;
      e += (ss.str());
      throw SyntaxError(e.c_str());
    }
    return results.str();
  }
  void buildRules()
  {
    lexertl::generator::build(rules, sm);
  }
protected:
  lexertl::rules                     rules;
  lexertl::state_machine             sm;
  size_t                             currentLine;

};

#endif
