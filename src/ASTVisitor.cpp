// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include "AST.h"
#include "SymbolTable.h"
#include "ASTRtti.h"
#include "Visitor.h"

void Node::accept(Visitor* visitor) {}

void Program::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  int nch = d_get_number_of_children(m_expressions);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      D_ParseNode *xpn = d_get_child(m_expressions, i);
      xpn->user.p->accept(visitor);
    }
  }
  visitor->PostVisit(this);
}

void Statement::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  D_ParseNode *xpn = d_get_child(m_statement, 0);
  if (xpn && xpn->user.p) xpn->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void Expression::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  D_ParseNode *xpn = d_get_child(m_expression, 0);
  xpn->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void LetStatement::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  m_identifier->user.p->accept(visitor);
  if(m_type && m_type->user.p) m_type->user.p->accept(visitor);
  m_declaration->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void Identifier::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  visitor->PostVisit(this);
}

void StringExpr::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  visitor->PostVisit(this);
}

void Literal::accept(Visitor* visitor)
{
  D_ParseNode *xpn = d_get_child(m_id, 0);
  xpn->user.p->accept(visitor);

}

void NumberExpr::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  visitor->PostVisit(this);
}

void BoolExpr::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  visitor->PostVisit(this);
}

void TypeName::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  visitor->PostVisit(this);
}

void Parameter::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  visitor->PostVisit(this);
}

void FunctionBody::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  m_expressions->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void FunctionDecl::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  D_ParseNode *xpn = d_get_child(m_identifier, 0);

  xpn = d_get_child(m_firstParameter, 0);
  if (xpn) {
    xpn->user.p->accept(visitor);
  }
  int nch = d_get_number_of_children(m_otherParameters);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      D_ParseNode *xpn = d_get_child(m_otherParameters, i);
      D_ParseNode *xpn2 = d_get_child(xpn, 1);
      xpn2->user.p->accept(visitor);
    }
  }
  m_body->user.p->accept(visitor);

  visitor->PostVisit(this);
}

void FunctionCall::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  D_ParseNode *xpn = d_get_child(m_identifier, 0);
  int nch = d_get_number_of_children(m_args);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      D_ParseNode *xpn = d_get_child(m_args, i);
      xpn->user.p->accept(visitor);

    }
  }
  visitor->PostVisit(this);
}


void CompoundStatement::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  int nch = d_get_number_of_children(m_expressions);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      D_ParseNode *xpn = d_get_child(m_expressions, i);
      xpn->user.p->accept(visitor);
    }
  }
  visitor->PostVisit(this);
}

void Argument::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  m_args->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void ParenExpression::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  m_parenExpr->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void BinaryExpression::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  if (m_left)        m_left->user.p->accept(visitor);
  if (m_right)       m_right->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void WhileLoop::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  m_logical_expr->user.p->accept(visitor);
  m_body->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void ForLoop::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  m_init_expr->user.p->accept(visitor);
  m_cond_expr->user.p->accept(visitor);
  m_step_expr->user.p->accept(visitor);
  m_body->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void IfStatement::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  m_logical_expr->user.p->accept(visitor);
  m_thenExpr->user.p->accept(visitor);
  if (m_elseExpr) {
    m_elseExpr->user.p->accept(visitor);
  }
  visitor->PostVisit(this);
}

void UnaryExpression::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  m_expression->user.p->accept(visitor);
  if(m_index) m_index->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void ReturnStatement::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  m_expression->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void TableExpression::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  int nch = d_get_number_of_children(m_expression);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      D_ParseNode *xpn = d_get_child(m_expression, i);
      xpn->user.p->accept(visitor);
    }
  }
  visitor->PostVisit(this);
}

void TableElement::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  m_expression->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void MatchStatement::accept(Visitor* visitor)
{
  visitor->PreVisit(this);

  if (matchId) matchId->user.p->accept(visitor);
  int nch = d_get_number_of_children(matchCase);
  if (nch != 0) {
    for (int i = 0; i < nch; i++) {
      D_ParseNode *xpn = d_get_child(matchCase, i);
      xpn->user.p->accept(visitor);
    }

  }
  visitor->PostVisit(this);
}

void MatchId::accept(Visitor* visitor) 
{
  visitor->PreVisit(this);
  visitor->PostVisit(this);
}

void CodeExpression::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  visitor->PostVisit(this);
}


void MatchCase::accept(Visitor* visitor)
{
  visitor->PreVisit(this);

  D_ParseNode *xpn = d_get_child(m_id, 0);
  if (xpn) xpn->user.p->accept(visitor);  
  if (m_codeExpr) m_codeExpr->user.p->accept(visitor);
  
  
  visitor->PostVisit(this);
}

void Extension::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  if (m_main && m_main->user.p) m_main->user.p->accept(visitor);
  visitor->PostVisit(this);
}

void DynamicNode::accept(Visitor* visitor)
{
  visitor->PreVisit(this);
  visitor->PostVisit(this);
}

