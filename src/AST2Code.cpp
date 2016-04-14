// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "AST.h"
#include "AST2Code.h"
#include <cassert>
#include "VisitorTools.h"

AST2Code::AST2Code() {}

void AST2Code::PreVisit(Program* node) 
{
  shift(stream);
}

void AST2Code::PostVisit(Program* node) 
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void AST2Code::PreVisit(Statement* node) 
{
  shift(stream);
}

void AST2Code::PostVisit(Statement* node)
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

void AST2Code::PreVisit(Expression* node)
{
  shift(stream);
}

void AST2Code::PostVisit(Expression* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void AST2Code::PreVisit(LetStatement* node)
{
  shift(stream);
}

void AST2Code::PostVisit(LetStatement* node)
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

void AST2Code::PreVisit(Identifier* node) 
{
  shift(stream);
}

void AST2Code::PostVisit(Identifier* node) 
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = node->getIdentifier();
    return r;

  });
}

void AST2Code::PreVisit(StringExpr* node)
{
  shift(stream);
}

void AST2Code::PostVisit(StringExpr* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = node->getValue();
    return r;
  });
}

void AST2Code::PreVisit(Literal* node)
{
  shift(stream);
}

void AST2Code::PostVisit(Literal* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;

  });
}

void AST2Code::PreVisit(NumberExpr* node)
{
  shift(stream);
}

void AST2Code::PostVisit(NumberExpr* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = node->getValue();
    return r;
  });
}

void AST2Code::PreVisit(BoolExpr* node)
{
  shift(stream);
}

void AST2Code::PostVisit(BoolExpr* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = node->getValue();
    return r;
  });
}

void AST2Code::PreVisit(TypeName* node)
{
  shift(stream);
}

void AST2Code::PostVisit(TypeName* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void AST2Code::PreVisit(Parameter* node)
{
  shift(stream);
}

void AST2Code::PostVisit(Parameter* node)
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

void AST2Code::PreVisit(FunctionBody* node)
{
  shift(stream);
}

void AST2Code::PostVisit(FunctionBody* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void AST2Code::PreVisit(FunctionDecl* node)
{
  shift(stream);
}

void AST2Code::PostVisit(FunctionDecl* node)
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

void AST2Code::PreVisit(FunctionExpr* node)
{
  shift(stream);
}

void AST2Code::PostVisit(FunctionExpr* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    size_t index = begin;
    size_t size = end - begin;
    bool second = false;
    r += "function ";
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

void AST2Code::PreVisit(FunctionCall* node)
{
  shift(stream);
}

void AST2Code::PostVisit(FunctionCall* node)
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

void AST2Code::PreVisit(CompoundStatement* node)
{
  shift(stream);
}

void AST2Code::PostVisit(CompoundStatement* node)
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

void AST2Code::PreVisit(Argument* node)
{
  shift(stream);
}

void AST2Code::PostVisit(Argument* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void AST2Code::PreVisit(ParenExpression* node)
{
  shift(stream);
}

void AST2Code::PostVisit(ParenExpression* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    r += "(";
    r += stream[begin].second;
    r += ")";
    return r;
  });
}

void AST2Code::PreVisit(BinaryExpression* node)
{
  shift(stream);
}

void AST2Code::PostVisit(BinaryExpression* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    r += stream[begin].second;
    r += node->getOperator();
    r += stream[++begin].second;
    return r;
  });

}

void AST2Code::PreVisit(WhileLoop* node)
{
  shift(stream);
}

void AST2Code::PostVisit(WhileLoop* node)
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

void AST2Code::PreVisit(ForLoop* node)
{
  shift(stream);
}

void AST2Code::PostVisit(ForLoop* node)
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

void AST2Code::PreVisit(IfStatement* node)
{
  shift(stream);
}

void AST2Code::PostVisit(IfStatement* node)
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

void AST2Code::PreVisit(UnaryExpression* node)
{
  shift(stream);
}

void AST2Code::PostVisit(UnaryExpression* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void AST2Code::PreVisit(ReturnStatement* node)
{
  shift(stream);
}

void AST2Code::PostVisit(ReturnStatement* node)
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    assert((end - begin) == 0);
    std::string r;
    
    r += "return ";
    r += stream[begin].second;
    return r;
  });
}

void AST2Code::PreVisit(NullStatement* node)
{
}

void AST2Code::PostVisit(NullStatement* node)
{
}

void AST2Code::PreVisit(MatchStatement*) 
{
  shift(stream);
}

void AST2Code::PostVisit(MatchStatement* node) 
{
  // Escape MatchStatement
  // Everything inside MatchStatement should be escaped and not visible
  // in generated output
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r = "";
    return r;
  });
}
void AST2Code::PreVisit(MatchId*) {}
void AST2Code::PostVisit(MatchId*) {}
void AST2Code::PreVisit(CodeExpression*) {}
void AST2Code::PostVisit(CodeExpression*) {}
void AST2Code::PreVisit(MatchCase*) {}
void AST2Code::PostVisit(MatchCase*) {}

void AST2Code::PreVisit(Extension*) 
{
  shift(stream);
}

void AST2Code::PostVisit(Extension* node) 
{
  reduce(stream, node, [](const Stream& stream, size_t begin, size_t end) {
    std::string r;
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    return r;
  });
}

void AST2Code::PreVisit(DynamicNode* node) 
{
  shift(stream);
}

void AST2Code::PostVisit(DynamicNode* node) 
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = node->generateCode();
    return r;
  });
}

void AST2Code::PreVisit(TableExpression* node)
{
  shift(stream);
}

void AST2Code::PostVisit(TableExpression* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = "[";
    for (size_t index = begin; index <= end; index++) {
      if (index >= (begin + 1)) {
        r += " ";
      }
      r += stream[index].second;
    }
    r += "]";
    return r;
  });
}

void AST2Code::PreVisit(TableElement*)
{
  shift(stream);
}

void AST2Code::PostVisit(TableElement* node)
{
  reduce(stream, node, [=](const Stream& stream, size_t begin, size_t end) {
    std::string r = "[";
    for (size_t index = begin; index <= end; index++) {
      r += stream[index].second;
    }
    r += "]";
    return r;
  });
}
