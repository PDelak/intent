// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "MatchReferences.h"
#include "AST.h"
#include <cctype>

StringRange findOneReference(std::string::const_iterator begin, 
                             std::string::const_iterator end)
{
  std::string::const_iterator b(begin), e(end);
  auto it = std::find(b, e, '$');
  if (it == end) return std::make_pair(end, end);
  b = it;
  e = it;
  e++;
  while (e != end && (isalnum(*e) || (*e == '.'))) {
    e++;
  }
  return std::make_pair(b, e);

}

std::list<StringRange> findAllReferences(std::string::const_iterator begin, 
                                         std::string::const_iterator end)
{
  std::list<StringRange> rangeList;
  auto b = begin;
  auto e = end;
  while (b != e) {
    auto range = findOneReference(b, e);
    if (range.first != e) {
      rangeList.push_back(range);
      b = range.second;
    }
    else b++;
  }
  return rangeList;
}


std::vector<std::string::const_iterator> findAllRangesStack(std::string::const_iterator begin, 
                                                            std::string::const_iterator end)
{
  std::vector<std::string::const_iterator> points;
  points.push_back(begin);
  auto l = findAllReferences(begin, end);
  
  auto itSecond = end;
  for (auto element : l) {
    points.push_back(element.first);
    points.push_back(element.second);
    itSecond = element.second;
  }
  if (itSecond != end) points.push_back(end);
  return points;
}


std::list<StringRange> findAllRangesList(const std::vector<std::string::const_iterator>& points)
{
  std::list<StringRange> rangeList;
  for (size_t i = 0; i < points.size() - 1; i++) {
    rangeList.push_back(std::make_pair(points[i], points[i + 1]));
  }
  return rangeList;
}

std::unordered_map<size_t, size_t> getReferenceIndexes(std::vector<std::string::const_iterator>& points)
{
  std::unordered_map<size_t, size_t> indexes;
  for (size_t i = 0; i < points.size() - 1; i++) {
    if (*points[i] == '$') {
      std::string ref = std::string(++points[i], points[i + 1]);
      size_t value = std::stoi(ref);
      indexes[i] = value;
    }
  }
  return indexes;
}

std::unordered_map<size_t, D_ParseNode*> generateD_ParseNodeTable(const std::unordered_map<size_t, size_t>& indexes, 
                                                                  void **_children, 
                                                                  int _offset)
{
  std::unordered_map<size_t, D_ParseNode*> table;
  for (auto element : indexes) {
    table[element.first] = &(*(D_PN(_children[element.second], _offset)));
  }
  return table;
}

