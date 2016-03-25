// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "BuiltinReductions.h"
#include "dparse.h"

int program_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  programPtr->setExpressions(&(*(D_PN(_children[0], _offset))));

  return 0;
}

int compound_statement_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  CompoundStatementPtr p(new CompoundStatement(&(*(D_PN(_children[1], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int statement_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  StatementPtr p(new Statement(&(*(D_PN(_children[0], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int expression_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  ExpressionPtr p(new Expression(&(*(D_PN(_children[0], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int if_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  IfStatementPtr p(new IfStatement(&(*(D_PN(_children[2], _offset))), &(*(D_PN(_children[4], _offset))), 0)); 
  (D_PN(_ps, _offset)->user).p = p;
  
  return 0;
}

int if_else_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  IfStatementPtr p(new IfStatement(&(*(D_PN(_children[2], _offset))), &(*(D_PN(_children[4], _offset))), &(*(D_PN(_children[6], _offset))))); 
  (D_PN(_ps, _offset)->user).p = p;
  
  return 0;
}

int while_loop_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  WhileLoopPtr p(new WhileLoop(&(*(D_PN(_children[2], _offset))), &(*(D_PN(_children[4], _offset))))); 
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int for_loop_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  ForLoopPtr p(new ForLoop(&(*(D_PN(_children[2], _offset))), &(*(D_PN(_children[4], _offset))), &(*(D_PN(_children[6], _offset))), &(*(D_PN(_children[8], _offset))))); 
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int binary_expr_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  BinaryExpressionPtr p(new BinaryExpression(&(*(D_PN(_children[0], _offset))), &(*(D_PN(_children[2], _offset))), std::string((*(D_PN(_children[1], _offset))).start_loc.s, (*(D_PN(_children[1], _offset))).end))); 
  (D_PN(_ps, _offset)->user).p = p;
  
  return 0;
}

int binary_expr_paren_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  ParenExpressionPtr p(new ParenExpression(&(*(D_PN(_children[1], _offset))))); 
  (D_PN(_ps, _offset)->user).p = p;
  
  return 0;
}

int unary_expr_not_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  UnaryExpressionPtr p(new UnaryExpression(&(*(D_PN(_children[1], _offset))), "not", PREFIX)); 
  (D_PN(_ps, _offset)->user).p = p;
  
  return 0;
}

int unary_expr_table_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  UnaryExpressionPtr p(new UnaryExpression(&(*(D_PN(_children[0], _offset))), &(*(D_PN(_children[1], _offset))), "index", PREFIX));
  (D_PN(_ps, _offset)->user).p = p;
  
  return 0;
}

int let_statement_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  LetStatementPtr p(new LetStatement(&(*(D_PN(_children[1], _offset))), &(*(D_PN(_children[2], _offset))), &(*(D_PN(_children[4], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int type_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  TypeNamePtr p(new TypeName(&(*(D_PN(_children[0], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int parameter_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  ParameterPtr p(new Parameter(&(*(D_PN(_children[0], _offset))), &(*(D_PN(_children[1], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int argument_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  ArgumentPtr p(new Argument(&(*(D_PN(_children[0], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int function_declaration_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  FunctionDeclPtr p(new FunctionDecl(&(*(D_PN(_children[1], _offset))), &(*(D_PN(_children[3], _offset))), &(*(D_PN(_children[4], _offset))), &(*(D_PN(_children[6], _offset))), &(*(D_PN(_children[7], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int function_body_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  FunctionBodyPtr p(new FunctionBody(&(*(D_PN(_children[0], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int function_call_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  FunctionCallPtr p(new FunctionCall(&(*(D_PN(_children[0], _offset))), &(*(D_PN(_children[2], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int return_statement_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  ReturnStatementPtr p(new ReturnStatement(&(*(D_PN(_children[1], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int bool_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  BoolExprPtr p(new BoolExpr(&(*(D_PN(_children[0], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int table_element_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  TableElementPtr p(new TableElement(&(*(D_PN(_children[1], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int literal_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  LiteralPtr p(new Literal(&(*(D_PN(_children[0], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int table_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  TableExpressionPtr p(new TableExpression(&(*(D_PN(_children[1], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int identifier_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  IdentifierPtr p(new Identifier((*(D_PN(_children[0], _offset))).start_loc.s, (*(D_PN(_children[0], _offset))).end, (*(D_PN(_children[0], _offset))).start_loc.line));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int number_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  NumberExprPtr p(new NumberExpr((*(D_PN(_children[0], _offset))).start_loc.s, (*(D_PN(_children[0], _offset))).end));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int string_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  StringExprPtr p(new StringExpr((*(D_PN(_children[0], _offset))).start_loc.s, (*(D_PN(_children[0], _offset))).end));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int match_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  MatchStatementPtr p(new MatchStatement(&(*(D_PN(_children[2], _offset))), &(*(D_PN(_children[5], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}

int matchcase_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  MatchCasePtr p(new MatchCase(&(*(D_PN(_children[1], _offset))), &(*(D_PN(_children[4], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;
  return 0;
}

int matchid_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  MatchIdPtr p(new MatchId((*(D_PN(_children[0], _offset))).start_loc.s, (*(D_PN(_children[0], _offset))).end));
  (D_PN(_ps, _offset)->user).p = p;
  return 0;
}

int code_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  CodeExpressionPtr p(new CodeExpression((*(D_PN(_children[0], _offset))).start_loc.s, (*(D_PN(_children[0], _offset))).end));
  (D_PN(_ps, _offset)->user).p = p;
  return 0;
}

int extension_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
{
  ExtensionPtr p(new Extension(&(*(D_PN(_children[0], _offset)))));
  (D_PN(_ps, _offset)->user).p = p;

  return 0;
}
