// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "AST.h"
#include "SemaChecker.h"
#include <cassert>
#include "VisitorTools.h"

SemanticChecker::SemanticChecker() {}

void SemanticChecker::PreVisit(Program* node) 
{
  shift(stream);
}

void SemanticChecker::PostVisit(Program* node) 
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void SemanticChecker::PreVisit(Statement* node) 
{
  shift(stream);
}

void SemanticChecker::PostVisit(Statement* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
      r += '\n';
    }
    return r;
  });
}

void SemanticChecker::PreVisit(Expression* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(Expression* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void SemanticChecker::PreVisit(LetStatement* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(LetStatement* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;    
    size_t index = begin;

    assert((end - begin) >= 1);
    r += "let ";
    r += stream[begin].second;
    r += " = ";
    index++;
    r += stream[index].second;
    return r;

  });
}

void SemanticChecker::PreVisit(Identifier* node) 
{
  shift(stream);
}

void SemanticChecker::PostVisit(Identifier* node) 
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = node->getIdentifier();
    return r;

  });
}

void SemanticChecker::PreVisit(StringExpr* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(StringExpr* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = node->getValue();
    return r;
  });
}

void SemanticChecker::PreVisit(Literal* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(Literal* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;

  });
}

void SemanticChecker::PreVisit(NumberExpr* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(NumberExpr* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = node->getValue();
    return r;
  });
}

void SemanticChecker::PreVisit(BoolExpr* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(BoolExpr* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = node->getValue();
    return r;
  });
}

void SemanticChecker::PreVisit(TypeName* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(TypeName* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void SemanticChecker::PreVisit(Parameter* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(Parameter* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    r += node->getIdentifier();
    if (node->getType()) 
    { 
      r += " : "; 
      std::string type = node->getType()->to_string(); 
      if (type == "number") type = "int";
      r += type;
    }
    return r;
  });
}

void SemanticChecker::PreVisit(FunctionBody* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(FunctionBody* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void SemanticChecker::PreVisit(FunctionDecl* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(FunctionDecl* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    size_t index = begin;
    D_ParseNode *xpn = d_get_child(node->getIdentifierPtr(), 0);
    std::string id = std::string(xpn->start_loc.s, xpn->end);
    size_t size = end - begin;
    bool second = false;
    r += "function ";
    r += id;
    r += "(";      
    for (size_t i = 0; i < size; i++) {
      if (second) r += ", ";
      r += stream[begin + i].second;
      second = true;
    }
    r += ")";
    r += stream[end].second;
    return r;
  });
}

void SemanticChecker::PreVisit(FunctionCall* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(FunctionCall* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    std::string id = std::string(node->getIdentifier()->start_loc.s, node->getIdentifier()->end);
    size_t size = end - begin;
    bool second = false;
    r += id;
    r += "(";
    for (size_t index = begin; index <= end; index++) {
      if(second) r += " ";
      r += stream[index].second;
      second = true;
    }
    
    r += ")";
    return r;
  });
}

void SemanticChecker::PreVisit(CompoundStatement* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(CompoundStatement* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    r += "{\n";
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    r += "}\n";
    return r;
  });
}

void SemanticChecker::PreVisit(Argument* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(Argument* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void SemanticChecker::PreVisit(ParenExpression* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(ParenExpression* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    r += "(";
    r += stream[begin].second;
    r += ")";
    return r;
  });
}

void SemanticChecker::PreVisit(BinaryExpression* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(BinaryExpression* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    r += stream[begin].second;
    r += node->getOperator();
    r += stream[++begin].second;
    return r;
  });

}

void SemanticChecker::PreVisit(WhileLoop* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(WhileLoop* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    assert((end - begin) == 1);
    std::string r;    
    r += "while (";
    r += stream[begin].second;
    r += ")";
    r += stream[begin + 1].second;
    return r;
  });
}

void SemanticChecker::PreVisit(ForLoop* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(ForLoop* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    assert((end - begin) == 3);
    std::string r;
    r += "for (";
    r += stream[begin].second;
    r += ", ";
    r += stream[begin + 1].second;
    r += ", ";
    r += stream[begin + 2].second;
    r += ")";
    r += stream[begin + 3].second;
    return r;
  });
}

void SemanticChecker::PreVisit(IfStatement* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(IfStatement* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    size_t size = end - begin;
    std::string r;
    
    r += "if (";
    r += stream[begin].second;
    r += ")";
    r += stream[begin + 1].second;
    if (size == 2) r += stream[begin + 2].second;
    return r;
  });
}

void SemanticChecker::PreVisit(UnaryExpression* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(UnaryExpression* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void SemanticChecker::PreVisit(ReturnStatement* node)
{
  shift(stream);
}

void SemanticChecker::PostVisit(ReturnStatement* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    assert((end - begin) == 0);
    std::string r;
    
    r += "return ";
    r += stream[begin].second;
    return r;
  });
}

void SemanticChecker::PreVisit(NullStatement* node)
{
}

void SemanticChecker::PostVisit(NullStatement* node)
{
}

void SemanticChecker::PreVisit(MatchStatement*) {}
void SemanticChecker::PostVisit(MatchStatement*) {}
void SemanticChecker::PreVisit(MatchId*) {}
void SemanticChecker::PostVisit(MatchId*) {}
void SemanticChecker::PreVisit(CodeExpression*) {}
void SemanticChecker::PostVisit(CodeExpression*) {}
void SemanticChecker::PreVisit(MatchCase*) {}
void SemanticChecker::PostVisit(MatchCase*) {}
void SemanticChecker::PreVisit(Extension*) {}
void SemanticChecker::PostVisit(Extension*) {}
