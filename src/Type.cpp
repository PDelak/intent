// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include "Type.h" 

TypePtr type_instance<type>::t;
TypePtr type_instance<string_type>::t;
TypePtr type_instance<number_type>::t;
TypePtr type_instance<bool_type>::t;
TypePtr type_instance<generic_type>::t;
TypePtr type_instance<function_type>::t;
TypePtr type_instance<table_type>::t;
TypePtr type_instance<any_type>::t;
TypePtr type_instance<void_type>::t;

std::string type::to_string() const { return "type"; }
type::~type() {} 

std::string string_type::to_string() const { return "string"; }
std::string number_type::to_string() const { return "number"; }
std::string bool_type::to_string() const { return "bool"; }
std::string generic_type::to_string() const { return "generic"; }
std::string table_type::to_string() const { return "table"; }
std::string any_type::to_string() const { return "any"; }
std::string void_type::to_string() const { return "void"; }

bool operator == (const type& lhs, const type& rhs) 
{ return typeid(lhs) == typeid(rhs); }

bool operator != (const type& lhs, const type& rhs)
{ return typeid(lhs) != typeid(rhs); }

bool equal(const type* lhs, const type* rhs) 
{ return *lhs == *rhs; }

bool not_equal(const type* lhs, const type* rhs)
{ return *lhs == *rhs; }

bool equal(const TypePtr& lhs, const TypePtr& rhs)
{ return *lhs == *rhs; }

bool not_equal(const TypePtr& lhs, const TypePtr& rhs)
{ return *lhs != *rhs; }


