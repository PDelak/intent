// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "AST.h"
#include "SemaChecker.h"
#include <cassert>
#include "VisitorTools.h"

typedef std::vector<std::pair<Node*, TypePtr>> Stream;

SemanticChecker::SemanticChecker() {}

void SemanticChecker::PreVisit(Program* node) 
{
    shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(Program* node) 
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    TypePtr t(new type);
    return t;
  });
}

void SemanticChecker::PreVisit(Statement* node) 
{
    shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(Statement* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(Expression* node)
{
    shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(Expression* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(LetStatement* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(LetStatement* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(Identifier* node) 
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(Identifier* node) 
{
  reduce<TypePtr>(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(StringExpr* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(StringExpr* node)
{
  reduce<TypePtr>(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(Literal* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(Literal* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(NumberExpr* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(NumberExpr* node)
{
  reduce<TypePtr>(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(BoolExpr* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(BoolExpr* node)
{
  reduce<TypePtr>(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(TypeName* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(TypeName* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(Parameter* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(Parameter* node)
{
  reduce<TypePtr>(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(FunctionBody* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(FunctionBody* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(FunctionDecl* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(FunctionDecl* node)
{
  reduce<TypePtr>(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(FunctionCall* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(FunctionCall* node)
{
  reduce<TypePtr>(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(CompoundStatement* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(CompoundStatement* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(Argument* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(Argument* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(ParenExpression* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(ParenExpression* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(BinaryExpression* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(BinaryExpression* node)
{
  reduce<TypePtr>(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });

}

void SemanticChecker::PreVisit(WhileLoop* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(WhileLoop* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(ForLoop* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(ForLoop* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(IfStatement* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(IfStatement* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(UnaryExpression* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(UnaryExpression* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(ReturnStatement* node)
{
  shift<TypePtr>(stream);
}

void SemanticChecker::PostVisit(ReturnStatement* node)
{
  reduce<TypePtr>(stream, node, [](const Stream& stream, size_t begin, size_t end) {
      TypePtr t(new type);
      return t;
  });
}

void SemanticChecker::PreVisit(NullStatement* node)
{
}

void SemanticChecker::PostVisit(NullStatement* node)
{
}

void SemanticChecker::PreVisit(MatchStatement*) {}
void SemanticChecker::PostVisit(MatchStatement*) {}
void SemanticChecker::PreVisit(MatchId*) {}
void SemanticChecker::PostVisit(MatchId*) {}
void SemanticChecker::PreVisit(CodeExpression*) {}
void SemanticChecker::PostVisit(CodeExpression*) {}
void SemanticChecker::PreVisit(MatchCase*) {}
void SemanticChecker::PostVisit(MatchCase*) {}
void SemanticChecker::PreVisit(Extension*) {}
void SemanticChecker::PostVisit(Extension*) {}
