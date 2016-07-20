// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INTENT_VISITOR_TOOLS_H
#define INTENT_VISITOR_TOOLS_H

#include <vector>
#include <string>
#include "Visitor.h"
#include "AST.h"

template<typename Stream>
std::pair<size_t, size_t> getRange(const Stream& stream)
{
  size_t size = stream.size();
  size_t endIndex = size - 1;
  size_t beginIndex = endIndex;

  while (stream[beginIndex].first != nullptr) {
    beginIndex--;
  }
  return std::make_pair(beginIndex, endIndex);
}

// pushes ObjectType argument on the stack, currently named Stream
template<typename ObjectType, typename Stream>
void shift(Stream& stream)
{
  ObjectType ot;
  stream.push_back(std::make_pair(nullptr, ot));
}

// get specified number of arguments from the stack and call supplied 
// user callback function - reduceFunction for customization
template<typename ObjectType, typename Stream, typename reduceFunction>
void reduce(Stream& stream, Node* node, reduceFunction f)
{
  size_t begin, end;
  std::tie(begin, end) = getRange(stream);
  ObjectType reducedStream;
  
  reducedStream = f(stream, begin + 1, end);
  
  size_t distance = end - begin;
  for (size_t i = 0; i <= distance; i++) {
    stream.pop_back();
  }
  stream.push_back(std::make_pair(node, reducedStream));
}

#endif
