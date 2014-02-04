#include <iostream>

class Rational{
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
    Rational() : numer(0), denom(1) {}
    Rational(ll num, ll den) : numer(num), denom(den) { normalize(); }
    
    Rational operator-() const {
        return Rational(-numer, denom);
    }
    
    Rational &operator+=(const Rational &rat){
        ll new_den = denom * rat.denom;
        ll new_num = numer * rat.denom + rat.numer * denom;
        denom = new_den;
        numer = new_num;
        normalize();
        return *this;
    }

    Rational &operator-=(const Rational &rat){
        return (*this += -rat);
    }
    
    Rational &operator*=(const Rational &rat){
        ll new_den = denom * rat.denom;
        ll new_num = numer * rat.numer;
        denom = new_den;
        numer = new_num;
        normalize();
        return *this;
    }

    Rational &operator/=(const Rational &rat){
        return (*this *= Rational(rat.denom, rat.numer));
    }

    bool operator==(const Rational &rat) const {
        return this->numer == rat.numer && this->denom == rat.denom;
    }
    
    Rational abs(){
        return Rational(abs(numer), denom);
    }

    friend std::ostream &operator<<(std::ostream &out, const Rational &rat){
        out << rat.numer << "/" << rat.denom;
        return out;
    }

    friend Rational operator+(const Rational &rat1, const Rational &rat2){
        Rational ans = rat1;
        return (ans += rat2);
    }

    friend Rational operator-(const Rational &rat1, const Rational &rat2){
        Rational ans = rat1;
        return (ans -= rat2);
    }

    friend Rational operator*(const Rational &rat1, const Rational &rat2){
        Rational ans = rat1;
        return (ans *= rat2);
    }

    friend Rational operator/(const Rational &rat1, const Rational &rat2){
        Rational ans = rat1;
        return (ans /= rat2);
    }
    
};


using namespace std;

#include <gtest/gtest.h>


TEST(RatinalTest, ADD){
    ASSERT_EQ(Rational(3, 2) + Rational(1, 2), Rational(2, 1));
    ASSERT_EQ(Rational(4, 5) + Rational(1, 3), Rational(17, 15));
    ASSERT_EQ(Rational(3, 5) + Rational(-2, 2), Rational(-2, 5));
}

TEST(RatinalTest, SUB){
    ASSERT_EQ(Rational(3, 2) - Rational(1, 2), Rational(1, 1));
    ASSERT_EQ(Rational(4, 5) - Rational(1, 3), Rational(7, 15));
    ASSERT_EQ(Rational(3, 5) - Rational(-2, 2), Rational(8, 5));
}

TEST(RatinalTest, MUL){
    ASSERT_EQ(Rational(3, 2) * Rational(1, 2), Rational(3, 4));
    ASSERT_EQ(Rational(4, 5) * Rational(1, 3), Rational(4, 15));
    ASSERT_EQ(Rational(3, 5) * Rational(-2, 2), Rational(3, -5));
}

TEST(RatinalTest, DIV){
    ASSERT_EQ(Rational(3, 2) / Rational(1, 2), Rational(3, 1));
    ASSERT_EQ(Rational(4, 5) / Rational(1, 3), Rational(12, 5));
    ASSERT_EQ(Rational(3, 5) / Rational(-2, 2), Rational(-3, 5));
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
