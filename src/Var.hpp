#ifndef VAR_HPP
#define VAR_HPP

#include "Interfaces.hpp"
#include "ARef.hpp"
#include "Namespace.hpp"

class Var : public ARef /*, public IFn, public Settable*/ {
  public:
    std::shared_ptr<Namespace> ns;
    virtual std::string toString(void) const;
    virtual std::shared_ptr<const lisp_object> deref();
    std::shared_ptr<Var> setDynamic(); /* {
      return std::dynamic_pointer_cast<Var>(shared_from_this());
    }; */
    bool isBound() const;
    std::shared_ptr<const lisp_object> get();
    std::shared_ptr<const lisp_object> set(std::shared_ptr<const lisp_object> val) {return val;};   // TODO
    bool isMacro() const {return false;};   // TODO
    bool isPublic() const {return false;};   // TODO

    static std::shared_ptr<Var> create(std::shared_ptr<const lisp_object>); /* {
      return std::make_shared<Var>(); // TODO
    };*/
    static std::shared_ptr<Var> create(); /* {
      return std::make_shared<Var>(); // TODO
    }; */
    static std::shared_ptr<Var> intern(std::shared_ptr<const Namespace>, std::shared_ptr<const Symbol>, std::shared_ptr<const lisp_object>); /* {
		return std::make_shared<Var>(); // TODO
	}; */
    static void pushThreadBindings(std::shared_ptr<const Associative> bindings) {};  // TODO
    static void popThreadBindings() {};   // TODO
};

#endif /* VAR_HPP */
