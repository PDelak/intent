// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INTENT_TYPE_H
#define INTENT_TYPE_H

#include <memory>
#include <vector>
#include "ASTFwd.h"

struct type;
typedef std::shared_ptr<type> TypePtr;

struct type { 
  virtual std::string to_string() const;
  virtual ~type();
};

struct string_type : public type 
{
  virtual std::string to_string() const;
};

struct number_type : public type 
{
  virtual std::string to_string() const;
};

struct bool_type : public type 
{
  virtual std::string to_string() const;
};

struct generic_type : public type 
{
  virtual std::string to_string() const;
};

struct function_type : public type 
{
  std::vector<std::pair<std::string, TypePtr>> parameters;
  NodePtr function_body;
  TypePtr return_type;
};

struct table_type : public type
{
	virtual std::string to_string() const;
};


struct any_type : public type
{
	virtual std::string to_string() const;
};

struct void_type : public type
{
	virtual std::string to_string() const;
};


bool operator == (const type& lhs, const type& rhs);
bool operator != (const type& lhs, const type& rhs);
bool equal(const type* lhs, const type* rhs);
bool not_equal(const type* lhs, const type* rhs);
bool equal(const TypePtr& lhs, const TypePtr& rhs);
bool not_equal(const TypePtr& lhs, const TypePtr& rhs);

template<typename T>
struct type_instance
{
	static TypePtr get();
};

template<>
struct type_instance<type>
{
	static TypePtr get() { if (!t) t.reset(new string_type); return t; }
private:
	static TypePtr t;
};

template<>
struct type_instance<string_type>
{
	static TypePtr get() { if (!t) t.reset(new string_type); return t; }
private:
	static TypePtr t;
};

template<>
struct type_instance<number_type>
{
	static TypePtr get() { if (!t) t.reset(new number_type); return t; }
private:
	static TypePtr t;
};

template<>
struct type_instance<bool_type>
{
	static TypePtr get() { if (!t) t.reset(new bool_type); return t; }
private:
	static TypePtr t;
};

template<>
struct type_instance<generic_type>
{
	static TypePtr get() { if (!t) t.reset(new generic_type); return t; }
private:
	static TypePtr t;
};

template<>
struct type_instance<function_type>
{
	static TypePtr get() { if (!t) t.reset(new function_type); return t; }
private:
	static TypePtr t;
};

template<>
struct type_instance<table_type>
{
	static TypePtr get() { if (!t) t.reset(new table_type); return t; }
private:
	static TypePtr t;
};

template<>
struct type_instance<any_type>
{
	static TypePtr get() { if (!t) t.reset(new any_type); return t; }
private:
	static TypePtr t;
};

template<>
struct type_instance<void_type>
{
	static TypePtr get() { if (!t) t.reset(new void_type); return t; }
private:
	static TypePtr t;
};

template<typename T>
TypePtr get_type_instance() { return type_instance<T>::get(); }

#endif
