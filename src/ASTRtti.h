// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef AST_RTTI_H
#define AST_RTTI_H

#include "AST.h"

template<typename T> struct ast_type_checker;

template<> struct ast_type_checker<Node>              
{ 
  static bool is(NodePtr p) { return NodeAST == p->getId(); } 
  static bool is(Node* p)   { return NodeAST == p->getId(); }
};
template<> struct ast_type_checker<ReturnStatement>   
{ 
  static bool is(NodePtr p) { return ReturnStatementAST == p->getId(); } 
  static bool is(Node* p)   { return ReturnStatementAST == p->getId(); }
};

template<> struct ast_type_checker<Program>           
{ 
  static bool is(NodePtr p) { return ProgramAST == p->getId(); } 
  static bool is(Node* p)   { return ProgramAST == p->getId(); }
};

template<> struct ast_type_checker<Statement>         
{ 
  static bool is(NodePtr p) { return StatementAST == p->getId(); } 
  static bool is(Node* p)   { return StatementAST == p->getId(); }
};

template<> struct ast_type_checker<Expression>        
{ 
  static bool is(NodePtr p) { return ExpressionAST == p->getId(); } 
  static bool is(Node* p)   { return ExpressionAST == p->getId(); }
};

template<> struct ast_type_checker<ExpressionStatement>
{
    static bool is(NodePtr p) { return ExpressionStatementAST == p->getId(); }
    static bool is(Node* p) { return ExpressionStatementAST == p->getId(); }
};

template<> struct ast_type_checker<LetStatement>      
{ 
  static bool is(NodePtr p) { return LetStatementAST == p->getId(); } 
  static bool is(Node* p)   { return LetStatementAST == p->getId(); }
};

template<> struct ast_type_checker<Identifier>        
{ 
  static bool is(NodePtr p) { return IdentifierAST == p->getId(); } 
  static bool is(Node* p)   { return IdentifierAST == p->getId(); }
};

template<> struct ast_type_checker<StringExpr>        
{ 
  static bool is(NodePtr p) { return StringExprAST == p->getId(); } 
  static bool is(Node* p)   { return StringExprAST == p->getId(); }
};

template<> struct ast_type_checker<Literal>           
{ 
  static bool is(NodePtr p) { return LiteralAST == p->getId(); } 
  static bool is(Node* p)   { return LiteralAST == p->getId(); }
};

template<> struct ast_type_checker<NumberExpr>        
{ 
  static bool is(NodePtr p) { return NumberExprAST == p->getId(); } 
  static bool is(Node* p)   { return NumberExprAST == p->getId(); }
};

template<> struct ast_type_checker<BoolExpr>          
{ 
  static bool is(NodePtr p) { return BoolExprAST == p->getId(); } 
  static bool is(Node* p)   { return BoolExprAST == p->getId(); }
};

template<> struct ast_type_checker<TypeName>          
{ 
  static bool is(NodePtr p) { return TypeNameAST == p->getId(); } 
  static bool is(Node* p)   { return TypeNameAST == p->getId(); }
};

template<> struct ast_type_checker<Parameter>         
{ 
  static bool is(NodePtr p) { return ParameterAST == p->getId(); } 
  static bool is(Node* p)   { return ParameterAST == p->getId(); }
};

template<> struct ast_type_checker<FunctionBody>      
{ 
  static bool is(NodePtr p) { return FunctionBodyAST == p->getId(); } 
  static bool is(Node* p)   { return FunctionBodyAST == p->getId(); }
};

template<> struct ast_type_checker<FunctionDecl>      
{ 
  static bool is(NodePtr p) { return FunctionDeclAST == p->getId(); } 
  static bool is(Node* p)   { return FunctionDeclAST == p->getId(); }
};

template<> struct ast_type_checker<FunctionCall>      
{ 
  static bool is(NodePtr p) { return FunctionCallAST == p->getId(); } 
  static bool is(Node* p)   { return FunctionCallAST == p->getId(); }
};

template<> struct ast_type_checker<CompoundStatement> 
{ 
  static bool is(NodePtr p) { return CompoundStatementAST == p->getId(); } 
  static bool is(Node* p)   { return CompoundStatementAST == p->getId(); }
};

template<> struct ast_type_checker<Argument>          
{ 
  static bool is(NodePtr p) { return ArgumentAST == p->getId(); } 
  static bool is(Node* p)   { return ArgumentAST == p->getId(); }
};

template<> struct ast_type_checker<ParenExpression>   
{ 
  static bool is(NodePtr p) { return ParenExpressionAST == p->getId(); } 
  static bool is(Node* p)   { return ParenExpressionAST == p->getId(); }
};

template<> struct ast_type_checker<BinaryExpression>  
{ 
  static bool is(NodePtr p) { return BinaryExpressionAST == p->getId(); } 
  static bool is(Node* p)   { return BinaryExpressionAST == p->getId(); }
};

template<> struct ast_type_checker<WhileLoop>         
{ 
  static bool is(NodePtr p) { return WhileLoopAST == p->getId(); } 
  static bool is(Node* p)   { return WhileLoopAST == p->getId(); }
};

template<> struct ast_type_checker<ForLoop>           
{ 
  static bool is(NodePtr p) { return ForLoopAST == p->getId(); } 
  static bool is(Node* p)   { return ForLoopAST == p->getId(); }
};

template<> struct ast_type_checker<IfStatement>       
{ 
  static bool is(NodePtr p) { return IfStatementAST == p->getId(); } 
  static bool is(Node* p)   { return IfStatementAST == p->getId(); }
};

template<> struct ast_type_checker<UnaryExpression>   
{ 
  static bool is(NodePtr p) { return UnaryExpressionAST == p->getId(); } 
  static bool is(Node* p)   { return UnaryExpressionAST == p->getId(); }
};

template<> struct ast_type_checker<TableExpression>   
{ 
  static bool is(NodePtr p) { return TableExpressionAST == p->getId(); } 
  static bool is(Node* p)   { return TableExpressionAST == p->getId(); }
};

template<> struct ast_type_checker<TableElement>      
{ 
  static bool is(NodePtr p) { return TableElementAST == p->getId(); } 
  static bool is(Node* p)   { return TableElementAST == p->getId(); }
};

template<> struct ast_type_checker<MatchStatement>
{
  static bool is(NodePtr p) { return MatchStatementAST == p->getId(); }
  static bool is(Node* p) { return MatchStatementAST == p->getId(); }
};

template<> struct ast_type_checker<MatchId>
{
  static bool is(NodePtr p) { return MatchIdAST == p->getId(); }
  static bool is(Node* p) { return MatchIdAST == p->getId(); }
};

template<> struct ast_type_checker<CodeExpression>
{
  static bool is(NodePtr p) { return CodeExpressionAST == p->getId(); }
  static bool is(Node* p) { return CodeExpressionAST == p->getId(); }
};

template<> struct ast_type_checker<MatchCase>
{
  static bool is(NodePtr p) { return MatchCaseAST == p->getId(); }
  static bool is(Node* p) { return MatchCaseAST == p->getId(); }
};

template<> struct ast_type_checker<Extension>
{
  static bool is(NodePtr p) { return ExtensionAST == p->getId(); }
  static bool is(Node* p) { return ExtensionAST == p->getId(); }
};

template<> struct ast_type_checker<DynamicNode>
{
  static bool is(NodePtr p) { return DynamicNodeAST == p->getId(); }
  static bool is(Node* p) { return DynamicNodeAST == p->getId(); }
};

template<typename T>
bool is(NodePtr p) { return ast_type_checker<T>::is(p); }

template<typename T>
bool is(Node* p) { return ast_type_checker<T>::is(p); }

#endif
