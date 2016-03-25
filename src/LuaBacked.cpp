// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include "AST.h"
#include "SymbolTable.h"
#include "ASTRtti.h"

std::string Program::serializeImpl(const ASTNodeDecorator& decorator) {
  std::string out;
  int nch = d_get_number_of_children(m_expressions);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      D_ParseNode *xpn = d_get_child(m_expressions, i);
      out += xpn->user.p->serialize(decorator);
    }
  }
  return  out;
}

std::string Statement::serializeImpl(const ASTNodeDecorator& decorator) {
  D_ParseNode *xpn = d_get_child(m_statement, 0);
  return xpn->user.p->serialize(decorator);
}

std::string Expression::serializeImpl(const ASTNodeDecorator& decorator) {
  D_ParseNode *xpn = d_get_child(m_expression, 0);
  return xpn->user.p->serialize(decorator);
}


std::string LetStatement::serializeImpl(const ASTNodeDecorator& decorator) {
    return std::string().append(m_identifier->user.p->serialize(decorator))
    .append("=").append(m_declaration->user.p->serialize(decorator))
    .append("\n");;
}

std::string Identifier::serializeImpl(const ASTNodeDecorator& decorator) { return std::string(id.begin()+1, id.end()); }
std::string StringExpr::serializeImpl(const ASTNodeDecorator& decorator) { return id; }

std::string Literal::serializeImpl(const ASTNodeDecorator& decorator) {
  D_ParseNode *xpn = d_get_child(m_id, 0);
  return xpn->user.p->serialize(decorator);
}

std::string NumberExpr::serializeImpl(const ASTNodeDecorator& decorator) { return number; }
std::string BoolExpr::serializeImpl(const ASTNodeDecorator& decorator) { return m_value; }
std::string TypeName::serializeImpl(const ASTNodeDecorator& decorator) { return m_value; }

std::string Parameter::serializeImpl(const ASTNodeDecorator& decorator) { return m_identifier; }


std::string FunctionBody::serializeImpl(const ASTNodeDecorator& decorator) {
  indent();
  std::string out;
  emitWithIndentation(out, m_expressions->user.p->serialize(decorator));
  deindent();
  out += "end\n";
  return out;
}

std::string FunctionDecl::serializeImpl(const ASTNodeDecorator& decorator) {
  D_ParseNode *xpn = d_get_child(m_identifier, 0);
  std::string id = std::string(xpn->start_loc.s + 1, xpn->end);
  
  std::string out = "function ";
  out += id;
  out += "(";
  xpn = d_get_child(m_firstParameter, 0);
  if (xpn) {
    std::string parameterName = xpn->user.p->serialize(decorator);
    parameterName = std::string(parameterName.begin() + 1, parameterName.end());
    out += parameterName;
  }
  int nch = d_get_number_of_children(m_otherParameters);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      out += ", ";
      D_ParseNode *xpn = d_get_child(m_otherParameters, i);
      D_ParseNode *xpn2 = d_get_child(xpn, 1);
	    std::string parameterName = xpn2->user.p->serialize(decorator);
      parameterName = std::string(parameterName.begin() + 1, parameterName.end());
      out += parameterName;
        
    }
  }
  out += ")\n";
  
  out += m_body->user.p->serialize(decorator);
  
  return out ; 
}

std::string FunctionCall::serializeImpl(const ASTNodeDecorator& decorator) {
  D_ParseNode *xpn = d_get_child(m_identifier, 0);
  std::string id = std::string(xpn->start_loc.s + 1, xpn->end);
  std::string out;
  out += id;

  out += "(";
  int nch = d_get_number_of_children(m_args);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      if (i >= 1 && i < nch) out += ",";
      D_ParseNode *xpn = d_get_child(m_args, i);
      out += xpn->user.p->serialize(decorator);            
    }
  }
  out += ")\n";
  return out;
}


std::string CompoundStatement::serializeImpl(const ASTNodeDecorator& decorator) {
  std::string out = "do\n";    
  int nch = d_get_number_of_children(m_expressions);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      D_ParseNode *xpn = d_get_child(m_expressions, i);
      indent();
      emitWithIndentation(out, xpn->user.p->serialize(decorator));
      deindent();
    }
  }
  emitWithIndentation(out, "end\n");
  return  out;
}

std::string Argument::serializeImpl(const ASTNodeDecorator& decorator) { return m_args->user.p->serialize(decorator); }

std::string ParenExpression::serializeImpl(const ASTNodeDecorator& decorator) {
  std::string out;
  out += "("; out += m_parenExpr->user.p->serialize(decorator); out += ")";
  out += " ";
  return out;
}

std::string BinaryExpression::serializeImpl(const ASTNodeDecorator& decorator) {
  std::string out;
  if (m_left) out = m_left->user.p->serialize(decorator); 
  if (!m_op.empty()) out += m_op;
  if (m_right)out += m_right->user.p->serialize(decorator);
  
  out += " ";
  return  out;
}


std::string WhileLoop::serializeImpl(const ASTNodeDecorator& decorator) {
  std::string out = "while ";
  out += m_logical_expr->user.p->serialize(decorator);
  out += " ";    
  out += m_body->user.p->serialize(decorator);
    
  return  out;
}


std::string ForLoop::serializeImpl(const ASTNodeDecorator& decorator) {
  return 
    std::string("for ").append(m_init_expr->user.p->serialize(decorator))
    .append(", ").append(m_cond_expr->user.p->serialize(decorator))
    .append(", ").append(m_step_expr->user.p->serialize(decorator))
    .append(" ").append(m_body->user.p->serialize(decorator));
}

std::string IfStatement::serializeImpl(const ASTNodeDecorator& decorator) {
  std::string out = "if ";
  out += m_logical_expr->user.p->serialize(decorator);
  out += " then ";
  emitWithIndentation(out, m_thenExpr->user.p->serialize(decorator));
  if (m_elseExpr) { 
    out += "else ";   
    indent();
	out += m_elseExpr->user.p->serialize(decorator);
    deindent();
  }
  emitWithIndentation(out, "end\n");
  return  out;
}

std::string UnaryExpression::serializeImpl(const ASTNodeDecorator& decorator) {
  std::string out;
  if (m_op != "index") {
    if (m_opType == PREFIX) { out += m_op; out += " "; }
    out += m_expression->user.p->serialize(decorator);
    if (m_opType == POSTFIX) out += m_op;
  }
  else {
    out += m_expression->user.p->serialize(decorator);    
    out += m_index->user.p->serialize(decorator);   
  }
  return  out;
}


std::string ReturnStatement::serializeImpl(const ASTNodeDecorator& decorator) {
  std::string out = "return ";
  out += m_expression->user.p->serialize(decorator);
  out += "\n";
  return out;
}

bool isLHSExpression(Node* n)
{
  Node* parent = n->getParent();
  while (parent) {
    if (parent->getLHS()) return true;
    parent = parent->getParent();
  }
  return false;
}

std::string TableExpression::serializeImpl(const ASTNodeDecorator& decorator)
{
  std::string out = "{";
  int nch = d_get_number_of_children(m_expression);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      if (i > 0) out += ", ";
      D_ParseNode *xpn = d_get_child(m_expression, i);
      ASTNodeDecorator tableDecorator;
      tableDecorator.addDecoration([](Node* p) 
      { 
      std::string ident;
      bool left = false;
      if (is<Identifier>(p))
      {
        Identifier* id = static_cast<Identifier*>(p);
        ident = id->getIdentifier();					
      }
      if (is<NumberExpr>(p) || is<Identifier>(p) || is<StringExpr>(p)) {
        left = isLHSExpression(p);
      }
      return left && (is<NumberExpr>(p) || is<StringExpr>(p) || is<Identifier>(p) && 
      (ident == "true" || ident == "false"));
      }, "[", "]");
      out += xpn->user.p->serialize(tableDecorator);
    }
  }
  out += "}";
  return out;
}

std::string TableElement::serializeImpl(const ASTNodeDecorator& decorator)
{
  std::string out = "[";
  out += m_expression->user.p->serialize(decorator);
  BinaryExpression* bn = static_cast<BinaryExpression*>(m_expression->user.p.get());
  out += "]";
  return out;
}

std::string CodeExpression::serializeImpl(const ASTNodeDecorator& decorator) {
  return m_code;
}

std::string MatchStatement::serializeImpl(const ASTNodeDecorator& decorator)
{
  return "";
}

std::string MatchCase::serializeImpl(const ASTNodeDecorator& decorator)
{
  return "";
}

std::string Extension::serializeImpl(const ASTNodeDecorator& decorator)
{
  return "";
}

std::string DynamicNode::serializeImpl(const ASTNodeDecorator& decorator)
{
  return "";
}
