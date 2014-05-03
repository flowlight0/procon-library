#ifndef GUARD_FINITE_FIELD
#define GUARD_FINITE_FIELD

#include <iostream>
#include <cassert>

template <int modulo> class GaloisField;


template <int modulo> class GaloisField{
  int value;
  typedef GaloisField<modulo> GF;
public:
  
  GaloisField(long long value);
  GaloisField();
  GaloisField(const GaloisField<modulo> &rhs);

  GaloisField<modulo> &operator+=(const GaloisField<modulo> &rhs);
  GaloisField<modulo> &operator-=(const GaloisField<modulo> &rhs);
  GaloisField<modulo> &operator*=(const GaloisField<modulo> &rhs);
  GaloisField<modulo> &operator/=(const GaloisField<modulo> &rhs);
  const GaloisField<modulo> operator-() const;
  const GaloisField<modulo> inverse  () const;
  inline int get() const { return value; };

  template <int mod> friend const GaloisField<mod>
  operator+(const GaloisField<mod>&, const GaloisField<mod>&);
  template <int mod> friend const GaloisField<mod>
  operator-(const GaloisField<mod>&, const GaloisField<mod>&);
  template <int mod> friend const GaloisField<mod>
  operator*(const GaloisField<mod>&, const GaloisField<mod>&);
  template <int mod> friend const GaloisField<mod>
  operator/(const GaloisField<mod>&, const GaloisField<mod>&);
  template <int mod> friend bool
  operator==(const GaloisField<mod>&, const GaloisField<mod>&);
  
private:
  void extgcd(int a, int b, int &x, int &y) const ; 
};

template <int modulo> GaloisField<modulo>::
GaloisField(long long _value) : value(_value % modulo){
  if (value < 0){
    value += modulo;
  }
}

template <int modulo> GaloisField<modulo>::
GaloisField() : value(0){}

template <int modulo> GaloisField<modulo>::
GaloisField(const GF &rhs) : value(rhs.get()){}

template <int modulo> GaloisField<modulo> &
GaloisField<modulo>::operator+=(const GF &rhs){
  this->value += rhs.get();
  if (this->value >= modulo)  this->value -= modulo;
  return *this;
}

template <int modulo> GaloisField<modulo> &
GaloisField<modulo>::operator-=(const GF &rhs){
  this->value -= rhs.get();
  if (this->value < 0)  this->value += modulo;
  return *this;
}

template <int modulo> GaloisField<modulo> &
GaloisField<modulo>::operator*=(const GF &rhs){
  this->value = (long long)this->value * rhs.get() % modulo;
  return *this;
}

template <int modulo> GaloisField<modulo> &
GaloisField<modulo>::operator/=(const GF &rhs){
  this->value = (long long)this->value * rhs.inverse().get() % modulo;
  return *this;
}

template <int modulo> const GaloisField<modulo>
GaloisField<modulo>::operator-() const {
  return GF(modulo - 1 - this->get());
}

template <int modulo> const GaloisField<modulo>
GaloisField<modulo>::inverse() const {
  if (this->value == 0){
    std::cerr << "There is no inverse of 0." << std::endl;
    assert(false);
  }
  int x, y;
  extgcd(this->value, modulo, x, y);
  return GF(x);
};




template <int modulo> const GaloisField<modulo>
operator+(const GaloisField<modulo> &f1, const GaloisField<modulo> &f2){
  GaloisField<modulo> f(f1);
  return f += f2;
}

template <int modulo> const GaloisField<modulo>
operator-(const GaloisField<modulo> &f1, const GaloisField<modulo> &f2){
  GaloisField<modulo> f(f1);
  return f -= f2;
}

template <int modulo> const GaloisField<modulo>
operator*(const GaloisField<modulo> &f1, const GaloisField<modulo> &f2){
  GaloisField<modulo> f(f1);
  return f *= f2;
}

template <int modulo> const GaloisField<modulo>
operator/(const GaloisField<modulo> &f1, const GaloisField<modulo> &f2){
  GaloisField<modulo> f(f1);
  return f /= f2;
}

template <int modulo> bool
operator==(const GaloisField<modulo> &f1, const GaloisField<modulo> &f2){
  return f1.get() == f2.get();
}

template <int modulo> void
GaloisField<modulo>::extgcd(int a, int b, int &x, int &y) const {
  // compute pair of x, y such that a * x + b * y == 1 (mod modulo)
  if (b == 0){
    x = 1;
    y = 0;
  } else {
    extgcd(b, a % b, y, x);
    y -= (a / b) * x;
  }
}

#endif

