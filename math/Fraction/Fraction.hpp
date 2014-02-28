#include <iostream>

class Fraction{
    typedef long long ll;
    ll numer;
    ll denom;
    
    inline ll abs(ll v) { return v < 0 ? -v : v; }
    ll gcd(ll u, ll v)  { return v == 0 ? u : gcd(v, u % v); }
    
    void normalize(){
        if (denom < 0){
            denom *= -1;
            numer *= -1;
        }
        ll d = gcd(abs(denom), abs(numer));
        denom /= d;
        numer /= d;
    }
public:
    Fraction() : numer(0), denom(1) {}
    Fraction(ll num, ll den) : numer(num), denom(den) { normalize(); }
    
    Fraction operator-() const {
        return Fraction(-numer, denom);
    }
    
    Fraction &operator+=(const Fraction &rat){
        ll new_den = denom * rat.denom;
        ll new_num = numer * rat.denom + rat.numer * denom;
        denom = new_den;
        numer = new_num;
        normalize();
        return *this;
    }

    Fraction &operator-=(const Fraction &rat){
        return (*this += -rat);
    }
    
    Fraction &operator*=(const Fraction &rat){
        ll new_den = denom * rat.denom;
        ll new_num = numer * rat.numer;
        denom = new_den;
        numer = new_num;
        normalize();
        return *this;
    }

    Fraction &operator/=(const Fraction &rat){
        return (*this *= Fraction(rat.denom, rat.numer));
    }

    bool operator==(const Fraction &rat) const {
        return this->numer == rat.numer && this->denom == rat.denom;
    }
    
    Fraction abs(){
        return Fraction(abs(numer), denom);
    }

    friend std::ostream &operator<<(std::ostream &out, const Fraction &rat){
        out << rat.numer << "/" << rat.denom;
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


