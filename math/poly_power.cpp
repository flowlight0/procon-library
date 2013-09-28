// m項間多項式のn項目をO(m^2 log(n))で計算
//
// a_m = coef[0] * a_0 + ... + coef[m-1] * a_{m-1} + coef[m] * d
// となる配列に対してpoly_powは
// a_n = ret[0] * a_0 + ... + ret[m-1] * a_{m-1} + ret[m] * d
// となる配列をかえす


#include <cassert>
#include <vector>

typedef long long ll;

std::vector<ll> poly_mul(const std::vector<ll> &x,
                         const std::vector<ll> &y,
                         const std::vector<ll> &coef, ll mod)
{
  assert(x.size() == y.size());
  
  int n = x.size() - 1;
  std::vector<ll> ret(n + 1, 0);
  std::vector<ll> q  (2 * n, 0);
  
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      (q[i+j] += x[i] * y[j]) %= mod;
    }
  }
  
  for(int i = 2 * n - 1; i >= n; i--){
    for(int j = 0; j < n; j++){
      (q[i-n+j] += q[i] * coef[j]) %= mod;
    }
  }

  ret[n] = x[n];
  
  for(int i = 0; i < n; i++){
    ret[i] = q[i];
    (ret[n] += y[n] * x[i]) %= mod;
  }
  
  return ret;
}

std::vector<ll> poly_pow(const std::vector<ll> &coef, ll N, ll mod){
  std::vector<ll> p(coef.size(), 0); 
  std::vector<ll> c(coef.size(), 0);

  c[0] = 1;
  p[1] = 1;
  
  while(N){
    if(N & 1) c = poly_mul(c, p, coef, mod);
    p = poly_mul(p, p, coef, mod);
    N >>= 1;
  }
  return c;
}

const ll mod = 1000 * 1000 * 1000 + 7;
#include <iostream>
#include <numeric>

int main(){
  int K, N;
  std::cin >> K >> N;

  std::vector<ll> coef(K + 1, 1); coef[K] = 0;
  coef = poly_pow(coef, N - 1, mod);

  std::cout << accumulate(coef.begin(), coef.end(), 0LL) % mod<< std::endl;
  
  return 0;
}
