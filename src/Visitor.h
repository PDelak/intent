// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VISITOR_H
#define VISITOR_H

struct Program;
struct Statement;
struct Expression;
struct LetStatement;
struct Identifier;
struct StringExpr;
struct Literal;
struct NumberExpr;
struct BoolExpr;
struct TypeName;
struct Parameter;
struct FunctionBody;
struct FunctionDecl;
struct FunctionExpr;
struct FunctionCall;
struct CompoundStatement;
struct Argument;
struct ParenExpression;
struct BinaryExpression;
struct WhileLoop;
struct ForLoop;
struct IfStatement;
struct UnaryExpression;
struct ReturnStatement;
struct TableExpression;
struct TableElement;
struct NullStatement;
struct MatchStatement;
struct MatchId;
struct CodeExpression;
struct MatchCase;
struct Extension;
struct DynamicNode;

struct Visitor
{
  virtual ~Visitor() {}
  virtual void PreVisit(Program*) = 0;
  virtual void PostVisit(Program*) = 0;
  virtual void PreVisit(Statement*) = 0;
  virtual void PostVisit(Statement*) = 0;
  virtual void PreVisit(Expression*) = 0;
  virtual void PostVisit(Expression*) = 0;
  virtual void PreVisit(LetStatement*) = 0;
  virtual void PostVisit(LetStatement*) = 0;
  virtual void PreVisit(Identifier*) = 0;
  virtual void PostVisit(Identifier*) = 0;
  virtual void PreVisit(StringExpr*) = 0;
  virtual void PostVisit(StringExpr*) = 0;
  virtual void PreVisit(Literal*) = 0;
  virtual void PostVisit(Literal*) = 0;
  virtual void PreVisit(NumberExpr*) = 0;
  virtual void PostVisit(NumberExpr*) = 0;
  virtual void PreVisit(BoolExpr*) = 0;
  virtual void PostVisit(BoolExpr*) = 0;
  virtual void PreVisit(TypeName*) = 0;
  virtual void PostVisit(TypeName*) = 0;
  virtual void PreVisit(Parameter*) = 0;
  virtual void PostVisit(Parameter*) = 0;
  virtual void PreVisit(FunctionBody*) = 0;
  virtual void PostVisit(FunctionBody*) = 0;
  virtual void PreVisit(FunctionDecl*) = 0;
  virtual void PostVisit(FunctionDecl*) = 0;
  virtual void PreVisit(FunctionExpr*) = 0;
  virtual void PostVisit(FunctionExpr*) = 0;
  virtual void PreVisit(FunctionCall*) = 0;
  virtual void PostVisit(FunctionCall*) = 0;
  virtual void PreVisit(CompoundStatement*) = 0;
  virtual void PostVisit(CompoundStatement*) = 0;
  virtual void PreVisit(Argument*) = 0;
  virtual void PostVisit(Argument*) = 0;
  virtual void PreVisit(ParenExpression*) = 0;
  virtual void PostVisit(ParenExpression*) = 0;
  virtual void PreVisit(BinaryExpression*) = 0;
  virtual void PostVisit(BinaryExpression*) = 0;
  virtual void PreVisit(WhileLoop*) = 0;
  virtual void PostVisit(WhileLoop*) = 0;
  virtual void PreVisit(ForLoop*) = 0;
  virtual void PostVisit(ForLoop*) = 0;
  virtual void PreVisit(IfStatement*) = 0;
  virtual void PostVisit(IfStatement*) = 0;
  virtual void PreVisit(UnaryExpression*) = 0;
  virtual void PostVisit(UnaryExpression*) = 0;
  virtual void PreVisit(ReturnStatement*) = 0;
  virtual void PostVisit(ReturnStatement*) = 0;
  virtual void PreVisit(NullStatement*) = 0;
  virtual void PostVisit(NullStatement*) = 0;
  virtual void PreVisit(MatchStatement*) = 0;
  virtual void PostVisit(MatchStatement*) = 0;
  virtual void PreVisit(MatchId*) = 0;
  virtual void PostVisit(MatchId*) = 0;
  virtual void PreVisit(CodeExpression*) = 0;
  virtual void PostVisit(CodeExpression*) = 0;
  virtual void PreVisit(MatchCase*) = 0;
  virtual void PostVisit(MatchCase*) = 0;
  virtual void PreVisit(Extension*) = 0;
  virtual void PostVisit(Extension*) = 0;
  virtual void PreVisit(DynamicNode*) = 0;
  virtual void PostVisit(DynamicNode*) = 0;
  virtual void PreVisit(TableExpression*) = 0;
  virtual void PostVisit(TableExpression*) = 0;
  virtual void PreVisit(TableElement*) = 0;
  virtual void PostVisit(TableElement*) = 0;

};

class NullVisitor : public Visitor
{
public:
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
  virtual void PreVisit(TableExpression*);
  virtual void PostVisit(TableExpression*);
  virtual void PreVisit(TableElement*);
  virtual void PostVisit(TableElement*);
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

};

#endif
