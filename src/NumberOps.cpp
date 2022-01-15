#include "Numbers.hpp"

class LongOps;
class DoubleOps;
class RatioOps;
class BigIntOps;
class BigDecimalOps;
class Ops {
  public:
    virtual const Ops &combine(const Ops&) const = 0;
    
    virtual const Ops &opsWith(const LongOps&) const = 0;
  	virtual const Ops &opsWith(const DoubleOps&) const = 0;
  	virtual const Ops &opsWith(const RatioOps&) const = 0;
  	virtual const Ops &opsWith(const BigIntOps&) const = 0;
  	virtual const Ops &opsWith(const BigDecimalOps&) const = 0;

    virtual bool isZero(std::shared_ptr<const Number> x) const = 0;
  	virtual bool isPos(std::shared_ptr<const Number> x) const = 0;
  	virtual bool isNeg(std::shared_ptr<const Number> x) const = 0;

  	virtual std::shared_ptr<const Number> add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;
  	virtual std::shared_ptr<const Number> addP(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;

  	virtual std::shared_ptr<const Number> multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;
  	virtual std::shared_ptr<const Number> multiplyP(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;

  	virtual std::shared_ptr<const Number> divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;
  	virtual std::shared_ptr<const Number> quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;
  	virtual std::shared_ptr<const Number> remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;

  	virtual bool equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;
  	virtual bool lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;
  	virtual bool lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;
  	virtual bool gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const = 0;

  	virtual std::shared_ptr<const Number> negate(std::shared_ptr<const Number> x) const = 0;
  	virtual std::shared_ptr<const Number> negateP(std::shared_ptr<const Number> x) const = 0;
  	virtual std::shared_ptr<const Number> inc(std::shared_ptr<const Number> x) const = 0;
  	virtual std::shared_ptr<const Number> incP(std::shared_ptr<const Number> x) const = 0;
  	virtual std::shared_ptr<const Number> dec(std::shared_ptr<const Number> x) const = 0;
  	virtual std::shared_ptr<const Number> decP(std::shared_ptr<const Number> x) const = 0;
};

class OpsP : public Ops {
  public:
  	virtual std::shared_ptr<const Number> addP(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  	  return add(x, y);
  	};
  	virtual std::shared_ptr<const Number> multiplyP(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  	  return multiply(x, y);
  	};
  	virtual std::shared_ptr<const Number> negateP(std::shared_ptr<const Number> x) const {
  	  return negate(x);
  	};
  	virtual std::shared_ptr<const Number> incP(std::shared_ptr<const Number> x) const {
  	  return inc(x);
  	};
  	virtual std::shared_ptr<const Number> decP(std::shared_ptr<const Number> x) const {
  	  return dec(x);
  	};
};

class LongOps : public Ops {
  public:
    virtual const Ops &combine(const Ops&) const;
    virtual const Ops &opsWith(const LongOps&) const;
  	virtual const Ops &opsWith(const RatioOps&) const;
  	virtual const Ops &opsWith(const DoubleOps&) const;
  	virtual const Ops &opsWith(const BigIntOps&) const;
  	virtual const Ops &opsWith(const BigDecimalOps&) const;

    virtual bool isZero(std::shared_ptr<const Number> x) const;
  	virtual bool isPos(std::shared_ptr<const Number> x) const;
  	virtual bool isNeg(std::shared_ptr<const Number> x) const;

  	virtual std::shared_ptr<const Number> add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> addP(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> multiplyP(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	
  	virtual bool equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> negate(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> negateP(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> inc(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> incP(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> dec(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> decP(std::shared_ptr<const Number> x) const;
  private:
    static long gcd(long u, long v);
};

class DoubleOps : public OpsP {
  public:
    virtual const Ops &combine(const Ops&) const;
    virtual const Ops &opsWith(const LongOps&) const;
  	virtual const Ops &opsWith(const RatioOps&) const;
  	virtual const Ops &opsWith(const DoubleOps&) const;
  	virtual const Ops &opsWith(const BigIntOps&) const;
  	virtual const Ops &opsWith(const BigDecimalOps&) const;

    virtual bool isZero(std::shared_ptr<const Number> x) const;
  	virtual bool isPos(std::shared_ptr<const Number> x) const;
  	virtual bool isNeg(std::shared_ptr<const Number> x) const;

  	virtual std::shared_ptr<const Number> add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual bool equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> negate(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> inc(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> dec(std::shared_ptr<const Number> x) const;
};

class BigIntOps : public OpsP {
  public:
    virtual const Ops &combine(const Ops&) const;
    virtual const Ops &opsWith(const LongOps&) const;
  	virtual const Ops &opsWith(const RatioOps&) const;
  	virtual const Ops &opsWith(const DoubleOps&) const;
  	virtual const Ops &opsWith(const BigIntOps&) const;
  	virtual const Ops &opsWith(const BigDecimalOps&) const;

    virtual bool isZero(std::shared_ptr<const Number> x) const;
  	virtual bool isPos(std::shared_ptr<const Number> x) const;
  	virtual bool isNeg(std::shared_ptr<const Number> x) const;

  	virtual std::shared_ptr<const Number> add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual bool equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> negate(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> inc(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> dec(std::shared_ptr<const Number> x) const;
};

class BigDecimalOps : public OpsP {
  public:
    virtual const Ops &combine(const Ops&) const;
    virtual const Ops &opsWith(const LongOps&) const;
  	virtual const Ops &opsWith(const RatioOps&) const;
  	virtual const Ops &opsWith(const DoubleOps&) const;
  	virtual const Ops &opsWith(const BigIntOps&) const;
  	virtual const Ops &opsWith(const BigDecimalOps&) const;

    virtual bool isZero(std::shared_ptr<const Number> x) const;
  	virtual bool isPos(std::shared_ptr<const Number> x) const;
  	virtual bool isNeg(std::shared_ptr<const Number> x) const;

  	virtual std::shared_ptr<const Number> add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual bool equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> negate(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> inc(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> dec(std::shared_ptr<const Number> x) const;
};

class RatioOps : public OpsP {
  public:
    virtual const Ops &combine(const Ops&) const;
    virtual const Ops &opsWith(const LongOps&) const;
  	virtual const Ops &opsWith(const RatioOps&) const;
  	virtual const Ops &opsWith(const DoubleOps&) const;
  	virtual const Ops &opsWith(const BigIntOps&) const;
  	virtual const Ops &opsWith(const BigDecimalOps&) const;

    virtual bool isZero(std::shared_ptr<const Number> x) const;
  	virtual bool isPos(std::shared_ptr<const Number> x) const;
  	virtual bool isNeg(std::shared_ptr<const Number> x) const;

  	virtual std::shared_ptr<const Number> add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual std::shared_ptr<const Number> remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual bool equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;
  	virtual bool gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const;

  	virtual std::shared_ptr<const Number> negate(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> inc(std::shared_ptr<const Number> x) const;
  	virtual std::shared_ptr<const Number> dec(std::shared_ptr<const Number> x) const;
};

static const LongOps LONG_OPS = LongOps();
static const DoubleOps DOUBLE_OPS = DoubleOps();
static const BigIntOps BIGINT_OPS = BigIntOps();
static const BigDecimalOps BIGDECIMAL_OPS = BigDecimalOps();
static const RatioOps RATIO_OPS = RatioOps();


// Helper Functions
const Ops& ops(std::shared_ptr<const lisp_object> x) {
	if(std::dynamic_pointer_cast<const Integer>(x))
		return LONG_OPS;
	if(std::dynamic_pointer_cast<const Float>(x))
		return DOUBLE_OPS;
	if(std::dynamic_pointer_cast<const BigInt>(x))
		return BIGINT_OPS;
	if(std::dynamic_pointer_cast<const Ratio>(x))
		return RATIO_OPS;
	if(std::dynamic_pointer_cast<const BigDecimal>(x))
		return BIGDECIMAL_OPS;
	return LONG_OPS;
}

std::shared_ptr<const Number> num(long x) {
  return std::make_shared<const Integer>(x);
}

std::shared_ptr<const Number> num(double x) {
  return std::make_shared<const Float>(x);
}

long add(long x, long y) {
  long ret = x + y;
  if((ret ^ x) < 0 && (ret ^ y) < 0)
    throw std::overflow_error("Integer Overflow");
  return ret;
}

std::shared_ptr<const Number> minus(std::shared_ptr<const lisp_object> x, std::shared_ptr<const lisp_object> y) {
  std::shared_ptr<const Number> nx =std::dynamic_pointer_cast<const Number>(x);
  std::shared_ptr<const Number> ny =std::dynamic_pointer_cast<const Number>(y);
	const Ops &yops = ops(y);
	return ops(x).combine(yops).add(nx, yops.negate(ny));
}

long multiply(long x, long y) {
  if(x == LONG_MIN && y < 0)
    throw std::overflow_error("Integer Overflow");
  long ret = x * y;
  if(y != 0 && ret/y != x)
    throw std::overflow_error("Integer Overflow");
  return ret;
}

std::shared_ptr<const Number> multiply(std::shared_ptr<const lisp_object> x, std::shared_ptr<const lisp_object> y) {
  std::shared_ptr<const Number> nx =std::dynamic_pointer_cast<const Number>(x);
  std::shared_ptr<const Number> ny =std::dynamic_pointer_cast<const Number>(y);
	return ops(x).combine(ops(y)).multiply(nx, ny);
}

std::shared_ptr<const Number> divide(BigInt n, BigInt d) {
  if(d.isZero())
    throw  std::domain_error("Divide by zero error.");
  BigInt gcd = n.gcd(d);
  if(gcd.isZero())
    return std::make_shared<const BigInt>(BigInt::ZERO);
  n = n / gcd;
  d = d / gcd;
  if(d == BigInt::ONE)
    return std::make_shared<const BigInt>(n);
  if(d == -BigInt::ONE)
    return std::make_shared<const BigInt>(-n);
  return std::make_shared<const Ratio>((d.signum() < 0 ? -n : n),(d.signum() < 0 ? -d : d));
}

double quotient(double n, double d) {
  if(d == 0)
    throw std::domain_error("Divide by zero error.");

	double q = n / d;
	if(q <= LONG_MAX && q >= LONG_MIN)
		return (double)(long) q;
	return (double) BigDecimal(q).toBigInt();
}

double remainder(double n, double d) {
  if(d == 0)
    throw std::domain_error("Divide by zero error.");

	double q = n / d;
	if(q <= LONG_MAX && q >= LONG_MIN)
	  return n - ((long)q) * d;
	return n - ((double)BigDecimal(q).toBigInt()) * d;
}

BigInt toBigInt(std::shared_ptr<const Number> x) {
  std::shared_ptr<const BigInt> bx = std::dynamic_pointer_cast<const BigInt>(x);
  if(bx)
    return *bx;
  return BigInt(*x);
}

BigDecimal toBigDecimal(std::shared_ptr<const Number> x) {
  std::shared_ptr<const BigDecimal> bdx = std::dynamic_pointer_cast<const BigDecimal>(x);
  if(bdx)
    return *bdx;
  std::shared_ptr<const BigInt> bx = std::dynamic_pointer_cast<const BigInt>(x);
  if(bx)
    return BigDecimal(*bx);
  return BigDecimal((double) *x);
}

Ratio toRatio(std::shared_ptr<const Number> x){
	std::shared_ptr<const Ratio> rx = std::dynamic_pointer_cast<const Ratio>(x);
	if(rx)
	  return *rx;
	std::shared_ptr<const BigDecimal> bdx = std::dynamic_pointer_cast<const BigDecimal>(x);
	if(bdx) {
	  BigDecimal bx = *bdx;
	  BigInt bv = bx.unscaledValue();
		int scale = bx.getScale();
		if(scale < 0)
			return Ratio(bv * BigInt::TEN.pow(-scale), BigInt::ONE);
		else
			return Ratio(bv, BigInt::TEN.pow(scale));
		}
	return Ratio(toBigInt(x), BigInt::ONE);
}

// LongOps
const Ops &LongOps::combine(const Ops &y) const {
  return y.opsWith(*this);
}
const Ops &LongOps::opsWith(const LongOps &x) const {
  return *this;
}
const Ops &LongOps::opsWith(const RatioOps&) const {
  return RATIO_OPS;
}
const Ops &LongOps::opsWith(const DoubleOps &x) const {
  return DOUBLE_OPS;
}
const Ops &LongOps::opsWith(const BigIntOps&) const {
  return BIGINT_OPS;
}
const Ops &LongOps::opsWith(const BigDecimalOps&) const {
  return BIGDECIMAL_OPS;
}

bool LongOps::isZero(std::shared_ptr<const Number> x) const {
  return ((long) *x) == 0;
}
bool LongOps::isPos(std::shared_ptr<const Number> x) const {
  return ((long) *x) > 0;
}
bool LongOps::isNeg(std::shared_ptr<const Number> x) const {
  return ((long) *x) < 0;
}

std::shared_ptr<const Number> LongOps::add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return num(::add((long)*x, (long)*y));
}
std::shared_ptr<const Number> LongOps::addP(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  long lx = (long) *x;
  long ly = (long) *y;
  long ret = lx + ly;
  if((lx ^ ret) < 0 && (ly ^ ret) < 0)
    return BIGINT_OPS.add(x, y);
  return num(ret);
}

std::shared_ptr<const Number> LongOps::multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return num(::multiply((long) *x, (long) *y));
}
std::shared_ptr<const Number> LongOps::multiplyP(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  long lx = (long) *x;
  long ly = (long) *y;
  if(lx == LONG_MIN && ly < 0)
    return BIGINT_OPS.multiply(x, y);
  long ret = lx * ly;
  if((ly != 0) && (ret/ly != lx))
    return BIGINT_OPS.multiply(x, y);
  return num(ret);
}

std::shared_ptr<const Number> LongOps::divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  long n = (long) *x;
	long val = (long) *y;
	long gcd_ = gcd(n, val);
	if(gcd_ == 0)
		return num((long)0);

	n = n / gcd_;
	long d = val / gcd_;
	if(d == 1)
		return num(n);
	if(d < 0) {
		n = -n;
		d = -d;
	}
	return std::make_shared<const Ratio>(BigInt(n), BigInt(d));
}
std::shared_ptr<const Number> LongOps::quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return num((long)*x / (long)*y);
}
std::shared_ptr<const Number> LongOps::remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return num((long)*x % (long)*y);
}

bool LongOps::equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return (long) *x == (long) *y;
}

bool LongOps::lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return (long) *x < (long) *y;
}

bool LongOps::lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return (long) *x <= (long) *y;
}

bool LongOps::gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return (long) *x >= (long) *y;
}

std::shared_ptr<const Number> LongOps::negate(std::shared_ptr<const Number> x) const {
  return num(-(long)*x);
}

std::shared_ptr<const Number> LongOps::negateP(std::shared_ptr<const Number> x) const {
  long val = (long) *x;
  if(val == LONG_MIN)
    return std::make_shared<BigInt>(-BigInt(val));
  return num(-val);
}

std::shared_ptr<const Number> LongOps::inc(std::shared_ptr<const Number> x) const {
  return num((long)*x + 1);
}

std::shared_ptr<const Number> LongOps::incP(std::shared_ptr<const Number> x) const {
  long val = (long) *x;
  if(val == LONG_MAX)
    return std::make_shared<BigInt>(BigInt(val) + BigInt::ONE);
  return num(val + 1);
}

std::shared_ptr<const Number> LongOps::dec(std::shared_ptr<const Number> x) const {
  return num((long)*x - 1);
}
std::shared_ptr<const Number> LongOps::decP(std::shared_ptr<const Number> x) const {
  long val = (long) *x;
  if(val == LONG_MIN)
    return std::make_shared<BigInt>(BigInt(val) - BigInt::ONE);
  return num(val - 1);
}

long LongOps::gcd(long u, long v) {
  while(v != 0) {
		long r = u % v;
		u = v;
		v = r;
	}
	return u;
}


// DoubleOps
const Ops &DoubleOps::combine(const Ops &y) const {
  return y.opsWith(*this);
}
const Ops &DoubleOps::opsWith(const LongOps &x) const {
  return *this;
}
const Ops &DoubleOps::opsWith(const RatioOps&) const {
  return *this;
}
const Ops &DoubleOps::opsWith(const DoubleOps &x) const {
  return *this;
}
const Ops &DoubleOps::opsWith(const BigIntOps&) const {
  return *this;
}
const Ops &DoubleOps::opsWith(const BigDecimalOps&) const {
  return *this;
}

bool DoubleOps::isZero(std::shared_ptr<const Number> x) const {
  return ((double) *x) == 0;
}
bool DoubleOps::isPos(std::shared_ptr<const Number> x) const {
  return ((double) *x) > 0;
}
bool DoubleOps::isNeg(std::shared_ptr<const Number> x) const {
return ((double) *x) < 0;
}

std::shared_ptr<const Number> DoubleOps::add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return num((double)*x + (double)*y);
}

std::shared_ptr<const Number> DoubleOps::multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return num((double)*x * (double)*y);
}

std::shared_ptr<const Number> DoubleOps::divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return num((double)*x / (double)*y);
}
std::shared_ptr<const Number> DoubleOps::quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return num(::quotient((double)*x, (double)*y));
}
std::shared_ptr<const Number> DoubleOps::remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return num(::remainder((double)*x, (double)*y));
}

bool DoubleOps::equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return (double) *x == (double) *y;
}

bool DoubleOps::lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
    return (double) *x < (double) *y;
}

bool DoubleOps::lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
    return (double) *x <= (double) *y;
}

bool DoubleOps::gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
    return (double) *x >= (double) *y;
}

std::shared_ptr<const Number> DoubleOps::negate(std::shared_ptr<const Number> x) const {
  return num(-(double) *x);
}

std::shared_ptr<const Number> DoubleOps::inc(std::shared_ptr<const Number> x) const {
  return num((double) *x + 1);
}

std::shared_ptr<const Number> DoubleOps::dec(std::shared_ptr<const Number> x) const {
  return num((double) *x - 1);
}


// BigIntOps
const Ops &BigIntOps::combine(const Ops& y) const {
  return y.opsWith(*this);
}
const Ops &BigIntOps::opsWith(const LongOps&) const {
  return *this;
}
const Ops &BigIntOps::opsWith(const RatioOps&) const {
  return RATIO_OPS;
}
const Ops &BigIntOps::opsWith(const DoubleOps&) const {
  return DOUBLE_OPS;
}
const Ops &BigIntOps::opsWith(const BigIntOps&) const {
  return *this;
}
const Ops &BigIntOps::opsWith(const BigDecimalOps&) const {
  return BIGDECIMAL_OPS;
}

bool BigIntOps::isZero(std::shared_ptr<const Number> x) const {
  BigInt bx = toBigInt(x);
  return bx == BigInt::ZERO;
}
bool BigIntOps::isPos(std::shared_ptr<const Number> x) const {
  BigInt bx = toBigInt(x);
  return bx.signum() > 0;
}
bool BigIntOps::isNeg(std::shared_ptr<const Number> x) const {
  BigInt bx = toBigInt(x);
  return bx.signum() < 0;
}

std::shared_ptr<const Number> BigIntOps::add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  BigInt ret = toBigInt(x) + toBigInt(y);
  return std::make_shared<const BigInt>(ret);
}

std::shared_ptr<const Number> BigIntOps::multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  BigInt ret = toBigInt(x) * toBigInt(y);
  return std::make_shared<const BigInt>(ret);
}

std::shared_ptr<const Number> BigIntOps::divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return ::divide(toBigInt(x), toBigInt(x));
}
std::shared_ptr<const Number> BigIntOps::quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  BigInt ret = toBigInt(x) / toBigInt(y);
  return std::make_shared<const BigInt>(ret);
}
std::shared_ptr<const Number> BigIntOps::remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  BigInt ret = toBigInt(x) % toBigInt(y);
  return std::make_shared<const BigInt>(ret);
}

bool BigIntOps::equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return toBigInt(x) == toBigInt(y);
}

bool BigIntOps::lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return toBigInt(x) < toBigInt(y);
}

bool BigIntOps::lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return toBigInt(x) <= toBigInt(y);
}

bool BigIntOps::gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return toBigInt(x) >= toBigInt(y);
}

std::shared_ptr<const Number> BigIntOps::negate(std::shared_ptr<const Number> x) const {
  return std::make_shared<BigInt>(-toBigInt(x));
}

std::shared_ptr<const Number> BigIntOps::inc(std::shared_ptr<const Number> x) const {
  return std::make_shared<BigInt>(toBigInt(x) + BigInt::ONE);
}

std::shared_ptr<const Number> BigIntOps::dec(std::shared_ptr<const Number> x) const {
  return std::make_shared<BigInt>(toBigInt(x) - BigInt::ONE);
}


// BigDecimalOps
const Ops &BigDecimalOps::combine(const Ops &y) const {
  return y.opsWith(*this);
}
const Ops &BigDecimalOps::opsWith(const LongOps&) const {
  return *this;
}
const Ops &BigDecimalOps::opsWith(const RatioOps&) const {
  return *this;
}
const Ops &BigDecimalOps::opsWith(const DoubleOps&) const {
  return DOUBLE_OPS;
}
const Ops &BigDecimalOps::opsWith(const BigIntOps&) const {
  return *this;
}
const Ops &BigDecimalOps::opsWith(const BigDecimalOps&) const {
  return *this;
}

bool BigDecimalOps::isZero(std::shared_ptr<const Number> x) const {
  return toBigDecimal(x).signum() == 0;
}
bool BigDecimalOps::isPos(std::shared_ptr<const Number> x) const {
  return toBigDecimal(x).signum() > 0;
}
bool BigDecimalOps::isNeg(std::shared_ptr<const Number> x) const {
  return toBigDecimal(x).signum() < 0;
}

std::shared_ptr<const Number> BigDecimalOps::add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  BigDecimal ret = toBigDecimal(x) + toBigDecimal(y);
  return std::make_shared<const BigDecimal>(ret);
}

std::shared_ptr<const Number> BigDecimalOps::multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  BigDecimal ret = toBigDecimal(x) * toBigDecimal(y);
  return std::make_shared<const BigDecimal>(ret);
}

std::shared_ptr<const Number> BigDecimalOps::divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  BigDecimal ret = toBigDecimal(x) / toBigDecimal(y);
  return std::make_shared<const BigDecimal>(ret);
}
std::shared_ptr<const Number> BigDecimalOps::quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  BigDecimal numerator = toBigDecimal(x);
  BigDecimal denominator = toBigDecimal(y);
  BigDecimal ret = numerator.divideToIntegralValue(denominator);
  return std::make_shared<const BigDecimal>(ret);
}
std::shared_ptr<const Number> BigDecimalOps::remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  BigDecimal ret = toBigDecimal(x) % toBigDecimal(y);
  return std::make_shared<const BigDecimal>(ret);
}

bool BigDecimalOps::equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  BigDecimal second = toBigDecimal(y);
  return toBigDecimal(x).equiv(second);
}

bool BigDecimalOps::lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return toBigDecimal(x) < toBigDecimal(y);
}

bool BigDecimalOps::lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return toBigDecimal(x) <= toBigDecimal(y);
}

bool BigDecimalOps::gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  return toBigDecimal(x) >= toBigDecimal(y);
}

std::shared_ptr<const Number> BigDecimalOps::negate(std::shared_ptr<const Number> x) const {
  BigDecimal ret = -toBigDecimal(x);
  return std::make_shared<const BigDecimal>(ret);
}

std::shared_ptr<const Number> BigDecimalOps::inc(std::shared_ptr<const Number> x) const {
  BigDecimal ret = toBigDecimal(x) + BigDecimal::ONE;
  return std::make_shared<const BigDecimal>(ret);
}

std::shared_ptr<const Number> BigDecimalOps::dec(std::shared_ptr<const Number> x) const {
  BigDecimal ret = toBigDecimal(x) - BigDecimal::ONE;
  return std::make_shared<const BigDecimal>(ret);
}


// RatioOps
const Ops &RatioOps::combine(const Ops &y) const {
  return y.opsWith(*this);
}
const Ops &RatioOps::opsWith(const LongOps &) const {
  return *this;
}
const Ops &RatioOps::opsWith(const RatioOps&) const {
  return *this;
}
const Ops &RatioOps::opsWith(const DoubleOps&) const {
  return DOUBLE_OPS;
}
const Ops &RatioOps::opsWith(const BigIntOps&) const {
  return *this;
}
const Ops &RatioOps::opsWith(const BigDecimalOps&) const {
  return BIGDECIMAL_OPS;
}

bool RatioOps::isZero(std::shared_ptr<const Number> x) const {
  Ratio r = toRatio(x);
  return r.numerator.signum() == 0;
}
bool RatioOps::isPos(std::shared_ptr<const Number> x) const {
  Ratio r = toRatio(x);
  return r.numerator.signum() > 0;
}
bool RatioOps::isNeg(std::shared_ptr<const Number> x) const {
  Ratio r = toRatio(x);
  return r.numerator.signum() < 0;
}

std::shared_ptr<const Number> RatioOps::add(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  Ratio rx = toRatio(x);
	Ratio ry = toRatio(y);
	return  ::divide((ry.numerator * rx.denominator) + (rx.numerator * ry.denominator), ry.denominator * rx.denominator);
}

std::shared_ptr<const Number> RatioOps::multiply(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  Ratio rx = toRatio(x);
	Ratio ry = toRatio(y);
	return  ::divide(rx.numerator * ry.numerator, rx.denominator * ry.denominator);
}

std::shared_ptr<const Number> RatioOps::divide(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  Ratio rx = toRatio(x);
	Ratio ry = toRatio(y);
	return  ::divide(rx.numerator * ry.denominator, rx.denominator * ry.numerator);
}

std::shared_ptr<const Number> RatioOps::quotient(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  Ratio rx = toRatio(x);
	Ratio ry = toRatio(y);
	BigInt q = (rx.numerator * ry.denominator) / (rx.denominator * ry.numerator);
	return std::make_shared<const BigInt>(q);
}

std::shared_ptr<const Number> RatioOps::remainder(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  Ratio rx = toRatio(x);
	Ratio ry = toRatio(y);
	BigInt q = (rx.numerator * ry.denominator) / (rx.denominator * ry.numerator);
	return ::minus(x, ::multiply(std::make_shared<const BigInt>(q), y));
}

bool RatioOps::equiv(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  Ratio rx = toRatio(x);
	Ratio ry = toRatio(y);
	return (rx.numerator == ry.numerator) && (rx.denominator == ry.denominator);
}

bool RatioOps::lt(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  Ratio rx = toRatio(x);
	Ratio ry = toRatio(y);
	return rx.numerator * ry.denominator < ry.numerator * rx.denominator;
}

bool RatioOps::lte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  Ratio rx = toRatio(x);
	Ratio ry = toRatio(y);
	return rx.numerator * ry.denominator <= ry.numerator * rx.denominator;
}

bool RatioOps::gte(std::shared_ptr<const Number> x, std::shared_ptr<const Number> y) const {
  Ratio rx = toRatio(x);
	Ratio ry = toRatio(y);
	return rx.numerator * ry.denominator >= ry.numerator * rx.denominator;
}

std::shared_ptr<const Number> RatioOps::negate(std::shared_ptr<const Number> x) const {
  Ratio rx = toRatio(x);
  return std::make_shared<const Ratio>(-rx.numerator, rx.denominator);
}

std::shared_ptr<const Number> RatioOps::inc(std::shared_ptr<const Number> x) const {
  Ratio rx = toRatio(x);
  return std::make_shared<const Ratio>(rx.numerator + rx.denominator, rx.denominator);
}

std::shared_ptr<const Number> RatioOps::dec(std::shared_ptr<const Number> x) const {
  Ratio rx = toRatio(x);
  return std::make_shared<const Ratio>(rx.numerator - rx.denominator, rx.denominator);
}
