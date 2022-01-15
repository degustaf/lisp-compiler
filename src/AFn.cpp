#include "AFn.hpp"

#include "Exception.hpp"
#include "Util.hpp"

std::shared_ptr<const lisp_object> AFn::throwArity(size_t n) const {
	throw ArityException(n, toString());
}

std::shared_ptr<const lisp_object> AFn::applyToHelper(std::shared_ptr<const IFn>, std::shared_ptr<const ISeq>) const {
	// TODO
	return NULL;
}

template<class Derived>
std::shared_ptr<const lisp_object> AFn_inherit<Derived>::applyTo(std::shared_ptr<const ISeq> arglist) const {
	return applyToHelper(std::dynamic_pointer_cast<const IFn>(std::enable_shared_from_this<Derived>::shared_from_this()), ret1(arglist, arglist = NULL));
}
