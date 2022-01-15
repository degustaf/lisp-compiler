#include "RT.hpp"

const std::shared_ptr<Namespace> RT::CLOJURE_NS = Namespace::findOrCreate(Symbol::intern("clojure.core"));
const std::shared_ptr<const Keyword> RT::LINE_KEY = Keyword::intern("", "line");
const std::shared_ptr<const Keyword> RT::COLUMN_KEY = Keyword::intern("", "column");
const std::shared_ptr<const Keyword> RT::TAG_KEY = Keyword::intern("", "tag");
const std::shared_ptr<const Keyword> RT::CONST_KEY = Keyword::intern("", "const");

const std::shared_ptr<Var> RT::CURRENT_NS = Var::intern(CLOJURE_NS, Symbol::intern("*ns*"),
                                                CLOJURE_NS)->setDynamic();

std::shared_ptr<const lisp_object> RT::get(std::shared_ptr<const lisp_object> coll,
                                           std::shared_ptr<const lisp_object> key) {
  if(coll == NULL)
    return NULL;
  std::shared_ptr<const ILookup> il = std::dynamic_pointer_cast<const ILookup>(coll);
  if(il)
    return il->valAt(key);
  std::shared_ptr<const ISet> is = std::dynamic_pointer_cast<const ISet>(coll);
  if(is)
    return is->get(key);
  // if(key instanceof Number && (coll instanceof String || coll.getClass().isArray()))   // TODO
	// if(coll instanceof ITransientSet)    // TODO
	return NULL;
}

std::shared_ptr<const IMap> RT::map(std::initializer_list<std::shared_ptr<const lisp_object> > init) {
  if(init.size() == 0)
    return LMap::EMPTY;
  return LMap::EMPTY;   // TODO
}

