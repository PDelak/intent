// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <iostream>
#include "AST.h"
#include "ASTRtti.h"
#include "SymbolTable.h"
#include "MatchReferences.h"

#include "dparse.h"

ProgramPtr programPtr;
int Node::level = 0;
basic_symbol_table Node::symbol_table;
bool Node::resolveTypesPhase = true;

SemanticError::SemanticError(const std::string& errorMessage) 
  :std::runtime_error(errorMessage) {}

Node::Node()
  :parent(NULL)
  ,lhs(false)
  ,rhs(false) { }

void Node::indent()   
{ ++level; }

void Node::deindent() 
{ --level; }

NodeType Node::getId() 
{ return NodeAST; }

void Node::emitWithIndentation(std::string& out, const std::string& body) {
  for (int i = 0; i < level; ++i)
    out += '\t';
  out += body;
}

std::string Node::serialize(const ASTNodeDecorator& decorator = ASTNodeDecorator())
{
  if (!this) return "";
  std::string out;
  bool value = false;
  if (decorator.fun) value = decorator.fun(this);

  if (value) out += decorator.before;
  out += this->serializeImpl(decorator);
  if (value) out += decorator.after;
  return out;
}


Program::Program(D_ParseNode* expressions) 
  :m_expressions(expressions)
{
  // insert all built-in functions into symbol table
  TypePtr printFunction(new function_type);
  function_type* ftype = static_cast<function_type*>(printFunction.get());
  TypePtr generic(new generic_type);
  ftype->parameters.push_back(std::make_pair("",generic));
  symbol_table.insertSymbol("@print", printFunction);
}

void Program::setOutput(const std::string& out) 
{ m_output = out; }

void Program::setExpressions(D_ParseNode* expressions) 
{ m_expressions = expressions; }

void Program::setIntermediateName(const std::string& i) 
{ m_intermediateName = i; }

std::string Program::intermediateName() const 
{ return m_intermediateName; }

bool Program::isTest() 
{ return !m_output.empty(); }

void Program::test() {
	
  if (m_output.empty()) return;

  std::string out = serialize();      
  out.erase(out.find_last_not_of(" \n\r\t") + 1);
  m_output.erase(m_output.find_last_not_of(" \n\r\t") + 1);
  if (out.compare(m_output) != 0) {
    std::cout << "test failed : " << m_intermediateName << std::endl;
    throw 1;
  }
    
}

void Program::generate(std::ofstream& o) {
  resolveTypesPhase = false;
  o << serialize().c_str();
}

NodeType Program::getId() 
{ return ProgramAST; }

Statement::Statement(D_ParseNode* statement) 
  :m_statement(statement) {}

NodeType Statement::getId() 
{ return StatementAST; }

Expression::Expression(D_ParseNode* expression = 0) 
  :m_expression(expression) 
{}

NodeType Expression::getId() 
{ return ExpressionAST; }

LetStatement::LetStatement(D_ParseNode* identifier, 
                           D_ParseNode* type, 
                           D_ParseNode* declaration)
  :m_identifier(identifier)
  ,m_type(type)
  ,m_declaration(declaration)
{}


NodeType LetStatement::getId() 
{ return LetStatementAST; }

Identifier::Identifier(const char* begin, 
                       const char* end, 
                       size_t line) 
  :id(begin, end)
  ,m_line(line) {}

NodeType Identifier::getId() 
{ return IdentifierAST; }

StringExpr::StringExpr(const char* begin, 
                       const char* end) 
  :id(begin, end)
{}

NodeType StringExpr::getId() 
{ return StringExprAST; }

Literal::Literal(D_ParseNode* id) 
  :m_id(id)
{}

NodeType Literal::getId() 
{ return LiteralAST; }

NumberExpr::NumberExpr(const char* begin, 
                       const char* end) 
  :number(begin, end) 
{}

NodeType NumberExpr::getId() 
{ return NumberExprAST; }

BoolExpr::BoolExpr(D_ParseNode* value)
{
  D_ParseNode *xpn = d_get_child(value, 0);
  m_value = std::string(xpn->start_loc.s, xpn->end);
}

NodeType BoolExpr::getId() 
{ return BoolExprAST; }

TypeName::TypeName(D_ParseNode* value)
{
  D_ParseNode *xpn = d_get_child(value, 0);
  m_value = std::string(xpn->start_loc.s, xpn->end);
}

NodeType TypeName::getId() 
{ return TypeNameAST; }

Parameter::Parameter(D_ParseNode* identifier, 
                     D_ParseNode* type)
{
  D_ParseNode *xpn = d_get_child(identifier, 0);
  m_identifier = std::string(xpn->start_loc.s, xpn->end);
  m_type = type;
  D_ParseNode *typeGroup = d_get_child(m_type, 0);
  if (typeGroup) {
    D_ParseNode *type = d_get_child(typeGroup, 1);
    if (type) {
      std::string stype = type->user.p->serialize();
      if (stype.compare("string") == 0) ptype.reset(new string_type);
      else if (stype.compare("bool") == 0) ptype.reset(new bool_type);
      else if (stype.compare("int") == 0) ptype.reset(new number_type);
    }
  }
  else ptype.reset(new generic_type);
}

TypePtr Parameter::getType() {
  return ptype;
}

NodeType Parameter::getId() 
{ return ParameterAST; }

FunctionBody::FunctionBody(D_ParseNode* expression) 
  : m_expressions(expression) 
{}

NodeType FunctionBody::getId() 
{ return FunctionBodyAST; }

FunctionDecl::FunctionDecl(D_ParseNode* identifier, 
                           D_ParseNode* firstParameter, 
                           D_ParseNode* otherParameters, 
                           D_ParseNode* returnType, 
                           D_ParseNode* body)
: m_identifier(identifier)
, m_firstParameter(firstParameter)
, m_otherParameters(otherParameters)
, m_returnType(returnType)
, m_body(body)
{}

NodeType FunctionDecl::getId() 
{ return FunctionDeclAST; }

FunctionExpr::FunctionExpr(
  D_ParseNode* firstParameter,
  D_ParseNode* otherParameters,
  D_ParseNode* returnType,
  D_ParseNode* body)
  : m_firstParameter(firstParameter)
  , m_otherParameters(otherParameters)
  , m_returnType(returnType)
  , m_body(body)
{}

NodeType FunctionExpr::getId()
{
  return FunctionExprAST;
}


FunctionCall::FunctionCall(D_ParseNode* identifier, 
                           D_ParseNode* args) 
  : m_identifier(identifier)
  , m_args(args) {}

NodeType FunctionCall::getId() 
{ return FunctionCallAST; }

CompoundStatement::CompoundStatement(D_ParseNode* expressions) 
  :m_expressions(expressions) 
{}

NodeType CompoundStatement::getId() 
{ return CompoundStatementAST; }

Argument::Argument(D_ParseNode* args) 
  :m_args(args) 
{}

NodeType Argument::getId() 
{ return ArgumentAST; }

ParenExpression::ParenExpression(D_ParseNode* parenExpr) 
  : m_parenExpr(parenExpr) 
{}

NodeType ParenExpression::getId() 
{ return ParenExpressionAST; }

BinaryExpression::BinaryExpression(D_ParseNode* left, 
                                   D_ParseNode* right, 
                                   const std::string & op) 
  : m_left(left)
  , m_right(right)
  , m_op(op)
{
  m_left->user.p->setLHS(true);
  m_right->user.p->setRHS(true);
}

NodeType BinaryExpression::getId() 
{ return BinaryExpressionAST; }

WhileLoop::WhileLoop(D_ParseNode* logical_expr, 
                     D_ParseNode* body) 
  : m_logical_expr(logical_expr)
  , m_body(body) 
{}

NodeType WhileLoop::getId() 
{ return WhileLoopAST; }

ForLoop::ForLoop(D_ParseNode* init_expr, 
                 D_ParseNode* cond_expr, 
                 D_ParseNode* step_expr, 
                 D_ParseNode* body)
  : m_init_expr(init_expr)
  , m_cond_expr(cond_expr)
  , m_step_expr(step_expr)
  , m_body(body) 
{}

NodeType ForLoop::getId() 
{ return ForLoopAST; }

IfStatement::IfStatement(D_ParseNode* logical_expr, 
                         D_ParseNode* thenExpr, 
                         D_ParseNode* elseExpr)
: m_logical_expr(logical_expr)
, m_thenExpr(thenExpr)
, m_elseExpr(elseExpr) 
{}

NodeType IfStatement::getId() 
{ return IfStatementAST; }

UnaryExpression::UnaryExpression(D_ParseNode* expression, 
                                 const std::string& op, 
                                 OperatorType opType) 
  : m_expression(expression)
  , m_index(nullptr)
  , m_op(op)
  , m_opType(opType) 
{}

UnaryExpression::UnaryExpression(D_ParseNode* expression,
  D_ParseNode* index,
  const std::string& op,
  OperatorType opType)
  : m_expression(expression)
  , m_index(index)
  , m_op(op)
  , m_opType(opType)
{}


NodeType UnaryExpression::getId() 
{ return UnaryExpressionAST; }

ReturnStatement::ReturnStatement(D_ParseNode* expression) 
  : m_expression(expression) 
{}

NodeType ReturnStatement::getId() 
{ return ReturnStatementAST; }

TableExpression::TableExpression(D_ParseNode* expression) 
  : m_expression(expression) 
{}

NodeType TableExpression::getId() 
{ return TableExpressionAST; }

TableElement::TableElement(D_ParseNode* expression) 
  : m_expression(expression) 
{}

NodeType TableElement::getId() 
{ return TableElementAST; }

NullStatement::NullStatement() 
{}

MatchStatement::MatchStatement(D_ParseNode* id, 
                               D_ParseNode* mCase)
  : matchId(id)
  , matchCase(mCase) 
{}

NodeType MatchStatement::getId() 
{ return MatchStatementAST; }

MatchId::MatchId(const char* begin, 
                 const char* end) 
  : id(begin, end) 
{}

NodeType MatchId::getId() 
{ return MatchIdAST; }

CodeExpression::CodeExpression(const char* begin, 
                               const char* end) 
  : m_code(begin, end) 
{}

NodeType CodeExpression::getId() 
{ return CodeExpressionAST; }

MatchCase::MatchCase(D_ParseNode* id, 
                     D_ParseNode* code) 
  : m_id(id)
  , m_codeExpr(code) 
{}

NodeType MatchCase::getId() 
{ return MatchCaseAST; }

std::string MatchCase::matchId() const
{
  D_ParseNode *xpn = d_get_child(m_id, 0);
  if (xpn) {
    MatchId* id = static_cast<MatchId*>(xpn->user.p.get());
    if (id) return id->getIdentifier();
  }
  return "";
}

std::string MatchCase::codeExpr() const
{
  CodeExpression* codeE = static_cast<CodeExpression*>(m_codeExpr->user.p.get());
  return codeE->code();
}

Extension::Extension(D_ParseNode* main) 
  : m_main(main) 
{}

NodeType Extension::getId() 
{ return ExtensionAST; }

void DynamicNode::setValueNodes(const std::list<StringRange>& code_ranges, int offset)
{
  size_t index = 0;
  for (const auto& val : code_ranges) {
    auto range = std::string(val.first, val.second);
    size_t begin = range.find('$');
    size_t end = range.find(".value");
    if (begin != std::string::npos && end != std::string::npos) {
      std::string ref = std::string(range.begin() + begin + 1, range.begin() + end);
      size_t value = std::stoi(ref);
      m_values[index] = &(*(D_PN(m_children[value], offset)));
    }
    index++;
  }
}

DynamicNode::DynamicNode(const std::string& code, 
                         void** children, 
                         int offset)
  : m_code(code)
  , m_children(children)
  , m_offset(offset)
{
  // that's just silly fix due to findAllRangesStack fix
  // it does not work correctly if reference is first element
  // in code stream
  m_code = "\n" + m_code;
  auto v = findAllRangesStack(m_code.begin(), m_code.end());
  auto code_ranges = findAllRangesList(v);

  setValueNodes(code_ranges, offset);

  mcode_ranges = code_ranges;

  // if there will be no ranges, put whole code
  if (mcode_ranges.empty()) { mcode_ranges.push_back(std::make_pair(m_code.begin(), m_code.end())); }
  
  auto indexes = getReferenceIndexes(v);
  m_childs = generateD_ParseNodeTable(indexes, m_children, m_offset);
  
}

NodeType DynamicNode::getId() 
{ return DynamicNodeAST; }

std::string DynamicNode::generateCode()
{
  auto l = codeRanges();
  std::string code;
  int index = 0;
  for (auto element : l) {
    auto values_it = m_values.find(index);

    if (values_it != m_values.end()) {
      if (values_it->second) {
        std::string c = std::string(values_it->second->start_loc.s, values_it->second->end);
        code += c;
      }
    }

    auto it = m_childs.find(index);
    if (it == m_childs.end()) code += std::string(element.first, element.second);
    
    if (it != m_childs.end()) {
      D_ParseNode *group = it->second;
      
      // currently builtin types and dynamic types are not composable
      // that's check agains DynamicNode Type
      if (group && 
          group->user.p && 
          group->user.p.get() &&
          is<DynamicNode>(group->user.p.get()) ) 
      {        
        std::string child_code = static_cast<DynamicNode*>(group->user.p.get())->generateCode();
        code += child_code;
        code += " ";
      }
      if (group && !group->user.p) {
        int nch = d_get_number_of_children(group);
        for (int i = 0; i < nch; i++) {
          D_ParseNode *child = d_get_child(group, i);
          std::string child_code;

          // currently builtin types and dynamic types are not composable
          // that's check agains DynamicNode Type
          if(child && 
             child->user.p && 
             is<DynamicNode>(child->user.p.get())) child_code = static_cast<DynamicNode*>(child->user.p.get())->generateCode();
          
          code += child_code;
          code += " ";
        }
      }
    }
    index++;
  }
  return code;
}
