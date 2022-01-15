#ifndef SET_HPP
#define SET_HPP

#include "Interfaces.hpp"

class LSet : public ISet {
  public:
    static std::shared_ptr<const LSet> EMPTY;
  private:
};

#endif /* SET_HPP */
