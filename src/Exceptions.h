// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INTENT_EXCEPTIONS_H
#define INTENT_EXCEPTIONS_H

#include <stdexcept>

class FileNotFoundException : public std::runtime_error
{
public:
  FileNotFoundException(const char* what);
};

class SyntaxError : public std::runtime_error
{
public:
  SyntaxError(const char* what);
};

#endif
