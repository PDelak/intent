// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "Visitor.h"
#include "AST.h"

void NullVisitor::PreVisit(Program*) {}
void NullVisitor::PostVisit(Program*) {}
void NullVisitor::PreVisit(Statement*) {}
void NullVisitor::PostVisit(Statement*) {}
void NullVisitor::PreVisit(Expression*) {}
void NullVisitor::PostVisit(Expression*) {}
void NullVisitor::PreVisit(LetStatement*) {}
void NullVisitor::PostVisit(LetStatement*) {}
void NullVisitor::PreVisit(Identifier*) {}
void NullVisitor::PostVisit(Identifier*) {}
void NullVisitor::PreVisit(StringExpr*) {}
void NullVisitor::PostVisit(StringExpr*) {}
void NullVisitor::PreVisit(Literal*) {}
void NullVisitor::PostVisit(Literal*) {}
void NullVisitor::PreVisit(NumberExpr*) {}
void NullVisitor::PostVisit(NumberExpr*) {}
void NullVisitor::PreVisit(BoolExpr*) {}
void NullVisitor::PostVisit(BoolExpr*) {}
void NullVisitor::PreVisit(Parameter*) {}
void NullVisitor::PostVisit(Parameter*) {}
void NullVisitor::PreVisit(FunctionBody*) {}
void NullVisitor::PostVisit(FunctionBody*) {}
void NullVisitor::PreVisit(TypeName*) {}
void NullVisitor::PostVisit(TypeName*) {}
void NullVisitor::PreVisit(FunctionDecl*) {}
void NullVisitor::PostVisit(FunctionDecl*) {}
void NullVisitor::PreVisit(FunctionExpr*) {}
void NullVisitor::PostVisit(FunctionExpr*) {}
void NullVisitor::PreVisit(FunctionCall*) {}
void NullVisitor::PostVisit(FunctionCall*) {}
void NullVisitor::PreVisit(CompoundStatement*) {}
void NullVisitor::PostVisit(CompoundStatement*) {}
void NullVisitor::PreVisit(Argument*) {}
void NullVisitor::PostVisit(Argument*) {}
void NullVisitor::PreVisit(ParenExpression*) {}
void NullVisitor::PostVisit(ParenExpression*) {}
void NullVisitor::PreVisit(BinaryExpression*) {}
void NullVisitor::PostVisit(BinaryExpression*) {}
void NullVisitor::PreVisit(WhileLoop*) {}
void NullVisitor::PostVisit(WhileLoop*) {}
void NullVisitor::PreVisit(ForLoop*) {}
void NullVisitor::PostVisit(ForLoop*) {}
void NullVisitor::PreVisit(IfStatement*) {}
void NullVisitor::PostVisit(IfStatement*) {}
void NullVisitor::PreVisit(UnaryExpression*) {}
void NullVisitor::PostVisit(UnaryExpression*) {}
void NullVisitor::PreVisit(ReturnStatement*) {}
void NullVisitor::PostVisit(ReturnStatement*) {}
void NullVisitor::PreVisit(TableExpression*) {}
void NullVisitor::PostVisit(TableExpression*) {}
void NullVisitor::PreVisit(TableElement*) {}
void NullVisitor::PostVisit(TableElement*) {}
void NullVisitor::PreVisit(NullStatement*) {}
void NullVisitor::PostVisit(NullStatement*) {}
void NullVisitor::PreVisit(MatchStatement*) {}
void NullVisitor::PostVisit(MatchStatement*) {}
void NullVisitor::PreVisit(MatchId*) {}
void NullVisitor::PostVisit(MatchId*) {}
void NullVisitor::PreVisit(CodeExpression*) {}
void NullVisitor::PostVisit(CodeExpression*) {}
void NullVisitor::PreVisit(MatchCase*) {}
void NullVisitor::PostVisit(MatchCase*) {}
void NullVisitor::PreVisit(Extension*) {}
void NullVisitor::PostVisit(Extension*) {}
void NullVisitor::PreVisit(DynamicNode*) {}
void NullVisitor::PostVisit(DynamicNode*) {}