#ifndef LAZYSEQ_HPP
#define LAZYSEQ_HPP

#include "Interfaces.hpp"

class LazySeq : public ISeq_inherit<ISeq>, public IPending, public IHashEq, public IMeta_inherit<LazySeq>, public std::enable_shared_from_this<LazySeq> {
  public:
    LazySeq(std::shared_ptr<const IFn> fn) : fn(fn) {};
    virtual std::shared_ptr<const ISeq> seq() const;
    virtual std::string toString(void) const;
    virtual size_t count(void) const;
    virtual std::shared_ptr<const lisp_object> first() const;
    virtual std::shared_ptr<const ISeq> next() const;
    virtual std::shared_ptr<const ISeq> more() const;
    
    std::shared_ptr<const lisp_object> sval();
  private:
    std::shared_ptr<const IFn> fn;
    std::shared_ptr<const lisp_object> sv;
    std::shared_ptr<const ISeq> s;
    
    LazySeq(std::shared_ptr<const IMap> m, std::shared_ptr<const ISeq> s) : IMeta_inherit(m), s(s) {};
		virtual std::shared_ptr<const LazySeq> with_meta_impl(std::shared_ptr<const IMap> meta) const;

    virtual std::shared_ptr<const ISeq>cons_impl(std::shared_ptr<const lisp_object>)const;
    virtual std::shared_ptr<const ISeq> empty_impl() const;
};

#endif /* LAZYSEQ_HPP */
