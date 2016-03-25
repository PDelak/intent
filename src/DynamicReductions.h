// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INTENT_DYNAMIC_REDUCTIONS_H
#define INTENT_DYNAMIC_REDUCTIONS_H

#include <iostream>
#include <unordered_map>
#include <array>
#include "AST.h"

int common_reduction(int id, void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser);

template<int I>
struct Fn
{
  static int reduction(void *_ps, void **_children, int _n_children, int _offset, D_Parser *_parser)
  {    
    return common_reduction(I, _ps, _children, _n_children, _offset, _parser);    
  }
};

#define R(N) Fn<N>::reduction

typedef int (*reductionf) (void *, void **, int, int, D_Parser*);

std::array<reductionf, 1024> make_dynamic_reductions();

#endif
