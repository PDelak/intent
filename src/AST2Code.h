// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef AST2CODE_VISITOR_H
#define AST2CODE_VISITOR_H

#include <iostream>
#include <vector>
#include <string>
#include "Visitor.h"
#include "AST.h"

typedef std::vector<std::pair<Node*, std::string>> Stream;

struct AST2Code : public Visitor
{
  AST2Code();
  virtual void PreVisit(TableExpression*);
  virtual void PostVisit(TableExpression*);
  virtual void PreVisit(TableElement*);
  virtual void PostVisit(TableElement*);
  virtual void PreVisit(Program*);
  virtual void PostVisit(Program*);
  virtual void PreVisit(Statement*);
  virtual void PostVisit(Statement*);
  virtual void PreVisit(Expression*);
  virtual void PostVisit(Expression*);
  virtual void PreVisit(LetStatement*);
  virtual void PostVisit(LetStatement*);
  virtual void PreVisit(Identifier*);
  virtual void PostVisit(Identifier*);
  virtual void PreVisit(StringExpr*);
  virtual void PostVisit(StringExpr*);
  virtual void PreVisit(Literal*);
  virtual void PostVisit(Literal*);
  virtual void PreVisit(NumberExpr*);
  virtual void PostVisit(NumberExpr*);
  virtual void PreVisit(BoolExpr*);
  virtual void PostVisit(BoolExpr*);
  virtual void PreVisit(TypeName*);
  virtual void PostVisit(TypeName*);
  virtual void PreVisit(Parameter*);
  virtual void PostVisit(Parameter*);
  virtual void PreVisit(FunctionBody*);
  virtual void PostVisit(FunctionBody*);
  virtual void PreVisit(FunctionDecl*);
  virtual void PostVisit(FunctionDecl*);
  virtual void PreVisit(FunctionExpr*);
  virtual void PostVisit(FunctionExpr*);
  virtual void PreVisit(FunctionCall*);
  virtual void PostVisit(FunctionCall*);
  virtual void PreVisit(CompoundStatement*);
  virtual void PostVisit(CompoundStatement*);
  virtual void PreVisit(Argument*);
  virtual void PostVisit(Argument*);
  virtual void PreVisit(ParenExpression*);
  virtual void PostVisit(ParenExpression*);
  virtual void PreVisit(BinaryExpression*);
  virtual void PostVisit(BinaryExpression*);
  virtual void PreVisit(WhileLoop*);
  virtual void PostVisit(WhileLoop*);
  virtual void PreVisit(ForLoop*);
  virtual void PostVisit(ForLoop*);
  virtual void PreVisit(IfStatement*);
  virtual void PostVisit(IfStatement*);
  virtual void PreVisit(UnaryExpression*);
  virtual void PostVisit(UnaryExpression*);
  virtual void PreVisit(ReturnStatement*);
  virtual void PostVisit(ReturnStatement*);
  virtual void PreVisit(NullStatement*);
  virtual void PostVisit(NullStatement*);
  virtual void PreVisit(MatchStatement*);
  virtual void PostVisit(MatchStatement*);
  virtual void PreVisit(MatchId*);
  virtual void PostVisit(MatchId*);
  virtual void PreVisit(CodeExpression*);
  virtual void PostVisit(CodeExpression*);
  virtual void PreVisit(MatchCase*);
  virtual void PostVisit(MatchCase*);
  virtual void PreVisit(Extension*);
  virtual void PostVisit(Extension*);
  virtual void PreVisit(DynamicNode*);
  virtual void PostVisit(DynamicNode*);

  std::string serialize() const {
    return stream[0].second;
  }

private:
  std::vector<std::pair<Node*, std::string>> stream;


};

#endif