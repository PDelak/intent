// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef AST_PTRS_H
#define AST_PTRS_H

struct Node;
typedef std::shared_ptr<Node> NodePtr;

struct FunctionDecl;
typedef std::shared_ptr<FunctionDecl> FunctionDeclPtr;

struct FunctionExpr;
typedef std::shared_ptr<FunctionExpr> FunctionExprPtr;

struct StatementBase;
typedef std::shared_ptr<StatementBase> StatementBasePtr;

struct ExpressionBase;
typedef std::shared_ptr<ExpressionBase> ExpressionBasePtr;

struct Program;
typedef std::shared_ptr<Program> ProgramPtr;

struct Statement;
typedef std::shared_ptr<Statement> StatementPtr;

struct Expression;
typedef std::shared_ptr<Expression> ExpressionPtr;

struct LetStatement;
typedef std::shared_ptr<LetStatement> LetStatementPtr;

struct Identifier;
typedef std::shared_ptr<Identifier> IdentifierPtr;

struct StringExpr;
typedef std::shared_ptr<StringExpr> StringExprPtr;

struct Literal;
typedef std::shared_ptr<Literal> LiteralPtr;

struct NumberExpr;
typedef std::shared_ptr<NumberExpr> NumberExprPtr;

struct BoolExpr;
typedef std::shared_ptr<BoolExpr> BoolExprPtr;

struct TypeName;
typedef std::shared_ptr<TypeName> TypeNamePtr;

struct Parameter;
typedef std::shared_ptr<Parameter> ParameterPtr;

struct FunctionBody;
typedef std::shared_ptr<FunctionBody> FunctionBodyPtr;

struct FunctionCall;
typedef std::shared_ptr<FunctionCall> FunctionCallPtr;

struct CompoundStatement;
typedef std::shared_ptr<CompoundStatement> CompoundStatementPtr;

struct Argument;
typedef std::shared_ptr<Argument> ArgumentPtr;

struct ParenExpression;
typedef std::shared_ptr<ParenExpression> ParenExpressionPtr;

struct BinaryExpression;
typedef std::shared_ptr<BinaryExpression> BinaryExpressionPtr;

struct WhileLoop;
typedef std::shared_ptr<WhileLoop> WhileLoopPtr;

struct ForLoop;
typedef std::shared_ptr<ForLoop> ForLoopPtr;

struct IfStatement;
typedef std::shared_ptr<IfStatement> IfStatementPtr;

struct UnaryExpression;
typedef std::shared_ptr<UnaryExpression> UnaryExpressionPtr;

struct ReturnStatement;
typedef std::shared_ptr<ReturnStatement> ReturnStatementPtr;

struct TableExpression;
typedef std::shared_ptr<TableExpression> TableExpressionPtr;

struct TableElement;
typedef std::shared_ptr<TableElement> TableElementPtr;

struct MatchStatement;
typedef std::shared_ptr<MatchStatement> MatchStatementPtr;

struct MatchId;
typedef std::shared_ptr<MatchId> MatchIdPtr;

struct CodeExpression;
typedef std::shared_ptr<CodeExpression> CodeExpressionPtr;

struct MatchCase;
typedef std::shared_ptr<MatchCase> MatchCasePtr;

struct Extension;
typedef std::shared_ptr<Extension> ExtensionPtr;

struct DynamicNode;
typedef std::shared_ptr<DynamicNode> DynamicNodePtr;

#endif
