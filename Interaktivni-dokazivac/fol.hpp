#ifndef _FOL_H
#define _FOL_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

using namespace std;

typedef string FunctionSymbol;
typedef string PredicateSymbol;
typedef string Variable;


class BaseTerm;
typedef shared_ptr<BaseTerm> Term;


class BaseTerm : public enable_shared_from_this<BaseTerm> {

public:
  enum Type { TT_VARIABLE, TT_FUNCTION };
  virtual Type getType() const = 0;
  virtual void printTerm(ostream & ostr) const = 0;
  virtual bool equalTo(const Term & t) const = 0;

  virtual ~BaseTerm() {}
};

class VariableTerm : public BaseTerm {
private:
  Variable _v;
public:
  VariableTerm(const Variable & v)
    :_v(v)
  {}

  virtual Type getType() const
  {
    return TT_VARIABLE;
  }

  const Variable & getVariable() const
  {
    return _v;
  }
  virtual void printTerm(ostream & ostr) const
  {
    ostr << _v;
  }
   virtual bool equalTo(const Term & t) const{
      return t->getType() == TT_VARIABLE &&
             ((VariableTerm *) t.get())->getVariable() == _v;
  }
};

class FunctionTerm : public BaseTerm {
private:
  FunctionSymbol _f;
  vector<Term> _ops;

public:
  FunctionTerm(const FunctionSymbol & f,
           const vector<Term> & ops = vector<Term> ())
    :_f(f),
     _ops(ops)
  {}

  virtual Type getType() const
  {
    return TT_FUNCTION;
  }

  const FunctionSymbol & getSymbol() const
  {
    return _f;
  }

  const vector<Term> & getOperands() const
  {
    return _ops;
  }

  virtual void printTerm(ostream & ostr) const
  {
    ostr << _f;

    for(unsigned i = 0; i < _ops.size(); i++)
      {
    if(i == 0)
      ostr << "(";
    _ops[i]->printTerm(ostr);
    if(i != _ops.size() - 1)
      ostr << ",";
    else
      ostr << ")";
      }
  }
  bool equalTo(const Term & t) const
  {
    if(t->getType() != TT_FUNCTION)
      return false;

    if(_f != ((FunctionTerm *) t.get())->getSymbol())
      return false;

    const vector<Term> & t_ops = ((FunctionTerm *) t.get())->getOperands();

    if(_ops.size() != t_ops.size())
      return false;

    for(unsigned i = 0; i < _ops.size(); i++)
      if(!_ops[i]->equalTo(t_ops[i]))
        return false;

    return true;
  }
};

class BaseFormula;
typedef shared_ptr<BaseFormula> Formula;

class BaseFormula : public enable_shared_from_this<BaseFormula> {

public:

  enum Type { T_TRUE, T_FALSE, T_ATOM, T_NOT,
          T_AND, T_OR, T_IMP, T_IFF, T_FORALL, T_EXISTS };

  virtual void printFormula(ostream & ostr) const = 0;
  virtual Type getType() const = 0;
  virtual bool equalTo(const Formula & f) const = 0;
  virtual void getAtoms(std::vector<Formula> atoms) = 0;
  virtual int formulaDepth() const = 0;
  virtual ~BaseFormula() {}
};



class AtomicFormula : public BaseFormula {
public:
};


class LogicConstant : public AtomicFormula {

public:
    bool equalTo( const Formula & f) const
    {
      return f->getType() == this->getType();
    }
    void getAtoms(std::vector<Formula> atoms){
        atoms.push_back(shared_from_this());
    }
    int formulaDepth() const{
        return 1;
    }
};


class True : public LogicConstant {

public:
  virtual void printFormula(ostream & ostr) const
  {
    ostr << "true";
  }

  virtual Type getType() const
  {
    return T_TRUE;
  }

};


class False : public LogicConstant {

public:
  virtual void printFormula(ostream & ostr) const
  {
    ostr << "false";
  }

  virtual Type getType() const
  {
    return T_FALSE;
  }
};


class Atom : public AtomicFormula {
protected:
  PredicateSymbol _p;
  vector<Term> _ops;

public:
  Atom(const PredicateSymbol & p,
       const vector<Term> & ops = vector<Term>())
    :_p(p),
     _ops(ops)
  {}

  const PredicateSymbol & getSymbol() const
  {
    return _p;
  }

  const vector<Term> & getOperands() const
  {
    return _ops;
  }

  virtual void printFormula(ostream & ostr) const
  {
    ostr << _p;
    for(unsigned i = 0; i < _ops.size(); i++)
      {
    if(i == 0)
      ostr << "(";
    _ops[i]->printTerm(ostr);
    if(i != _ops.size() - 1)
      ostr << ",";
    else
      ostr << ")";
      }
  }

  virtual Type getType() const
  {
    return T_ATOM;
  }
  bool equalTo(const Formula & f) const
  {
    if(f->getType() != T_ATOM)
      return false;

    if(_p != ((Atom *) f.get())->getSymbol())
      return false;

    const vector<Term> & f_ops = ((Atom *) f.get())->getOperands();

    if(_ops.size() != f_ops.size())
      return false;

    for(unsigned i = 0; i < _ops.size(); i++)
      if(!_ops[i]->equalTo(f_ops[i]))
        return false;

      return true;
  }
  void getAtoms(std::vector<Formula> atoms){
      atoms.push_back(shared_from_this());
  }
  int formulaDepth() const {
    return 1;
  }
};

class Equality : public Atom {
public:
  Equality(const Term & lop, const Term & rop)
    :Atom("=", vector<Term> ())
  {
    _ops.push_back(lop);
    _ops.push_back(rop);
  }

  const Term & getLeftOperand() const
  {
    return _ops[0];
  }

  const Term & getRightOperand() const
  {
    return _ops[1];
  }

  virtual void printFormula(ostream & ostr) const
  {
    _ops[0]->printTerm(ostr);
    ostr << " = ";
    _ops[1]->printTerm(ostr);
  }
};

class Disequality : public Atom {
public:
  Disequality(const Term & lop, const Term & rop)
    :Atom("~=", vector<Term> ())
  {
    _ops.push_back(lop);
    _ops.push_back(rop);
  }

  const Term & getLeftOperand() const
  {
    return _ops[0];
  }

  const Term & getRightOperand() const
  {
    return _ops[1];
  }

  virtual void printFormula(ostream & ostr) const
  {

    _ops[0]->printTerm(ostr);
    ostr << " ~= ";
    _ops[1]->printTerm(ostr);
  }
};


class UnaryConjective : public BaseFormula {
protected:
   Formula _op;
public:
  UnaryConjective(const Formula & op)
    :_op(op)
  {}

  const Formula & getOperand() const
  {
    return _op;
  }
};

class Not : public UnaryConjective {
public:
  Not(const Formula & op)
    :UnaryConjective(op)
  {}

  virtual void printFormula(ostream & ostr) const
  {
    ostr << "~";
    Type op_type = _op->getType();

    if(op_type == T_AND || op_type == T_OR ||
       op_type == T_IMP || op_type == T_IFF)
      ostr << "(";

    _op->printFormula(ostr);

    if(op_type == T_AND || op_type == T_OR ||
       op_type == T_IMP || op_type == T_IFF)
      ostr << ")";
  }

  virtual Type getType() const
  {
    return T_NOT;
  }
  bool equalTo(const Formula & f) const
  {
    return f->getType() == this->getType() &&
      _op->equalTo(((UnaryConjective *)f.get())->getOperand());
  }
  void getAtoms(std::vector<Formula> atoms){
     _op->getAtoms(atoms);
  }
  int formulaDepth() const {
      return _op->formulaDepth() + 1;
  }
};


class BinaryConjective : public BaseFormula {
protected:
   Formula _op1, _op2;
public:
  BinaryConjective(const Formula & op1, const Formula & op2)
    :_op1(op1),
     _op2(op2)
  {}

  const Formula & getOperand1() const
  {
    return _op1;
  }

  const Formula & getOperand2() const
  {
    return _op2;
  }
  bool equalTo( const Formula & f) const
  {
    return f->getType() == this->getType() &&
      _op1->equalTo(((BinaryConjective *)f.get())->getOperand1())
      &&
      _op2->equalTo(((BinaryConjective *)f.get())->getOperand2());
  }
  void getAtoms(std::vector<Formula> atoms){
     _op1->getAtoms(atoms);
     _op2->getAtoms(atoms);
  }
  int formulaDepth() const{
      if(_op1->formulaDepth() > _op2->formulaDepth()){
          return _op1->formulaDepth() + 1;
      }
      else{
          return _op2->formulaDepth() + 1;
      }
  }
};


class And : public BinaryConjective {
public:
  And(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

  virtual void printFormula(ostream & ostr) const
  {
    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();

    if(op1_type == T_OR || op1_type == T_IMP ||
       op1_type == T_IFF)
      ostr << "(";

    _op1->printFormula(ostr);

    if(op1_type == T_OR || op1_type == T_IMP ||
       op1_type == T_IFF)
      ostr << ")";

    ostr << " & ";

    if(op2_type == T_OR || op2_type == T_IMP ||
       op2_type == T_IFF || op2_type == T_AND)
      ostr << "(";

    _op2->printFormula(ostr);

    if(op2_type == T_OR || op2_type == T_IMP ||
       op2_type == T_IFF || op2_type == T_AND)
      ostr << ")";
  }

  virtual Type getType() const
  {
    return T_AND;
  }
 };



class Or : public BinaryConjective {
public:
  Or(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

  virtual void printFormula(ostream & ostr) const
  {

    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();

    if(op1_type == T_IMP || op1_type == T_IFF)
      ostr << "(";

    _op1->printFormula(ostr);

    if(op1_type == T_IMP || op1_type == T_IFF)
      ostr << ")";

    ostr << " | ";

    if(op2_type == T_IMP ||
       op2_type == T_IFF || op2_type == T_OR)
      ostr << "(";

    _op2->printFormula(ostr);

    if(op2_type == T_IMP ||
       op2_type == T_IFF || op2_type == T_OR)
      ostr << ")";
  }

  virtual Type getType() const
  {
    return T_OR;
  }
};


class Imp : public BinaryConjective {
public:
  Imp(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

  virtual void printFormula(ostream & ostr) const
  {

    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();

    if(op1_type == T_IFF)
      ostr << "(";

    _op1->printFormula(ostr);

    if(op1_type == T_IFF)
      ostr << ")";

    ostr << " => ";

    if(op2_type == T_IMP || op2_type == T_IFF)
      ostr << "(";

    _op2->printFormula(ostr);

    if(op2_type == T_IMP || op2_type == T_IFF)
      ostr << ")";
  }

  virtual Type getType() const
  {
    return T_IMP;
  }
};



class Iff : public BinaryConjective {

public:
  Iff(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

  virtual void printFormula(ostream & ostr) const
  {

    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();

    _op1->printFormula(ostr);

    ostr << " => ";

    if(op2_type == T_IFF)
      ostr << "(";

    _op2->printFormula(ostr);

    if(op2_type == T_IFF)
      ostr << ")";

  }

  virtual Type getType() const
  {
    return T_IFF;
  }
};


class Quantifier : public BaseFormula {
protected:
  Variable _v;
  Formula  _op;
public:
  Quantifier(const Variable & v, const Formula & op)
    :_v(v),
     _op(op)
  {}

  const Variable & getVariable() const
  {
    return _v;
  }

  const Formula & getOperand() const
  {
    return _op;
  }
  bool equalTo(const Formula & f) const
  {
    return f->getType() == getType() &&
      ((Quantifier *) f.get())->getVariable() == _v &&
      ((Quantifier *) f.get())->getOperand()->equalTo(_op);
  }
  void getAtoms(std::vector<Formula> atoms){
     _op->getAtoms(atoms);
  }
  int formulaDepth() const{
      return 0;
  }
};

class Forall : public Quantifier {
public:
  Forall(const Variable & v, const Formula & op)
    :Quantifier(v, op)
  {}

  virtual Type getType() const
  {
    return T_FORALL;
  }
  virtual void printFormula(ostream & ostr) const
  {
    cout << "![" << _v << "] : ";

    Type op_type = _op->getType();

    if(op_type == T_AND || op_type == T_OR ||
       op_type == T_IMP || op_type == T_IFF)
      ostr << "(";

    _op->printFormula(ostr);

    if(op_type == T_AND || op_type == T_OR ||
       op_type == T_IMP || op_type == T_IFF)
      ostr << ")";
  }
};

class Exists : public Quantifier {
public:
  Exists(const Variable & v, const Formula & op)
    :Quantifier(v, op)
  {}

  virtual Type getType() const
  {
    return T_EXISTS;
  }

  virtual void printFormula(ostream & ostr) const
  {
    cout  << "?[" << _v << "] : ";

    Type op_type = _op->getType();

    if(op_type == T_AND || op_type == T_OR ||
       op_type == T_IMP || op_type == T_IFF)
      ostr << "(";

    _op->printFormula(ostr);

    if(op_type == T_AND || op_type == T_OR ||
       op_type == T_IMP || op_type == T_IFF)
      ostr << ")";
  }
};

inline
ostream & operator << (ostream & ostr, const Term & t)
{
  t->printTerm(ostr);
}

inline
ostream & operator << (ostream & ostr, const Formula & f)
{
  f->printFormula(ostr);
}

extern Formula parsed_formula;

#endif // _FOL_H
