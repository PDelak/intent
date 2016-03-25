// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "Exceptions.h"

FileNotFoundException::FileNotFoundException(const char* what) 
  :std::runtime_error(what) 
{}

SyntaxError::SyntaxError(const char* what) 
  :std::runtime_error(what) 
{}
