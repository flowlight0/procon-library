/***********************************************************
愚直な素因数分解
O(√N)
 ***********************************************************/

#include <vector>

std::vector<long long> compute_factors(long long n){
    std::vector<long long> factors;
    for (long long i = 2; i * i <= n; i++){
        while(n % i == 0){
            factors.push_back(i);
            n /= i;
        }
    }
    if (n > 1) factors.push_back(n);
    return factors;
}

// solution for http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_A

#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;

int main(){
    ll n;
    while(cin >> n){
        vector<ll> factors = compute_factors(n);
        cout << n << ":";
        for (size_t i = 0; i < factors.size(); i++) cout << " " << factors[i];
        cout <<endl;
    }
}
