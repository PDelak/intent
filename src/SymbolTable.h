// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BASIC_SYMBOL_TABLE_H
#define BASIC_SYMBOL_TABLE_H

#include <unordered_map>
#include <list>
#include <string>
#include <utility>
#include "Type.h"

typedef std::pair<std::string, TypePtr> symbol;

typedef std::list<symbol> symbol_list;

typedef std::list<symbol>::iterator symbol_iterator;

typedef std::unordered_map<size_t, symbol_list> basic_symbol_table_type;

typedef std::unordered_map<size_t, symbol_list>::iterator symbol_table_iterator;

struct SymbolNotFound : public std::runtime_error
{
  SymbolNotFound(const std::string& symbol, const std::string lineno) 
  :std::runtime_error("Undefined symbol : " + symbol + " in " + lineno + " line ") {}
};


struct basic_symbol_table
{
  basic_symbol_table():symbol_table_id(0) {}

  void enterScope()
  {
    ++symbol_table_id;
  }

  void exitScope()
  {
    symbol_table_iterator table_iterator = symbol_table.find(symbol_table_id);
    if (table_iterator != symbol_table.end()) symbol_table.erase(table_iterator);
    --symbol_table_id;
  }
  
  void insertSymbol(const std::string& symbol, const TypePtr& type)
  {
    symbol_table[symbol_table_id].push_back(std::make_pair(symbol, type));
  }
  
  std::pair<std::string, TypePtr> findSymbol(const std::string& symbol, size_t lineno)
  {
    int local_table_id = symbol_table_id;
    while (local_table_id >= 0) {
      symbol_iterator it = symbol_table[local_table_id].begin();
      while (it != symbol_table[local_table_id].end()) {
        if (it->first.compare(symbol) == 0) return *it;
        it++;
      }
      --local_table_id;
    }
    std::stringstream ss;
    ss << lineno;
    throw SymbolNotFound(symbol, ss.str());
  }

  size_t symbol_table_id;
  basic_symbol_table_type symbol_table;

};

#endif
