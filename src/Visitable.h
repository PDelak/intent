// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VISITABLE_H
#define VISITABLE_H

class Visitor;

class Visitable
{
public:
  virtual void accept(Visitor* v) = 0;
  virtual ~Visitable() {}
};

#endif
