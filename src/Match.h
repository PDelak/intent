// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef MATCH_TOOLS_INTENT_H
#define MATCH_TOOLS_INTENT_H

#include <vector>

struct MatchCaseRep
{
  std::string id;
  std::string code;
};

struct MatchStatementRep
{
  std::string id;
  std::vector<MatchCaseRep> cases;
};

#endif
