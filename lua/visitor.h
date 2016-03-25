#ifndef VISITOR_H
#define VISITOR_H

struct Node;
struct TranslationUnit;
struct Expression;
struct TypeDeclExpression;
struct DeclarationExpression;
struct Decl;
struct TemplateParams;
struct TypeInitializer;

struct Literal;
struct Number;
struct Identifier;
struct StringExp;
struct BoolExp;
struct Declaration;
struct Reference;
struct ReferenceExpression;
struct CompoundExpression;
struct ReturnExpression;
struct LogicalExpression;
struct RangeExpression;
struct ForeachExpression;
struct QQuoteExpression;
struct SelectExpression;
struct CaseExpression;
struct OtherExpression;
struct MatchExpression;
struct IfExpression;
struct CallExpression;
struct Parameter;
struct ListExpression;
struct FunctionExpression;
struct MacroExpression;
struct Operator;
struct TupleExpression;
struct BuiltinType;
struct Type;
struct CompoundType;
struct FunctionType;
struct TupleType;

class Visitor
{
public:
  virtual ~Visitor() {}
  virtual void PreVisit(Node*) = 0;
  virtual void PostVisit(Node*) = 0;
  virtual void PreVisit(Expression*) = 0;
  virtual void PostVisit(Expression*) = 0;
  virtual void PreVisit(TypeDeclExpression*) = 0;
  virtual void PostVisit(TypeDeclExpression*) = 0;
  virtual void PreVisit(DeclarationExpression*) = 0;
  virtual void PostVisit(DeclarationExpression*) = 0;
  virtual void PreVisit(Decl*) = 0;
  virtual void PostVisit(Decl*) = 0;
  virtual void PreVisit(TemplateParams*) = 0;
  virtual void PostVisit(TemplateParams*) = 0;
  virtual void PreVisit(TypeInitializer*) = 0;
  virtual void PostVisit(TypeInitializer*) = 0;

  virtual void PreVisit(Literal*) = 0;
  virtual void PostVisit(Literal*) = 0;
  virtual void PreVisit(TranslationUnit*) = 0;
  virtual void PostVisit(TranslationUnit*) = 0;
  virtual void PreVisit(Number*) = 0;
  virtual void PostVisit(Number*) = 0;
  virtual void PreVisit(Identifier*) = 0;
  virtual void PostVisit(Identifier*) = 0;
  virtual void PreVisit(StringExp*) = 0;
  virtual void PostVisit(StringExp*) = 0;
  virtual void PreVisit(BoolExp*) = 0;
  virtual void PostVisit(BoolExp*) = 0;
  virtual void PreVisit(Declaration*) = 0;
  virtual void PostVisit(Declaration*) = 0;
  virtual void PreVisit(Reference*) = 0;
  virtual void PostVisit(Reference*) = 0;
  virtual void PreVisit(ReferenceExpression*) = 0;
  virtual void PostVisit(ReferenceExpression*) = 0;
  virtual void PreVisit(CompoundExpression*) = 0;
  virtual void PostVisit(CompoundExpression*) = 0;
  virtual void PreVisit(ReturnExpression*) = 0;
  virtual void PostVisit(ReturnExpression*) = 0;
  virtual void PreVisit(LogicalExpression*) = 0;
  virtual void PostVisit(LogicalExpression*) = 0;
  virtual void PreVisit(RangeExpression*) = 0;
  virtual void PostVisit(RangeExpression*) = 0;
  virtual void PreVisit(ForeachExpression*) = 0;
  virtual void PostVisit(ForeachExpression*) = 0;
  virtual void PreVisit(QQuoteExpression*) = 0;
  virtual void PostVisit(QQuoteExpression*) = 0;
  virtual void PreVisit(SelectExpression*) = 0;
  virtual void PostVisit(SelectExpression*) = 0;
  virtual void PreVisit(CaseExpression*) = 0;
  virtual void PostVisit(CaseExpression*) = 0;
  virtual void PreVisit(OtherExpression*) = 0;
  virtual void PostVisit(OtherExpression*) = 0;
  virtual void PreVisit(MatchExpression*) = 0;
  virtual void PostVisit(MatchExpression*) = 0;
  virtual void PreVisit(IfExpression*) = 0;
  virtual void PostVisit(IfExpression*) = 0;
  virtual void PreVisit(CallExpression*) = 0;
  virtual void PostVisit(CallExpression*) = 0;
  virtual void PreVisit(Parameter*) = 0;
  virtual void PostVisit(Parameter*) = 0;
  virtual void PreVisit(ListExpression*) = 0;
  virtual void PostVisit(ListExpression*) = 0;
  virtual void PreVisit(FunctionExpression*) = 0;
  virtual void PostVisit(FunctionExpression*) = 0;
  virtual void PreVisit(MacroExpression*) = 0;
  virtual void PostVisit(MacroExpression*) = 0;
  virtual void PreVisit(Operator*) = 0;
  virtual void PostVisit(Operator*) = 0;
  virtual void PreVisit(TupleExpression*) = 0;
  virtual void PostVisit(TupleExpression*) = 0;
  virtual void PreVisit(Type*) = 0;
  virtual void PostVisit(Type*) = 0;
  virtual void PreVisit(BuiltinType*) = 0;
  virtual void PostVisit(BuiltinType*) = 0;
  virtual void PreVisit(CompoundType*) = 0;
  virtual void PostVisit(CompoundType*) = 0;
  virtual void PreVisit(FunctionType*) = 0;
  virtual void PostVisit(FunctionType*) = 0;
  virtual void PreVisit(TupleType*) = 0;
  virtual void PostVisit(TupleType*) = 0;
};

class DefaultVisitor : public Visitor
{
public:
  virtual void PreVisit(Node*) {}
  virtual void PostVisit(Node*) {}
  virtual void PreVisit(Expression*) {}
  virtual void PostVisit(Expression*) {}
  virtual void PreVisit(TypeDeclExpression*) {}
  virtual void PostVisit(TypeDeclExpression*) {}
  virtual void PreVisit(DeclarationExpression*) {}
  virtual void PostVisit(DeclarationExpression*) {}
  virtual void PreVisit(Decl*) {}
  virtual void PostVisit(Decl*) {}
  virtual void PreVisit(TemplateParams*) {}
  virtual void PostVisit(TemplateParams*) {}
  virtual void PreVisit(TypeInitializer*) {}
  virtual void PostVisit(TypeInitializer*) {}

  virtual void PreVisit(Literal*) {}
  virtual void PostVisit(Literal*) {}
  virtual void PreVisit(TranslationUnit*) {}
  virtual void PostVisit(TranslationUnit*) {}
  virtual void PreVisit(Number*) {}
  virtual void PostVisit(Number*) {}
  virtual void PreVisit(Identifier*) {}
  virtual void PostVisit(Identifier*) {}
  virtual void PreVisit(StringExp*) {}
  virtual void PostVisit(StringExp*) {}
  virtual void PreVisit(BoolExp*) {}
  virtual void PostVisit(BoolExp*) {}
  virtual void PreVisit(Declaration*) {}
  virtual void PostVisit(Declaration*) {}
  virtual void PreVisit(Reference*) {}
  virtual void PostVisit(Reference*) {}
  virtual void PreVisit(ReferenceExpression*) {}
  virtual void PostVisit(ReferenceExpression*) {}
  virtual void PreVisit(CompoundExpression*) {}
  virtual void PostVisit(CompoundExpression*) {}
  virtual void PreVisit(ReturnExpression*) {}
  virtual void PostVisit(ReturnExpression*) {}
  virtual void PreVisit(LogicalExpression*) {}
  virtual void PostVisit(LogicalExpression*) {}
  virtual void PreVisit(RangeExpression*) {}
  virtual void PostVisit(RangeExpression*) {}
  virtual void PreVisit(ForeachExpression*) {}
  virtual void PostVisit(ForeachExpression*) {}
  virtual void PreVisit(QQuoteExpression*) {}
  virtual void PostVisit(QQuoteExpression*) {}
  virtual void PreVisit(SelectExpression*) {}
  virtual void PostVisit(SelectExpression*) {}
  virtual void PreVisit(CaseExpression*) {}
  virtual void PostVisit(CaseExpression*) {}
  virtual void PreVisit(OtherExpression*) {}
  virtual void PostVisit(OtherExpression*) {}
  virtual void PreVisit(MatchExpression*) {}
  virtual void PostVisit(MatchExpression*) {}
  virtual void PreVisit(IfExpression*) {}
  virtual void PostVisit(IfExpression*) {}
  virtual void PreVisit(CallExpression*) {}
  virtual void PostVisit(CallExpression*) {}
  virtual void PreVisit(Parameter*) {}
  virtual void PostVisit(Parameter*) {}
  virtual void PreVisit(ListExpression*) {}
  virtual void PostVisit(ListExpression*) {}
  virtual void PreVisit(FunctionExpression*) {}
  virtual void PostVisit(FunctionExpression*) {}
  virtual void PreVisit(MacroExpression*) {}
  virtual void PostVisit(MacroExpression*) {}
  virtual void PreVisit(Operator*) {}
  virtual void PostVisit(Operator*) {}
  virtual void PreVisit(TupleExpression*) {}
  virtual void PostVisit(TupleExpression*) {}
  virtual void PreVisit(Type*) {}
  virtual void PostVisit(Type*) {}
  virtual void PreVisit(BuiltinType*) {}
  virtual void PostVisit(BuiltinType*) {}
  virtual void PreVisit(CompoundType*) {}
  virtual void PostVisit(CompoundType*) {}
  virtual void PreVisit(FunctionType*) {}
  virtual void PostVisit(FunctionType*) {}
  virtual void PreVisit(TupleType*) {}
  virtual void PostVisit(TupleType*) {}

};

#endif
