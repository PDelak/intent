#ifndef VISITABLE_H
#define VISITABLE_H

class Visitor;

class Visitable
{
public:
  virtual void accept(Visitor* v) = 0;
  virtual ~Visitable() {}
};

#endif
