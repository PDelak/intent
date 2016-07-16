// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef AST_H
#define AST_H

#include <list>
#include <utility>
#include <map>
#include <memory>
#include <sstream>
#include <array>
#include <functional>
#include "SymbolTable.h"
#include "ASTFwd.h"

typedef std::pair<std::string::const_iterator, 
                  std::string::const_iterator> StringRange;

struct Visitor;

enum NodeType {
  NodeAST = 0,
  ProgramAST,
  StatementAST,
  ExpressionStatementAST,
  ExpressionAST,
  LetStatementAST,
  IdentifierAST,
  StringExprAST,
  LiteralAST,
  NumberExprAST,
  BoolExprAST,
  TypeNameAST,
  ParameterAST,
  FunctionBodyAST,
  FunctionDeclAST,
  FunctionExprAST,
  FunctionCallAST,
  CompoundStatementAST,
  ArgumentAST,
  ParenExpressionAST,
  BinaryExpressionAST,
  WhileLoopAST,
  ForLoopAST,
  IfStatementAST,
  UnaryExpressionAST,
  ReturnStatementAST,
  TableExpressionAST,
  TableElementAST,
  MatchStatementAST,
  MatchIdAST,
  CodeExpressionAST,
  MatchCaseAST,
  ExtensionAST,
  DynamicNodeAST,
  Size
};

typedef struct NodeAdapter 
{ 
  NodePtr p; 
} NodeAdapter;

#ifndef D_ParseNode_User
#define D_ParseNode_User NodeAdapter
#endif

#include "dparse.h"

struct SemanticError : public std::runtime_error 
{ 
  SemanticError(const std::string& errorMessage); 
};

struct ASTNodeDecorator
{
  mutable std::string before;
  mutable std::string after;
  mutable std::function<bool(Node* p)> fun;

  ASTNodeDecorator& addDecoration(std::function<bool (Node* p)> f, 
                                  const std::string& b, 
                                  const std::string& a)
  {
    fun = f;
    before = b;
    after = a;
    return *this;
  }
};

struct Node 
{
  Node();
  virtual ~Node() {}
  static void indent();
  static void deindent();

  static void emitWithIndentation(std::string& out, 
                                  const std::string& body);

  void setParent(Node* p) 
  { parent = p; }
  
  Node* getParent() const 
  { return parent; }

  void setLHS(bool b) 
  { lhs = b; }
  
  void setRHS(bool b) 
  { rhs = b; }

  bool getLHS() const 
  { return lhs; }

  bool getRHS() const 
  { return rhs; }

  std::string serialize(const ASTNodeDecorator& decorator);

  // Returns unique identifier for AST node
  virtual NodeType getId();
  virtual void accept(Visitor* visitor) = 0;

  static basic_symbol_table symbol_table;
  static bool resolveTypesPhase;
  

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator) = 0;

private:
  static int level;
  Node* parent;
  bool lhs;
  bool rhs;
};

struct Statement : public Node
{
  Statement(D_ParseNode* statement = 0);
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  D_ParseNode* m_statement;
};

struct ExpressionStatement : public Statement
{
    ExpressionStatement(D_ParseNode* expression);

    virtual NodeType getId();
    virtual void accept(Visitor* visitor);

protected:
    virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
    D_ParseNode* m_expression;

};


struct Expression : public Statement
{
  Expression(D_ParseNode* expression);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_expression;

};


struct Program : public Statement
{
  Program(D_ParseNode* expressions = 0);
  void setOutput(const std::string& out);

  void setExpressions(D_ParseNode* expressions);
  void setIntermediateName(const std::string& i);
  std::string intermediateName() const;
  bool isTest();
  void test();
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

  void generate(std::ofstream& o);


protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  D_ParseNode* m_expressions;
  std::string m_output;
  std::string m_intermediateName;
};


struct LetStatement : public Statement
{
  LetStatement(D_ParseNode* identifier, 
               D_ParseNode* type, 
               D_ParseNode* declaration);
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  D_ParseNode* m_identifier;
  D_ParseNode* m_type;
  D_ParseNode* m_declaration;
};

struct Identifier : public Expression
{
  Identifier(const char* begin, 
             const char* end, 
             size_t line);
  
  virtual NodeType getId(); 
  virtual void accept(Visitor* visitor);
  
  std::string getIdentifier() 
  { return id; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  std::string id;
  size_t m_line;
};


struct StringExpr : public Expression
{
  StringExpr(const char* begin, 
             const char* end);

  virtual NodeType getId(); 
  
  virtual void accept(Visitor* visitor);

  std::string getValue() const 
  { return id; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  std::string id;
};


struct Literal : public Expression
{
  Literal(D_ParseNode* id);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
  
protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_id;

};


struct NumberExpr : public Expression
{
  NumberExpr(const char* begin, 
             const char* end);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

  std::string getValue() const 
  { return number; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  std::string number;

};


struct BoolExpr : public Expression
{
  BoolExpr(D_ParseNode* value);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

  std::string getValue() const 
  { return m_value; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  std::string m_value;

};


struct TypeName : public Node
{
  TypeName(D_ParseNode* value);
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

  std::string getValue() const 
  { return m_value; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  std::string m_value;

};


struct Parameter : public Node
{
  Parameter(D_ParseNode* identifier, 
            D_ParseNode* type);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
  TypePtr getType();
  
  std::string getIdentifier() const 
  { return m_identifier; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  std::string m_identifier;
  D_ParseNode* m_type;
  TypePtr ptype;

};


struct FunctionBody : public Statement
{
  FunctionBody(D_ParseNode* expression);
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
  
protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_expressions;

};


struct FunctionDecl : public Statement
{
  FunctionDecl(D_ParseNode* identifier, 
               D_ParseNode* firstParameter, 
               D_ParseNode* otherParameters, 
               D_ParseNode* returnType, 
               D_ParseNode* body);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

  D_ParseNode* getIdentifierPtr() const      
  { return m_identifier; }
  
  D_ParseNode* getFirstParameterPtr() const  
  { return m_firstParameter; }
  
  D_ParseNode* getOtherParametersPtr() const 
  { return m_otherParameters; }
  
  D_ParseNode* getReturnTypePtr() const      
  { return m_returnType; }
  
  D_ParseNode* getBodyPtr() const            
  { return m_body; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_identifier;
  D_ParseNode* m_firstParameter;
  D_ParseNode* m_otherParameters;
  D_ParseNode* m_returnType;
  D_ParseNode* m_body;
};

struct FunctionExpr : public Expression
{
  FunctionExpr(D_ParseNode* firstParameter,
               D_ParseNode* otherParameters,
               D_ParseNode* returnType,
               D_ParseNode* body);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

  D_ParseNode* getFirstParameterPtr() const
  {
    return m_firstParameter;
  }

  D_ParseNode* getOtherParametersPtr() const
  {
    return m_otherParameters;
  }

  D_ParseNode* getReturnTypePtr() const
  {
    return m_returnType;
  }

  D_ParseNode* getBodyPtr() const
  {
    return m_body;
  }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_firstParameter;
  D_ParseNode* m_otherParameters;
  D_ParseNode* m_returnType;
  D_ParseNode* m_body;
};


struct FunctionCall : public Expression
{
  FunctionCall(D_ParseNode* identifier, 
               D_ParseNode* args);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

  D_ParseNode* getIdentifier() const 
  { return m_identifier; }
  
  D_ParseNode* getArgs() const 
  { return m_args; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_identifier;
  D_ParseNode* m_args;
  TypePtr m_resultType;
};


struct CompoundStatement : public Statement
{
  CompoundStatement(D_ParseNode* expressions);
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
	
private:
  D_ParseNode* m_expressions;
};


struct Argument : public Expression
{
  Argument(D_ParseNode* args);
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator()) ;
private:
  D_ParseNode* m_args;

};


struct ParenExpression : public Expression
{
  ParenExpression(D_ParseNode* parenExpr);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_parenExpr;

};


struct BinaryExpression : public Expression
{
  BinaryExpression(D_ParseNode* left, 
                   D_ParseNode* right, 
                   const std::string & op);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

  D_ParseNode* left() const 
  { return m_left; }

  D_ParseNode* right() const 
  { return m_right; }

  std::string getOperator() const 
  { return m_op; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_left;
  D_ParseNode* m_right;
  std::string m_op;

};


struct WhileLoop : public Statement
{
  WhileLoop(D_ParseNode* logical_expr, 
            D_ParseNode* body);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_logical_expr;
  D_ParseNode *m_body;
};


struct ForLoop : public Statement
{
  ForLoop(D_ParseNode* init_expr, 
          D_ParseNode* cond_expr, 
          D_ParseNode* step_expr, 
          D_ParseNode* body);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  D_ParseNode* m_init_expr;
  D_ParseNode* m_cond_expr;
  D_ParseNode* m_step_expr;
  D_ParseNode* m_body;
};


struct IfStatement : public Statement
{
  IfStatement(D_ParseNode* logical_expr, 
              D_ParseNode* thenExpr, 
              D_ParseNode* elseExpr);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  D_ParseNode* m_logical_expr;
  D_ParseNode* m_thenExpr;
  D_ParseNode* m_elseExpr;
};

enum OperatorType { PREFIX = 0, POSTFIX = 1};

struct UnaryExpression : public Expression
{
  UnaryExpression(D_ParseNode* expression, 
                  const std::string& op, 
                  OperatorType opType);

  UnaryExpression(D_ParseNode* expression,
                  D_ParseNode* index,
    const std::string& op,
    OperatorType opType);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  D_ParseNode* m_expression;
  D_ParseNode* m_index;
  std::string m_op;
  OperatorType m_opType;

};


struct ReturnStatement : public Statement
{
  ReturnStatement(D_ParseNode* expression);
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_expression;

};

struct TableExpression : public Expression
{
  TableExpression(D_ParseNode* expression);
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_expression;
};

struct TableElement : public Expression
{
  TableElement(D_ParseNode* expression);
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_expression;
};

struct NullStatement : public Statement
{
  NullStatement();

  virtual NodeType getId() 
  { return NodeAST; }
  
  virtual void accept(Visitor* visitor) 
  {}

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator()) 
  { return ""; }
};

struct MatchStatement : public Statement
{
  MatchStatement(D_ParseNode* id, 
                 D_ParseNode* mcase);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
  D_ParseNode* matchId;
  D_ParseNode* matchCase;
};

struct MatchId : public Expression
{
  MatchId(const char* begin, 
          const char* end);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
  std::string getIdentifier() 
  { return id; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator()) { return id; }

private:
  std::string id;
};

struct CodeExpression : public Expression
{
  CodeExpression(const char* begin, 
                 const char* end);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
  
  std::string code() const 
  { return m_code; }

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  std::string m_code;
};

struct MatchCase : public Expression
{
  MatchCase(D_ParseNode* id, 
            D_ParseNode* code);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
  
  std::string matchId() const;
  std::string codeExpr() const;

protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());

private:
  D_ParseNode* m_id;
  D_ParseNode* m_codeExpr;
};

struct Extension : public Statement
{
  Extension(D_ParseNode* main);
  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
private:
  D_ParseNode* m_main;
};

struct DynamicNode : public Statement
{
  DynamicNode(const std::string& code, 
              void** children, 
              int offset);

  virtual NodeType getId();
  virtual void accept(Visitor* visitor);
  
  std::string code() const 
  { return m_code; }
  
  std::list<StringRange> codeRanges() const 
  { return mcode_ranges; }
  
  std::string generateCode();

private:
  void setValueNodes(const std::list<StringRange>& code_ranges, int offset);
  
protected:
  virtual std::string serializeImpl(const ASTNodeDecorator& decorator = ASTNodeDecorator());
public:
  std::string m_code;
  void **m_children;
  int m_offset;
  std::list<StringRange> mcode_ranges;
  std::unordered_map<size_t, D_ParseNode*> m_values;
  std::unordered_map<size_t, D_ParseNode*> m_childs;
};


#endif
