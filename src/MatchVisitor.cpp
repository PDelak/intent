// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "AST.h"
#include "MatchVisitor.h"

MatchVisitor::MatchVisitor()
{}

void MatchVisitor::PreVisit(MatchStatement* ms) 
{
  MatchStatementRep match;
  matchStack.push_back(match);
}

void MatchVisitor::PostVisit(MatchStatement*) 
{
  matchStack.pop_back();
}

void MatchVisitor::PreVisit(MatchId* mid) 
{
  size_t index = matchStack.size() - 1;
  matchStack[index].id = mid->getIdentifier();
  matchTable[matchStack[index].id].id = mid->getIdentifier();
}

void MatchVisitor::PostVisit(MatchId*) {}
void MatchVisitor::PreVisit(CodeExpression* c) {}
void MatchVisitor::PostVisit(CodeExpression*) {}

void MatchVisitor::PreVisit(MatchCase* mc) 
{
  MatchCaseRep matchCase;
  matchCase.id = mc->matchId();
  matchCase.code = mc->codeExpr();
  size_t matchStatementIndex = matchStack.size() - 1;

  matchTable[matchStack[matchStatementIndex].id].cases.push_back(matchCase);
}

void MatchVisitor::PostVisit(MatchCase*) {}

std::string MatchVisitor::matchTableToString() const
{
  std::string buf;
  auto it = matchTable.begin();
  while (it != matchTable.end()) {
    buf.append("MatchStatement : ");
    buf.append(it->second.id);
    buf.append("\n");
    size_t size = it->second.cases.size();
    for (size_t i = 0; i < it->second.cases.size(); i++) {
      buf.append("MatchCase id : ");
      buf.append(it->second.cases[i].id);
      buf.append("\n");
      buf.append("MatchCase code : ");
      buf.append(it->second.cases[i].code);
      buf.append("\n");
    }
    buf.append("\n");
    it++;
  }
  return buf;
}