// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "AST.h"
#include "NodeLinker.h"

ASTNodeLinker::ASTNodeLinker() {}

void ASTNodeLinker::PreVisit(Program* node) 
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(Program*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(Statement* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(Statement*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(Expression* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(Expression*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(LetStatement* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(LetStatement*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(Identifier* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(Identifier*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(StringExpr* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(StringExpr*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(Literal* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(Literal*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(NumberExpr* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(NumberExpr*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(BoolExpr* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(BoolExpr*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(TypeName* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(TypeName*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(Parameter* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(Parameter*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(FunctionBody* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(FunctionBody*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(FunctionDecl* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(FunctionDecl*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(FunctionExpr* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(FunctionExpr*)
{
  nodeStack.pop();
}


void ASTNodeLinker::PreVisit(FunctionCall* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(FunctionCall*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(CompoundStatement* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(CompoundStatement*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(Argument* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(Argument*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(ParenExpression* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(ParenExpression*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(BinaryExpression* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(BinaryExpression*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(WhileLoop* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(WhileLoop*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(ForLoop* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(ForLoop*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(IfStatement* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(IfStatement*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(UnaryExpression* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(UnaryExpression*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(ReturnStatement* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(ReturnStatement*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(TableExpression* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);

}

void ASTNodeLinker::PostVisit(TableExpression*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(TableElement* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(TableElement*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(NullStatement* node)
{
  if (!nodeStack.empty()) node->setParent(nodeStack.top());
  nodeStack.push(node);
}

void ASTNodeLinker::PostVisit(NullStatement*)
{
  nodeStack.pop();
}

void ASTNodeLinker::PreVisit(MatchStatement*) {}
void ASTNodeLinker::PostVisit(MatchStatement*) {}
void ASTNodeLinker::PreVisit(MatchId*) {}
void ASTNodeLinker::PostVisit(MatchId*) {}
void ASTNodeLinker::PreVisit(CodeExpression*) {}
void ASTNodeLinker::PostVisit(CodeExpression*) {}
void ASTNodeLinker::PreVisit(MatchCase*) {}
void ASTNodeLinker::PostVisit(MatchCase*) {}
void ASTNodeLinker::PreVisit(Extension*) {}
void ASTNodeLinker::PostVisit(Extension*) {}
void ASTNodeLinker::PreVisit(DynamicNode*) {}
void ASTNodeLinker::PostVisit(DynamicNode*) {}

