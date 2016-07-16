// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BUILTIN_REDUCTIONS_INTENT_H
#define BUILTIN_REDUCTIONS_INTENT_H

#include <iostream>
#include <fstream>
#include "AST.h"
#include "SemaChecker.h"
#include "NodeLinker.h"

extern ProgramPtr programPtr;

#include "dparse.h"

int program_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int compound_statement_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int statement_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int expression_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int expression_statement_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int if_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int if_else_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int while_loop_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int for_loop_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int binary_expr_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int binary_expr_paren_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int unary_expr_not_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int unary_expr_table_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int let_statement_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int type_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int parameter_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int argument_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int function_declaration_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int function_expression_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int function_body_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int function_call_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int return_statement_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int bool_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int table_element_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int literal_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int table_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int identifier_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int number_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int string_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int matchset_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int match_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int matchcase_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int matchid_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int code_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);
int extension_reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);

#endif
