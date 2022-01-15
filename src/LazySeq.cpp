#include "LazySeq.hpp"

#include "List.hpp"
#include "RT.hpp"
#include "Util.hpp"

std::shared_ptr<const LazySeq> LazySeq::with_meta_impl(std::shared_ptr<const IMap> m) const {
	return std::shared_ptr<LazySeq>(new LazySeq(m, seq()));
}

std::shared_ptr<const ISeq> LazySeq::seq() const {
  std::shared_ptr<LazySeq> ls = std::make_shared<LazySeq>(*this);
  if(ls->sv) {
    std::shared_ptr<const lisp_object> o;
    std::shared_ptr<LazySeq> ls2;
    do {
      o = ls->sv ? ls->sv : ls->s;
      ls2 = std::const_pointer_cast<LazySeq>(std::dynamic_pointer_cast<const LazySeq>(o));
    } while(ls2);
    ls->s = RT::seq(o);
  }
  return ls->s;
}

std::string LazySeq::toString(void) const {
  return ::toString(shared_from_this());
}

size_t LazySeq::count(void) const {
  size_t c = 0;
	for(std::shared_ptr<const ISeq> s = seq(); s != NULL; s = s->next())
		c++;                                                                                
	return c;
}

std::shared_ptr<const ISeq> LazySeq::empty_impl() const {
  return List::Empty;
}

std::shared_ptr<const lisp_object> LazySeq::first() const {
  std::shared_ptr<const ISeq> s = seq();
	if(s == NULL)
		return NULL;
	return s->first();
}

std::shared_ptr<const ISeq> LazySeq::next() const {
  std::shared_ptr<const ISeq> s = seq();
	if(s == NULL)
		return NULL;
	return s->next();
}

std::shared_ptr<const ISeq> LazySeq::more() const {
  std::shared_ptr<const ISeq> s = seq();
	if(s == NULL)
		return List::Empty;
	return s->next();
}

std::shared_ptr<const ISeq> LazySeq::cons_impl(std::shared_ptr<const lisp_object> o) const {
	return std::dynamic_pointer_cast<const ISeq>(RT::cons(o, seq()));
}


std::shared_ptr<const lisp_object> LazySeq::sval() {
  if(fn) {
    sv = fn->invoke();
    fn = NULL;
  }
  if(sv)
    return sv;
  return s;
}

