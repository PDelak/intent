// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INTENT_TYPESET_MERGET_H
#define INTENT_TYPESET_MERGET_H

#include <sstream>
#include <unordered_map>
#include <tuple>
#include "../lexertl/rules.hpp"
#include "../lexertl/state_machine.hpp"
#include "../lexertl/generator.hpp"
#include "../lexertl/lookup.hpp"
#include "../dparser/dparse_tables.h"
#include "Exceptions.h"
#include "BasicParser.h"
#include "TypeSetLE.h"


class typeset_merger : public basic_parser<LangElem, typeset_merger>
{
public:
  typeset_merger();
  size_t parse(const char* begin, const char* end, std::string& typeset);
  bool isEOL(const lexertl::recursive_match_results<const char* >& results, bool eof) { return !eof && results.id == EOL; }
  void parseType(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool eof, std::string& typeset);

private:
};
#endif
