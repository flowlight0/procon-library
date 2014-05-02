#include <iostream>

class Fraction{
    typedef long long ll;
    
    inline ll abs(ll v) { return v < 0 ? -v : v; }
    ll gcd(ll u, ll v)  { return v == 0 ? u : gcd(v, u % v); }
    
    void normalize(){
        if (denominator < 0){
            denominator *= -1;
            numerator *= -1;
        }
        ll d = gcd(abs(denominator), abs(numerator));
        denominator /= d;
        numerator /= d;
    }
public:
    
    ll numerator;
    ll denominator;

    Fraction() : numerator(0), denominator(1) {}
    Fraction(ll num, ll den) : numerator(num), denominator(den) { normalize(); }
    
    Fraction operator-() const {
        return Fraction(-numerator, denominator);
    }
    
    Fraction &operator+=(const Fraction &rat){
        ll new_den = denominator * rat.denominator;
        ll new_num = numerator * rat.denominator + rat.numerator * denominator;
        denominator = new_den;
        numerator = new_num;
        normalize();
        return *this;
    }

    Fraction &operator-=(const Fraction &rat){
        return (*this += -rat);
    }
    
    Fraction &operator*=(const Fraction &rat){
        ll new_den = denominator * rat.denominator;
        ll new_num = numerator * rat.numerator;
        denominator = new_den;
        numerator = new_num;
        normalize();
        return *this;
    }

    Fraction &operator/=(const Fraction &rat){
        return (*this *= Fraction(rat.denominator, rat.numerator));
    }

    bool operator==(const Fraction &rat) const {
        return this->numerator == rat.numerator && this->denominator == rat.denominator;
    }
    
    Fraction abs(){
        return Fraction(abs(numerator), denominator);
    }

    friend std::ostream &operator<<(std::ostream &out, const Fraction &rat){
        out << rat.numerator << "/" << rat.denominator;
        return out;
    }

    friend Fraction operator+(const Fraction &rat1, const Fraction &rat2){
        Fraction ans = rat1;
        return (ans += rat2);
    }

    friend Fraction operator-(const Fraction &rat1, const Fraction &rat2){
        Fraction ans = rat1;
        return (ans -= rat2);
    }

    friend Fraction operator*(const Fraction &rat1, const Fraction &rat2){
        Fraction ans = rat1;
        return (ans *= rat2);
    }

    friend Fraction operator/(const Fraction &rat1, const Fraction &rat2){
        Fraction ans = rat1;
        return (ans /= rat2);
    }
};


