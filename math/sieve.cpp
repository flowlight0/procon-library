/***********************************************************
エストラテネスの篩
O(n log n)よりははやい.
 ***********************************************************/

#include <vector>

std::vector<bool> prime_table(int N){
    std::vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= N; i++){
        if (is_prime[i]){
            for (int j = i * 2; j <= N; j += i) is_prime[j] = false;
        }
    }
    return is_prime;
}

std::vector<int> compute_primes(int N){
    std::vector<bool> is_prime = prime_table(N);
    std::vector<int> primes;
    for (int i = 0; i < N + 1; i++) if (is_prime[i]) primes.push_back(i);
    return primes;
}


// solution for http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0009&lang=jp

#include <vector>
#include <iostream>

using namespace std;

int main(){
    int n;
    while(cin >> n){
        vector<int> primes = compute_primes(n);
        cout << primes.size() << endl;
    }
    return 0;
}
