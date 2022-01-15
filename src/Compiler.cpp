#include <exception>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <thread>
#include <typeinfo>
#include <vector>


#include "Bool.hpp"
#include "Keyword.hpp"
#include "LazySeq.hpp"
#include "Numbers.hpp"
#include "RT.hpp"
#include "Set.hpp"
#include "String.hpp"
#include "Symbol.hpp"
#include "Util.hpp"
#include "Var.hpp"
#include "Vector.hpp"

static const std::shared_ptr<const Symbol> DEF = Symbol::intern("def");
static const std::shared_ptr<const Symbol> LOOP = Symbol::intern("loop*");
static const std::shared_ptr<const Symbol> RECUR = Symbol::intern("recur");
static const std::shared_ptr<const Symbol> IF = Symbol::intern("if");
static const std::shared_ptr<const Symbol> LET = Symbol::intern("let*");
static const std::shared_ptr<const Symbol> LETFN = Symbol::intern("letfn*");
static const std::shared_ptr<const Symbol> DO = Symbol::intern("do");
static const std::shared_ptr<const Symbol> FN = Symbol::intern("fn*");
static const std::shared_ptr<const Symbol> FNONCE = std::dynamic_pointer_cast<const Symbol>(
        Symbol::intern("fn*")->with_meta(RT::map({Keyword::intern("", "once"), T})));
static const std::shared_ptr<const Symbol> QUOTE = Symbol::intern("quote");


static std::shared_ptr<Var> LOCAL_ENV = Var::create(NULL)->setDynamic();
static std::shared_ptr<Var> VARS = Var::create()->setDynamic();
static std::shared_ptr<Var> METHOD = Var::create(NULL)->setDynamic();
static std::shared_ptr<Var> IN_CATCH_FINALLY = Var::create(NULL)->setDynamic();
static std::shared_ptr<Var> SOURCE_PATH = Var::intern(NULL, NULL, NULL)->setDynamic();  // TODO
static std::shared_ptr<Var> COMPILER_OPTIONS;

static std::shared_ptr<const lisp_object> getCompilerOption(std::shared_ptr<const Keyword> k){
	return RT::get(COMPILER_OPTIONS->deref(), k);
}

static std::shared_ptr<Var> LINE = Var::create(std::make_shared<Integer>(0))->setDynamic();
static std::shared_ptr<Var> COLUMN = Var::create(std::make_shared<Integer>(0))->setDynamic();
static long lineDeref(){
	return (long)*std::dynamic_pointer_cast<const Number>(LINE->deref());
}
static long columnDeref(){
	return (long)*std::dynamic_pointer_cast<const Number>(COLUMN->deref());
}

static std::shared_ptr<Var> CLEAR_PATH = Var::create(NULL)->setDynamic();
static std::shared_ptr<Var> CLEAR_ROOT = Var::create(NULL)->setDynamic();
static std::shared_ptr<Var> CLEAR_SITES = Var::create(NULL)->setDynamic();

static const std::shared_ptr<Keyword> disableLocalsClearingKey = Keyword::intern("disable-locals-clearing");


class CompilerException : public std::runtime_error {
  public:
    CompilerException(std::string source, long line, long column, std::runtime_error cause) :
          std::runtime_error(std::string(cause.what()) + ", compiling:(" + source + ":" + std::to_string(line) + ":" + std::to_string(column) + ")"),
          source(source), line(line) {};
    const std::string source;
  	const long line;
};

class IllegalArgumentException : public std::runtime_error {
  public:
    explicit IllegalArgumentException(const std::string& what_arg) : std::runtime_error(what_arg) {};
    explicit IllegalArgumentException(const char* what_arg) : std::runtime_error(what_arg) {};
};

class IllegalStateException : public std::runtime_error {
  public:
    explicit IllegalStateException(const std::string& what_arg) : std::runtime_error(what_arg) {};
    explicit IllegalStateException(const char* what_arg) : std::runtime_error(what_arg) {};
};

class UnsupportedOperationException : public std::runtime_error {
  public:
    explicit UnsupportedOperationException(const std::string& what_arg) : std::runtime_error(what_arg) {};
    explicit UnsupportedOperationException(const char* what_arg) : std::runtime_error(what_arg) {};
};

typedef enum {
  STATEMENT,  //value ignored
	EXPRESSION, //value required
	RETURN,     //tail position relative to enclosing recur frame
	EVAL
} context;

typedef enum {
    PATH,
    BRANCH,
} PATHTYPE;

class PathNode : public lisp_object {
  public:
    const PATHTYPE type;
    const std::shared_ptr<const PathNode> parent;

    PathNode(PATHTYPE type, std::shared_ptr<const PathNode> parent) :
      type(type), parent(parent) {};
};

static std::shared_ptr<const Namespace> namespaceFor(std::shared_ptr<const Symbol> sym) {
  // TODO
  return Namespace::findOrCreate(Symbol::intern("TODO"));
}

static std::shared_ptr<const ISeq> fwdPath(std::shared_ptr<const PathNode> p1) {
  std::shared_ptr<const ISeq> ret = NULL;
  for(;p1 != NULL; p1 = p1->parent)
    ret = std::dynamic_pointer_cast<const ISeq>(ret->cons(p1));
  return ret;
}

static std::shared_ptr<const PathNode> commonPath(std::shared_ptr<const PathNode> n1, std::shared_ptr<const PathNode> n2) {
  std::shared_ptr<const ISeq> xp = fwdPath(n1);
  std::shared_ptr<const ISeq> yp = fwdPath(n2);
  if(xp->first() != yp->first())
    return NULL;
  while(xp->next()->first() && (xp->next()->first() == yp->next()->first())) {
    xp = xp->next();
    yp = yp->next();
  }
  return std::dynamic_pointer_cast<const PathNode>(xp->first());
}

static std::shared_ptr<const Symbol> tagOf(std::shared_ptr<const lisp_object> o){
	std::shared_ptr<const IMeta> m = std::dynamic_pointer_cast<const IMeta>(o);
	if(m) {
		std::shared_ptr<const lisp_object> meta = m->meta();
		std::shared_ptr<const lisp_object> tag = RT::get(meta, RT::TAG_KEY);
		std::shared_ptr<const Symbol> syTag = std::dynamic_pointer_cast<const Symbol>(tag);
		if(syTag)
		  return syTag;
		std::shared_ptr<const lisp_string> sTag = std::dynamic_pointer_cast<const lisp_string>(tag);
		if(sTag)
		return Symbol::intern("", sTag->toString());
	}
	return NULL;
}

static std::shared_ptr<const Namespace> currentNS() {
	return std::dynamic_pointer_cast<const Namespace>(RT::CURRENT_NS->deref());
}

static std::shared_ptr<const lisp_object> resolveIn(std::shared_ptr<const Namespace> n, std::shared_ptr<const Symbol> sym, bool allowPrivate) {
  // TODO
  return NULL;
}

static std::shared_ptr<const lisp_object> resolve(std::shared_ptr<const Symbol> sym, bool allowPrivate = false) {
  return resolveIn(currentNS(), sym, allowPrivate);
}

static int registerConstant(std::shared_ptr<const lisp_object> o) {
  // TODO
  return 0;
}

static void registerVar(std::shared_ptr<Var> var) {
  if(!VARS->isBound())
		return;
	std::shared_ptr<const IMap> varsMap = std::dynamic_pointer_cast<const IMap>(VARS->deref());
	std::shared_ptr<const lisp_object> id = RT::get(varsMap, var);
	if(id == NULL)
	  VARS->set(RT::assoc(varsMap, var, std::make_shared<Integer>(registerConstant(var))));
}

class ObjExpr;
class Expr : public lisp_object {
  public:
    virtual std::shared_ptr<const lisp_object> eval() const = 0;
    // virtual void emit(context C, const ObjExpr &objx /*, GeneratorAdapter */) = 0;
    virtual bool hasClass() const = 0;
	  virtual const std::type_info* getClass() const = 0;
	  virtual std::string toString(void) const {return "";};
};

class LiteralExpr : public Expr {
  public:
    virtual std::shared_ptr<const lisp_object> val() const = 0;
    virtual std::shared_ptr<const lisp_object> eval() const {
      return val();
    };
};

class AssignableExpr {
  public:
  	virtual std::shared_ptr<const lisp_object> evalAssign(Expr& val) const = 0;
  	// virtual void emitAssign(context C, const ObjExpr &objx /*, GeneratorAdapter */) = 0;
};

class MaybePrimitiveExpr : public Expr{
  public:
	  virtual bool canEmitPrimitive() const = 0;
	  // void emitUnboxed(context C, const ObjExpr &objx /*, GeneratorAdapter */) = 0;
};

const std::type_info* primClass(std::shared_ptr<const Symbol> sym) {
  if(sym == NULL)
    return NULL;
#define a(type) if(sym->getName() == "type") return &typeid(type)
  a(int);
  a(long);
  a(float);
  a(double);
  a(char);
  a(short);
  a(bool);
  a(void);
#undef a
  return NULL;
}

class HostExpr {
  public:
    static const std::type_info* tagToClass(std::shared_ptr<const lisp_object> tag);
    static const std::type_info* maybeClass(std::shared_ptr<const lisp_object> form, bool stringOkay);
  private:
    static const std::type_info* maybeSpecialTag(std::shared_ptr<const Symbol> sym);
};

const std::type_info* HostExpr::tagToClass(std::shared_ptr<const lisp_object> tag) {
  const std::type_info *c = NULL;
  std::shared_ptr<const Symbol> sym = std::dynamic_pointer_cast<const Symbol>(tag);
  if(sym) {
    if(sym->getNamespace() == "")
      c = maybeSpecialTag(sym);
  }
  if(c == NULL)
    c = maybeClass(tag, true);
  if(c)
    return c;
  throw IllegalArgumentException("Unable to resolve classname: " + tag->toString());
}

const std::type_info* HostExpr::maybeClass(std::shared_ptr<const lisp_object> form, bool stringOkay) {
  std::shared_ptr<const Symbol> sym = std::dynamic_pointer_cast<const Symbol>(form);
  if(sym) {
    if(sym->getNamespace() == "") {
      // TODO
    }
  }
  // TODO
  return NULL;
}

const std::type_info* HostExpr::maybeSpecialTag(std::shared_ptr<const Symbol> sym) {
  const std::type_info* c = primClass(sym);
  if(c)
    return c;
  // TODO
  return NULL;
}

static const std::type_info* maybePrimitiveType(std::shared_ptr<const Expr> e) {
  std::shared_ptr<const MaybePrimitiveExpr> mpe = std::dynamic_pointer_cast<const MaybePrimitiveExpr>(e);
  if(mpe && mpe->hasClass() && mpe->canEmitPrimitive()) {
    const std::type_info* c = e->getClass();
    if(isPrimitive(c))
			return c;
  }
  return NULL;
}

class ObjMethod {
  public:
    ObjMethod(std::shared_ptr<ObjExpr> objx, std::shared_ptr<ObjMethod> parent) :
              usesThis(false), objx(objx), parent(parent),
			  localsUsedInCatchFinally(LSet::EMPTY) {};

    bool usesThis;
    const std::shared_ptr<const IMap> locals;
    const std::shared_ptr<ObjExpr> objx;
    const std::shared_ptr<ObjMethod> parent;
    std::shared_ptr<const LSet> localsUsedInCatchFinally;
  private:
    virtual int numParams() = 0;
	  virtual std::string getMethodName() = 0;
};

class LocalBinding : public lisp_object {
  public:
    LocalBinding(int num, std::shared_ptr<const Symbol> sym, std::shared_ptr<const Symbol> tag,
                 std::shared_ptr<const Expr> init, bool isArg, std::shared_ptr<const PathNode> clearPathRoot) :
                 sym(sym), tag(tag), init(init), name(sym->getName()), isArg(isArg), clearPathRoot(clearPathRoot), 
                 canBeCleared(!((bool)getCompilerOption(disableLocalsClearingKey))), recurMistmatch(false),
                 used(false), idx(num) {};

    const std::shared_ptr<const Symbol> sym;
  	const std::shared_ptr<const Symbol> tag;
  	std::shared_ptr<const Expr> init;
  	const std::string name;
  	const bool isArg;
    const std::shared_ptr<const PathNode> clearPathRoot;
  	bool canBeCleared;
  	bool recurMistmatch;
    bool used;
    int idx;
    
    bool hasClass() const;
    
    const std::type_info* getClass() const;
    
    const std::type_info* getPrimitiveType() const {
      return maybePrimitiveType(init);
    };
};

bool LocalBinding::hasClass() const {
  if(init && init->hasClass() && isPrimitive(init->getClass())) {
    std::shared_ptr<const MaybePrimitiveExpr> mpe = std::dynamic_pointer_cast<const MaybePrimitiveExpr>(init);
    if(mpe == NULL)
      return false;
  }
  return (tag != NULL) || (init != NULL && init->hasClass());
}

const std::type_info* LocalBinding::getClass() const {
  return (tag != NULL) ? HostExpr::tagToClass(tag) : init->getClass();
}

class NilExpr : public LiteralExpr {
  public:
    virtual std::shared_ptr<const lisp_object> val() const {
      return NULL;
    };
    virtual bool hasClass() const {return true;};
	  virtual const std::type_info* getClass() const {return NULL;};
};
static const std::shared_ptr<const NilExpr> NIL_EXPR = std::make_shared<const NilExpr>();

class BooleanExpr : public LiteralExpr {
  public:
    BooleanExpr(bool tf) : tf(tf) {};
    virtual std::shared_ptr<const lisp_object> val() const {
      return tf ? T : F;
    };
    virtual bool hasClass() const {return true;};
	  virtual const std::type_info* getClass() const {
	    return &typeid(lisp_bool);
	  };
  private:
    bool tf;
};
static const std::shared_ptr<const BooleanExpr> TRUE_EXPR = std::make_shared<const BooleanExpr>(true);
static const std::shared_ptr<const BooleanExpr> FALSE_EXPR = std::make_shared<const BooleanExpr>(false);

class ObjExpr : public Expr {
  public:
    std::shared_ptr<const IMap> closes;
    // TODO
};

class LocalBindingExpr : public MaybePrimitiveExpr, public AssignableExpr, public std::enable_shared_from_this<LocalBindingExpr> {
  public:
    LocalBindingExpr(std::shared_ptr<LocalBinding> b, std::shared_ptr<const Symbol> tag);

    const std::shared_ptr<LocalBinding> b;
	  const std::shared_ptr<const Symbol> tag;
    const std::shared_ptr<const PathNode> clearPath;
    const std::shared_ptr<const PathNode> clearRoot;
    bool shouldClear = false;

    virtual std::shared_ptr<const lisp_object> eval() const {
      throw UnsupportedOperationException("Can't eval locals");
    };
	  virtual std::shared_ptr<const lisp_object> evalAssign(Expr&) const {
      throw UnsupportedOperationException("Can't eval locals");
    };
    virtual bool hasClass() const {return (tag != NULL) || b->hasClass();};
	  virtual const std::type_info* getClass() const {
	    if(tag)
        return HostExpr::tagToClass(tag);
      return b->getClass();
	  };
	  bool canEmitPrimitive() const {return b->getPrimitiveType();};
};

LocalBindingExpr::LocalBindingExpr(std::shared_ptr<LocalBinding> b, std::shared_ptr<const Symbol> tag) :
                 b(b), tag(tag),
                 clearPath(std::dynamic_pointer_cast<const PathNode>(CLEAR_PATH->get())),
                 clearRoot(std::dynamic_pointer_cast<const PathNode>(CLEAR_ROOT->get())) {
  if(b->getPrimitiveType() && tag)
    throw UnsupportedOperationException("Can't type hint a primitive local");
  std::shared_ptr<const ICollection> sites = std::dynamic_pointer_cast<const ICollection>(RT::get(CLEAR_SITES->get(),b));
  b->used = true;
  if(b->idx > 0) {
    if(sites) {
      for(std::shared_ptr<const ISeq> s = sites->seq(); s!=NULL; s = s->next()) {
        std::shared_ptr<LocalBindingExpr> o = std::const_pointer_cast<LocalBindingExpr>(std::dynamic_pointer_cast<const LocalBindingExpr>(s->first()));
        std::shared_ptr<const PathNode> common = commonPath(clearPath, o->clearPath);
        if(common && common->type == PATH)
          o->shouldClear = false;
      }
    }
    if(clearRoot == b->clearPathRoot) {
      shouldClear = true;
      sites = sites->cons(shared_from_this());
      CLEAR_SITES->set(RT::assoc(CLEAR_SITES->get(), b, sites));
    }
  }
}

class VarExpr : public Expr, public AssignableExpr {
  public:
    VarExpr(std::shared_ptr<Var> v, std::shared_ptr<const Symbol> tag) {};    // TODO
    virtual std::shared_ptr<const lisp_object> eval() const {return NULL;};   // TODO
    virtual bool hasClass() const {return false;};   // TODO
	virtual const std::type_info* getClass() const {return NULL;};    // TODO
  	virtual std::shared_ptr<const lisp_object> evalAssign(Expr& val) const {return NULL;};    // TODO
  // TODO
};

static void closeOver(std::shared_ptr<const LocalBinding> b, std::shared_ptr<ObjMethod> method) {
  if(b && method) {
    std::shared_ptr<const LocalBinding> lb = std::dynamic_pointer_cast<const LocalBinding>(RT::get(method->locals, b));
    if(lb) {
      if(lb->idx == 0)
        method->usesThis = true;
        if(IN_CATCH_FINALLY->deref())
          method->localsUsedInCatchFinally = std::dynamic_pointer_cast<const LSet>(
                  method->localsUsedInCatchFinally->cons(std::make_shared<Integer>(b->idx)));
    } else {
      method->objx->closes = std::dynamic_pointer_cast<const IMap>(RT::assoc(method->objx->closes, b, b));
			closeOver(b, method->parent);
    }
  }
}

static std::shared_ptr<LocalBinding> referenceLocal(std::shared_ptr<const Symbol> sym) {
  if(!LOCAL_ENV->isBound())
    return NULL;
  std::shared_ptr<LocalBinding> b = std::const_pointer_cast<LocalBinding>(std::dynamic_pointer_cast<const LocalBinding>(RT::get(LOCAL_ENV->deref(), sym)));
  if(b) {
    std::shared_ptr<ObjMethod> method = std::const_pointer_cast<ObjMethod>(std::dynamic_pointer_cast<const ObjMethod>(METHOD->deref()));
    if(b->idx == 0)
      method->usesThis = true;
    closeOver(b, method);
  }
  return b;
}

static std::shared_ptr<const Var> lookupVar(std::shared_ptr<const Symbol> sym, bool internNew, bool registerMacro = true) {
  // TODO
  return NULL;
}

static std::shared_ptr<const Var> isMacro(std::shared_ptr<const lisp_object> op) {
  std::shared_ptr<const Symbol> sym = std::dynamic_pointer_cast<const Symbol>(op);
  if(sym) {
    if(referenceLocal(sym))
      return NULL;
  }
  std::shared_ptr<const Var> v = std::dynamic_pointer_cast<const Var>(op);
  if(sym || v) {
    if(v == NULL)
      v = lookupVar(sym, false, false);
    if(v && v->isMacro()) {
      if(v->ns != currentNS() && !v->isPublic())
				throw new IllegalStateException("var: " + v->toString() + " is not public");
			return v;
    }
  }
  return NULL;
}

std::shared_ptr<const lisp_object> macroexpand1(std::shared_ptr<const lisp_object> x) {
  // TODO
  return x;
}

static std::shared_ptr<const lisp_object> macroexpand(std::shared_ptr<const lisp_object> form) {
	std::shared_ptr<const lisp_object> exf = macroexpand1(form);
	if(exf != form)
		return macroexpand(exf);
	return form;
}

static std::shared_ptr<const Expr> analyze(context c, std::shared_ptr<const lisp_object> form, std::string name = "");

static std::shared_ptr<const Expr> analyzeSymbol(std::shared_ptr<const Symbol> sym) {
  std::shared_ptr<const Symbol> tag = tagOf(sym);
  if(sym->getNamespace() == "") {
    std::shared_ptr<LocalBinding> b = referenceLocal(sym);
    if(b)
      return std::make_shared<const LocalBindingExpr>(b, tag);
  } else {
    if(namespaceFor(sym) == NULL) {
      std::shared_ptr<const Symbol> nsSym = Symbol::intern(sym->getNamespace());
      const std::type_info *c = HostExpr::maybeClass(nsSym, false);
      if(c) {
        // TODO
      }
    }
  }

  std::shared_ptr<const lisp_object> o = resolve(sym);
  std::shared_ptr<Var> v = std::const_pointer_cast<Var>(std::dynamic_pointer_cast<const Var>(o));
  if(v) {
    if(isMacro(v))
      throw std::runtime_error("Can't take value of a macro: " + v->toString());
    if(RT::boolCast(RT::get(v->meta(),RT::CONST_KEY)))
			return analyze(EXPRESSION, RT::list(QUOTE, v->get()));
		registerVar(v);
		return std::make_shared<VarExpr>(v, tag);
    // TODO
  }
  // TODO
}

static std::shared_ptr<const Expr> analyze(context c, std::shared_ptr<const lisp_object> form, std::string name) {
  try {
    std::shared_ptr<const LazySeq> mform = std::dynamic_pointer_cast<const LazySeq>(form);
    if(mform) {
      form = RT::seq(form);
      if(form == NULL)
		form = List::Empty;
        // std::shared_ptr<const IMeta> mform = List::Empty;
        // form = mform->with_meta(mform->meta());
    }
    if(form == NULL)
      return NIL_EXPR;
    if(*form == T)
      return TRUE_EXPR;
    if(*form == F)
      return FALSE_EXPR;
    std::shared_ptr<const Symbol> sym = std::dynamic_pointer_cast<const Symbol>(form);
    if(sym)
      return analyzeSymbol(sym);
    // TODO
  } catch (const std::runtime_error &e) {
    try {
      const CompilerException &ce = dynamic_cast<const CompilerException&>(e);
      throw ce;
    } catch (const std::bad_cast&) {
      throw CompilerException(SOURCE_PATH->deref()->toString(), lineDeref(), columnDeref(), e);
    }
  }
}


std::shared_ptr<const lisp_object> eval(std::shared_ptr<const lisp_object> form, bool freshLoader) {
	// Var.pushThreadBindings(RT.map(LOADER, RT.makeClassLoader()));  // TODO
	try {
	  long line = lineDeref();
		long column = columnDeref();
		std::shared_ptr<const IMeta> mform = std::dynamic_pointer_cast<const IMeta>(form);
		if(mform && mform->meta() && mform->meta()->containsKey(RT::LINE_KEY))
			line = (long)*std::dynamic_pointer_cast<const Number>(mform->meta()->valAt(RT::LINE_KEY));
		if(mform && mform->meta() && mform->meta()->containsKey(RT::COLUMN_KEY))
			column = (long)*std::dynamic_pointer_cast<const Number>(mform->meta()->valAt(RT::COLUMN_KEY));
		// Var.pushThreadBindings(RT.map(LINE, line, COLUMN, column));  // TODO
		try {
		  form = macroexpand(form);
		  std::shared_ptr<const ISeq> s = std::dynamic_pointer_cast<const ISeq>(form);
		  if(s && (s->first() == DO)) {
		    for(s = s->next(); s->next() != NULL; s = s->next())
		      eval(s->first(), false);
		    return eval(s->first(), false);
		  }
		  std::shared_ptr<const IType> t = std::dynamic_pointer_cast<const IType>(form);
		  if(t) {
		    // TODO
		  }
		  std::shared_ptr<const ICollection> c = std::dynamic_pointer_cast<const ICollection>(form);
		  if(c) {
		    std::shared_ptr<const Symbol> sy = std::dynamic_pointer_cast<const Symbol>(RT::first(c));
		    if(sy->getName().substr(0,3) == "def") {
		      std::shared_ptr<const ObjExpr> fexpr = std::dynamic_pointer_cast<const ObjExpr>(analyze(EXPRESSION, RT::list(FN, LVector::EMPTY, form), "eval" + RT::nextID()));
		      std::shared_ptr<const IFn> fn = std::dynamic_pointer_cast<const IFn>(fexpr->eval());
		      return fn->invoke();
		    }
		  }
		  std::shared_ptr<const Expr> expr = analyze(EVAL, form);
		  return expr->eval();
		} catch(const std::exception &e) {
  	  // Var.popThreadBindings();   // TODO
  	  throw e;
  	}
	} catch(const std::exception &e) {
	  // Var.popThreadBindings();   // TODO
	  throw e;
	}
}

















