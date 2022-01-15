#ifndef NUMBERS_HPP
#define NUMBERS_HPP

#include "Interfaces.hpp"

#include <vector>
#include <climits>

class BigInt;
class BigDecimal;
class Ratio;

class Number : public lisp_object {
	public:
		// virtual operator char() const = 0;
		// virtual operator int() const = 0;
		// virtual operator short() const = 0;
		virtual explicit operator long() const = 0;
		// virtual operator float() const = 0;
		virtual explicit operator double() const = 0;
		virtual explicit operator BigInt() const = 0;
		virtual explicit operator BigDecimal() const = 0;
		virtual explicit operator Ratio() const = 0;
};

class Integer : public Number {
	public:
		Integer(long i): val(i) {};

		virtual std::string toString(void) const;
		virtual explicit operator long() const;
		virtual explicit operator double() const;
		virtual explicit operator BigInt() const;
		virtual explicit operator BigDecimal() const;
		virtual explicit operator Ratio() const;
	private:
		const long val;
};

class Float : public Number {
	public:
		Float(double x): val(x) {};

		virtual std::string toString(void) const;
		virtual explicit operator long() const;
		virtual explicit operator double() const;
		virtual explicit operator BigInt() const;
		virtual explicit operator BigDecimal() const;
		virtual explicit operator Ratio() const;
	private:
		const double val;
};

#define BASE (1 << CHAR_BIT)
class BigInt : public Number {
	public:
		BigInt(long x);
		BigInt() = default;
		virtual explicit operator long() const;
		virtual explicit operator double() const;
		// virtual operator std::string() const;
		virtual std::string toString() const;
		virtual explicit operator BigInt() const;
		virtual explicit operator BigDecimal() const;
		virtual explicit operator Ratio() const;

		bool operator==(const BigInt &y) const {return (sign == y.sign) && cmp(y);};
		BigInt operator+(const BigInt &y) const;
		BigInt operator-() const;
		BigInt operator-(const BigInt &y) const;
		BigInt operator*(const BigInt &y) const;
		BigInt operator*(const long &y) const;
		BigInt operator/(const BigInt &y) const;
		BigInt operator%(const BigInt &y) const;
		BigInt operator>>(unsigned long y) const;
		BigInt operator>>(int y) const;
		bool operator<(const BigInt &y) const;
		bool operator<=(const BigInt &y) const;
		bool operator>(const BigInt &y) const;
		bool operator>=(const BigInt &y) const;
		BigInt abs() const;

		long signum(void) const;
		BigInt gcd(const BigInt &d) const;
		bool isZero(void) const;
		bool isNull(void) const {return array.size() == 0;};
		bool isOdd(void) const;
		BigInt pow(BigInt y) const;
		BigInt divide(const BigInt &y, BigInt *q) const;
		long divide(long y, BigInt *q) const;
		BigDecimal toBigDecimal(int sign, int scale) const;
		int bitLength();

		// These should be private with Friend BigDecimal::stripZerosToMatchScale
		int cmp(const BigInt &y) const;
		unsigned char &operator[] (size_t n) {return array[n];};
		const unsigned char &operator[] (size_t n) const {return array[n];};

		static const BigInt ZERO;
		static const BigInt ONE;
		static const BigInt TEN;
	private:
		BigInt(size_t n, bool) : sign(1), ndigits(1), array(n) {};
		bool isOne(void) const;
		bool isPos(void) const;
		static size_t maxDigits(const BigInt &x, const BigInt &y);
		void add(const BigInt &x, const BigInt &y);
		int add_loop(size_t n, const BigInt &x, int carry);
		int add_loop(size_t n, const BigInt &x, const BigInt &y, int carry);
		void sub(const BigInt &x, const BigInt &y);
		int sub_loop(size_t n, const BigInt &x, int borrow);
		int sub_loop(size_t n, const BigInt &x, const BigInt &y, int borrow);
		void div(const BigInt &y, BigInt *q, BigInt *r) const;
		int quotient(const BigInt &x, int y);
		size_t length() const;
		void normalize(void);
		static int bitLengthForInt(unsigned int);

		int sign;
		size_t ndigits;
		std::vector<unsigned char> array;
};

typedef enum {
	ROUND_UP,
	ROUND_DOWN,
	ROUND_CEILING,
	ROUND_FLOOR,
	ROUND_HALF_UP,
	ROUND_HALF_DOWN,
	ROUND_HALF_EVEN,
	ROUND_UNNECESSARY,
} roundingMode;

class BigDecimal : public Number {
	public:
		BigDecimal(double x);
		BigDecimal(BigInt x) : intVal((compactValFor(x) != INFLATED) ? BigInt() : x),
							   intCompact(compactValFor(x)) {};
		BigDecimal(BigInt x, int scale) : BigDecimal(x) {this->scale = scale;};

		static BigDecimal valueOf(long val);
		static BigDecimal valueOf(long unscaledVal, int scale);

		virtual explicit operator long() const;
		virtual explicit operator double() const;
		// virtual explicit operator std::string() const;
		virtual std::string toString(void) const;
		virtual explicit operator BigInt() const;
		virtual explicit operator BigDecimal() const;
		virtual explicit operator Ratio() const;

		bool operator==(const BigDecimal &y) const;
		bool equiv(const BigDecimal &y) const;
		BigDecimal operator+(const BigDecimal &y) const;
		BigDecimal operator-() const;
		BigDecimal operator-(const BigDecimal &y) const;
		BigDecimal operator*(const BigDecimal &y) const;
		// BigDecimal operator*(const long &y) const;
		BigDecimal operator/(const BigDecimal &y) const;
		BigDecimal operator%(const BigDecimal &y) const;
		BigDecimal divideToIntegralValue(const BigDecimal &y) const;
		BigDecimal divide(const BigDecimal &y, int mcPrecision, roundingMode rm) const;
		bool operator<(const BigDecimal &y) const;
		bool operator<=(const BigDecimal &y) const;
		bool operator>(const BigDecimal &y) const;
		bool operator>=(const BigDecimal &y) const;
		BigDecimal abs() const;

		BigInt toBigInt() const;

		int signum() const;
		int getPrecision() const;
		int getScale() const;
		int unscaledValue();

		static const BigDecimal ONE;
	private:
		BigDecimal(BigInt intVal, long val, int scale, int prec) :
			intVal(intVal), intCompact(val), scale(scale), precision(prec) { };

		BigDecimal() = default;

		BigDecimal setScale(int newscale, roundingMode rm) const;
		BigInt inflate() const;
		int checkScale(long val) const;
		BigInt bigMultiplyPowerTen(int n) const;
		BigDecimal stripZerosToMatchScale(long preferredScale);
		int compareMagnitude(const BigDecimal &val) const;
		int cmp(const BigDecimal &y) const;

		BigInt intVal;
		long intCompact;
		int scale;
		int precision;

		static long longMultiplyPowerTen(long val, int n);
		static BigInt bigTenToThe(int n);
		static BigDecimal divideAndRound(long ldividend, const BigInt bdividend, long ldivisor, const BigInt bdivisor,
			int scale, roundingMode rm, int preferredScale);
		static int longCompareMagnitude(long x, long y);
		static long compactValFor(BigInt b);
		static int longDigitLength(long x);
		static int bigDigitLength(BigInt b);
		static int saturateLong(long s);
		static BigDecimal doRound(BigDecimal &d, int mcPrecision, roundingMode rm);

		static const long INFLATED = LONG_MIN;
		static const std::vector<BigDecimal> zeroThroughTen;
		static const std::vector<BigDecimal> ZERO_SCALED_BY;

		friend BigDecimal BigInt::toBigDecimal(int sign, int scale) const;
};

class Ratio : public Number {
	public:
		Ratio(BigInt numerator, BigInt denominator) : numerator(numerator), denominator(denominator) {};
		const BigInt numerator;
		const BigInt denominator;

		virtual explicit operator long() const;
		virtual explicit operator double() const;
		// virtual explicit operator std::string() const;
		virtual std::string toString(void) const;
		virtual explicit operator BigInt() const;
		virtual explicit operator BigDecimal() const;
		virtual explicit operator Ratio() const;

		BigInt toBigInt() const;
		BigDecimal toBigDecimal() const;
		BigDecimal toBigDecimal(int precision, roundingMode rm) const;
};



#endif /* NUMBERS_HPP */
