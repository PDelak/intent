// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef MATCH_VISITOR_H
#define MATCH_VISITOR_H

#include <iostream>
#include <list>
#include <unordered_map>
#include "Visitor.h"
#include "AST.h"
#include "Match.h"

typedef std::unordered_map<std::string, MatchStatementRep>  MatchStatementTable;

struct MatchVisitor : public NullVisitor
{
  MatchVisitor();
  virtual void PreVisit(MatchStatement*);
  virtual void PostVisit(MatchStatement*);
  virtual void PreVisit(MatchId*);
  virtual void PostVisit(MatchId*);
  virtual void PreVisit(CodeExpression*);
  virtual void PostVisit(CodeExpression*);
  virtual void PreVisit(MatchCase*);
  virtual void PostVisit(MatchCase*);

  std::vector<MatchStatementRep> matchStack;
  MatchStatementTable matchTable;

  std::string matchTableToString() const;
};

#endif
