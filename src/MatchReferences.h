// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef MATCH_REFERENCES_INTENT_H
#define MATCH_REFERENCES_INTENT_H

#include <string>
#include <iterator>
#include <list>
#include <vector>
#include <unordered_map>
#include "AST.h"

typedef std::pair<std::string::const_iterator, std::string::const_iterator> StringRange;

StringRange findOneReference(std::string::const_iterator begin, 
                             std::string::const_iterator end);

std::list<StringRange> findAllReferences(std::string::const_iterator begin, 
                                         std::string::const_iterator end);

std::vector<std::string::const_iterator> findAllRangesStack(std::string::const_iterator begin, 
                                                            std::string::const_iterator end);

std::list<StringRange> findAllRangesList(const std::vector<std::string::const_iterator>& points);

std::unordered_map<size_t, size_t> getReferenceIndexes(std::vector<std::string::const_iterator>& points);

std::unordered_map<size_t, D_ParseNode*> generateD_ParseNodeTable(const std::unordered_map<size_t, size_t>& indexes, 
                                                                  void **_children, 
                                                                  int _offset);

#endif
