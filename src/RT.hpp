#ifndef RT_HPP
#define RT_HPP

#include <memory>

#include "Keyword.hpp"
#include "List.hpp"
#include "Namespace.hpp"
#include "Var.hpp"

class RT {
  public:
    static const std::shared_ptr<Namespace> CLOJURE_NS;
    static const std::shared_ptr<const Keyword> LINE_KEY;
    static const std::shared_ptr<const Keyword> COLUMN_KEY;
    static const std::shared_ptr<const Keyword> TAG_KEY;
    static const std::shared_ptr<const Keyword> CONST_KEY;
    
    static const std::shared_ptr<Var> CURRENT_NS;
    
    static std::shared_ptr<const lisp_object> first(std::shared_ptr<const lisp_object> o) {
      return o;   // TODO
    };
    static std::shared_ptr<const lisp_object> list() {return NULL;};
    static std::shared_ptr<const lisp_object> list(std::shared_ptr<const lisp_object> o1) {
      return std::make_shared<List>(o1);
    };
    static std::shared_ptr<const lisp_object> list(std::shared_ptr<const lisp_object> o1,
                                                   std::shared_ptr<const lisp_object> o2) {
      return std::make_shared<List>(o2)->cons(o1);
    };
    static std::shared_ptr<const lisp_object> list(std::shared_ptr<const lisp_object> o1,
                                                   std::shared_ptr<const lisp_object> o2,
                                                   std::shared_ptr<const lisp_object> o3) {
      return std::make_shared<List>(o3)->cons(o2)->cons(o1);
    };
    static int nextID(){return 0;}; // TODO
    static std::shared_ptr<const ISeq> seq(std::shared_ptr<const lisp_object>) {return List::Empty;};  // TODO
    static std::shared_ptr<const ICollection> cons(std::shared_ptr<const lisp_object> x,
                                                   std::shared_ptr<const lisp_object> coll) {return NULL;};  // TODO
    static std::shared_ptr<const lisp_object> get(std::shared_ptr<const lisp_object> coll,
                                                  std::shared_ptr<const lisp_object> key);
    static std::shared_ptr<const Associative> assoc(std::shared_ptr<const lisp_object> coll,
                                                    std::shared_ptr<const lisp_object> key,
                                                    std::shared_ptr<const lisp_object> val) {return NULL;};   // TODO
    static std::shared_ptr<const IMap> map(std::initializer_list<std::shared_ptr<const lisp_object> >);
    static bool boolCast(std::shared_ptr<const lisp_object>) {return false;};   // TODO
};

#endif /* RT_HPP */
