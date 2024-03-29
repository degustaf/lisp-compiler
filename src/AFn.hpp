#ifndef AFN_HPP
#define AFN_HPP

#include "Interfaces.hpp"

class AFn : public IFn/*, public std::enable_shared_from_this<AFn>*/ {
	public:
		std::shared_ptr<const lisp_object> throwArity(size_t n) const;
		virtual std::shared_ptr<const lisp_object> invoke() const {return throwArity(0);} ;
		virtual std::shared_ptr<const lisp_object> invoke(std::shared_ptr<const lisp_object>) const {return throwArity(1);} ;
		virtual std::shared_ptr<const lisp_object> invoke(std::shared_ptr<const lisp_object>,
				std::shared_ptr<const lisp_object>) const {return throwArity(2);} ;
		virtual std::shared_ptr<const lisp_object> invoke(std::shared_ptr<const lisp_object>,
				std::shared_ptr<const lisp_object>, std::shared_ptr<const lisp_object>) const {return throwArity(3);} ;
		virtual std::shared_ptr<const lisp_object> invoke(std::shared_ptr<const lisp_object>,
				std::shared_ptr<const lisp_object>, std::shared_ptr<const lisp_object>,
				std::shared_ptr<const lisp_object>) const {return throwArity(4);} ;
		virtual std::shared_ptr<const lisp_object> invoke(std::shared_ptr<const lisp_object>,
				std::shared_ptr<const lisp_object>, std::shared_ptr<const lisp_object>,
				std::shared_ptr<const lisp_object>, std::shared_ptr<const lisp_object>) const {return throwArity(5);} ;

		virtual std::shared_ptr<const lisp_object> applyTo(std::shared_ptr<const ISeq>) const = 0;
		virtual std::shared_ptr<const lisp_object> applyToHelper(std::shared_ptr<const IFn>, std::shared_ptr<const ISeq>) const;
};

template<class Derived>
class AFn_inherit : public AFn, public std::enable_shared_from_this<Derived> {
	public:
		virtual ~AFn_inherit() = default;
		virtual std::shared_ptr<const lisp_object> applyTo(std::shared_ptr<const ISeq>) const;
	private:
		AFn_inherit() = default;
		friend Derived;
};

#endif /* AFN_HPP */
