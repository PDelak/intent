// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "AST.h"
#include "ASTPrinter.h"

ASTPrinter::ASTPrinter(std::ostream& o) :out(o), level(0) {}


void ASTPrinter::PreVisit(Program* node) 
{
  indent();
  out << "Program : " << std::endl;
}

void ASTPrinter::PostVisit(Program*) 
{
  deindent();
}

void ASTPrinter::PreVisit(Statement*) 
{
  indent();
  out << "Statement" << std::endl;
}

void ASTPrinter::PostVisit(Statement*) 
{
  deindent();
}

void ASTPrinter::PreVisit(Expression*) 
{
  indent();
  out << "Expression" << std::endl;
}

void ASTPrinter::PostVisit(Expression*) 
{
  deindent();
}

void ASTPrinter::PreVisit(LetStatement*) 
{
  indent();
  out << "LetStatement" << std::endl;
}

void ASTPrinter::PostVisit(LetStatement*) 
{
  deindent();
}

void ASTPrinter::PreVisit(Identifier*) 
{
  indent();
  out << "Identifier" << std::endl;
}

void ASTPrinter::PostVisit(Identifier*) 
{
  deindent();
}

void ASTPrinter::PreVisit(StringExpr*) 
{
  indent();
  out << "Identifier" << std::endl;
}

void ASTPrinter::PostVisit(StringExpr*) 
{
  deindent();
}

void ASTPrinter::PreVisit(Literal*) 
{
  indent();
  out << "Literal" << std::endl;
}

void ASTPrinter::PostVisit(Literal*) 
{
  deindent();
}

void ASTPrinter::PreVisit(NumberExpr*) 
{
  indent();
  out << "NumberExpr" << std::endl;

}

void ASTPrinter::PostVisit(NumberExpr*) 
{
  deindent();
}

void ASTPrinter::PreVisit(BoolExpr*) 
{
  indent();
  out << "BoolExpr" << std::endl;

}

void ASTPrinter::PostVisit(BoolExpr*) 
{
  deindent();
}

void ASTPrinter::PreVisit(TypeName*) 
{
  indent();
  out << "TypeName" << std::endl;
}

void ASTPrinter::PostVisit(TypeName*) 
{
  deindent();
}

void ASTPrinter::PreVisit(Parameter*) 
{
  indent();
  out << "Parameter" << std::endl;
}

void ASTPrinter::PostVisit(Parameter*) 
{
  deindent();
}

void ASTPrinter::PreVisit(FunctionBody*) 
{
  indent();
  out << "FunctionBody" << std::endl;
}

void ASTPrinter::PostVisit(FunctionBody*) 
{
  deindent();
}

void ASTPrinter::PreVisit(FunctionDecl*) 
{
  indent();
  out << "FunctionDecl" << std::endl;
}

void ASTPrinter::PostVisit(FunctionDecl*) 
{
  deindent();
}

void ASTPrinter::PreVisit(FunctionCall*) 
{
  indent();
  out << "FunctionCall" << std::endl;
}

void ASTPrinter::PostVisit(FunctionCall*) 
{
  deindent();
}

void ASTPrinter::PreVisit(CompoundStatement*) 
{
  indent();
  out << "CompoundStatement" << std::endl;
}

void ASTPrinter::PostVisit(CompoundStatement*) 
{
  deindent();
}

void ASTPrinter::PreVisit(Argument*) 
{
  indent();
  out << "Argument" << std::endl;
}

void ASTPrinter::PostVisit(Argument*) 
{
  deindent();
}

void ASTPrinter::PreVisit(ParenExpression*) 
{
  indent();
  out << "Argument" << std::endl;
}

void ASTPrinter::PostVisit(ParenExpression*) 
{
  deindent();
}

void ASTPrinter::PreVisit(BinaryExpression*) 
{
  indent();
  out << "BinaryExpression" << std::endl;
}

void ASTPrinter::PostVisit(BinaryExpression*) 
{
  deindent();
}

void ASTPrinter::PreVisit(WhileLoop*) 
{
  indent();
  out << "WhileLoop" << std::endl;
}

void ASTPrinter::PostVisit(WhileLoop*) 
{
  deindent();
}

void ASTPrinter::PreVisit(ForLoop*) 
{
  indent();
  out << "WhileLoop" << std::endl;
}

void ASTPrinter::PostVisit(ForLoop*) 
{
  deindent();
}

void ASTPrinter::PreVisit(IfStatement*) 
{
  indent();
  out << "IfStatement" << std::endl;
}

void ASTPrinter::PostVisit(IfStatement*) 
{
  deindent();
}

void ASTPrinter::PreVisit(UnaryExpression*) 
{
  indent();
  out << "UnaryExpression" << std::endl;
}

void ASTPrinter::PostVisit(UnaryExpression*) 
{
  deindent();
}

void ASTPrinter::PreVisit(ReturnStatement*) 
{
  indent();
  out << "ReturnStatement" << std::endl;
}

void ASTPrinter::PostVisit(ReturnStatement*) 
{
  deindent();
}

void ASTPrinter::PreVisit(NullStatement*)
{
  indent();
  out << "NullStatement" << std::endl;
}

void ASTPrinter::PostVisit(NullStatement*)
{
  deindent();
}

void ASTPrinter::PreVisit(MatchStatement*) {}
void ASTPrinter::PostVisit(MatchStatement*) {}
void ASTPrinter::PreVisit(MatchId*) {}
void ASTPrinter::PostVisit(MatchId*) {}
void ASTPrinter::PreVisit(CodeExpression*) {}
void ASTPrinter::PostVisit(CodeExpression*) {}
void ASTPrinter::PreVisit(MatchCase*) {}
void ASTPrinter::PostVisit(MatchCase*) {}
void ASTPrinter::PreVisit(Extension*) {}
void ASTPrinter::PostVisit(Extension*) {}
void ASTPrinter::PreVisit(DynamicNode*) {}
void ASTPrinter::PostVisit(DynamicNode*) {}

void ASTPrinter::PreVisit(TableExpression*)
{
  indent();
  out << "TableExpression" << std::endl;
}

void ASTPrinter::PostVisit(TableExpression*)
{
  deindent();
}

void ASTPrinter::PreVisit(TableElement*)
{
  indent();
  out << "TableElement" << std::endl;
}

void ASTPrinter::PostVisit(TableElement*)
{
  deindent();
}

inline void ASTPrinter::indent() {
  for (unsigned i = 0; i < level; ++i)
    out << '\t';
  ++level;
}

inline void ASTPrinter::deindent() {
  --level;
}
