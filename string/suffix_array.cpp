/*******************************************************

vector<int> vecに対してO(N)でSuffix Arrayの構築
Suffix ArrayからのLCPのO(N)構築

ただしvecの各要素はmax(vec.size(), 256)以下の非負整数

コードは元論文のものから写経
Juha Karkkainen and Peter Sanders "Simple Linear Work Suffix Array Construction"

verified at
http://www.spoj.com/problems/SARRAY/ (構築)
http://www.spoj.com/problems/SUBST1/ (LCP)

*******************************************************/

#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include <cassert>


class SuffixArray{
  typedef std::vector<int> Array;
  
  size_t vec_size;
  Array  org_vec;
  Array  suf_arr;
  Array  lcp_arr;
  Array  rank;
  
  inline bool leq(int a1, int a2, int b1, int b2){
    return a1 != b1 ? a1 < b1 : a2 <= b2;
  }
  
  inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3){
    return a1 != b1 ? a1 < b1 : leq(a2, a3, b2, b3);
  }

  void radix_pass(Array &a, Array &b, const Array &s,
                  int offset, int n, int alpha_size)
  {
    const int least_alphabet_size = 256;
    alpha_size = std::max(alpha_size, least_alphabet_size);
    
    Array count(alpha_size + 1, 0);

    for(int i = 0; i < n; i++){
      count[s[a[i]+offset]]++;
    }
    
    for(int i = 0, sum = 0;  i <= alpha_size; i++){
      int tmp  = count[i];
      count[i] = sum;
      sum += tmp;
    }
    
    for(int i = 0; i < n; i++){
      b[count[s[a[i]+offset]]++] = a[i];
    }
  }
  
  void build_sa(const Array &S, Array &suf, int n){
    int n0  = (n + 2) / 3;
    int n1  = (n + 1) / 3;
    int n2  = n / 3;
    int n02 = n0 + n2;
    Array S12  (n02 + 3), S0  (n0);
    Array suf12(n02 + 3), suf0(n0);
    
    for(int i = 0; i < 3; i++){
      S12[n02 + i] = suf12[n02 + i] = 0;
    }
    
    for(int i = 0, j = 0; i < n + (n0 - n1); i++){
      if(i % 3 != 0) S12[j++] = i;
    }
    
    // radix sort the mod 1 and 2 triples
    radix_pass(S12, suf12, S, 2, n02, S.size());
    radix_pass(suf12, S12, S, 1, n02, S.size());
    radix_pass(S12, suf12, S, 0, n02, S.size());
    
    // find lexicographic names of triples
    int name = 0;
    int c0 = -1, c1 = -1, c2 = -1;
    for(int i = 0; i < n02; i++){
      if(S[suf12[i]] != c0 || S[suf12[i]+1] != c1 || S[suf12[i]+2] != c2){
        name++;
        c0 = S[suf12[i]  ];
        c1 = S[suf12[i]+1];
        c2 = S[suf12[i]+2];
      }
      int offset = suf12[i] % 3 == 1 ? 0 : n0;
      S12[suf12[i]/3+offset] = name;
    }
    
    if(name < n02){
      build_sa(S12, suf12, n02);
      for(int i = 0; i < n02; i++) S12[suf12[i]] = i + 1;
    }else{
      for(int i = 0; i < n02; i++) suf12[S12[i] - 1] = i;
    }
    
    // sort the mod 0 suffixes by their first character.
    for(int i = 0, j = 0; i < n02; i++){
      if(suf12[i] < n0) S0[j++] = 3 * suf12[i];
    }
    
    radix_pass(S0, suf0, S, 0, n0, S.size());
    
    int t = n0 - n1, p = 0, k = 0;
    while(t < n02 && p < n0){
#define GetI() (suf12[t]<n0 ? suf12[t]*3+1 : (suf12[t]-n0)*3+2)
      
      int i        = GetI();
      int j        = suf0[p];
      bool small12 = (suf12[t] < n0) ? 
        leq(S[i], S12[suf12[t]+n0], S[j], S12[j/3]) :
        leq(S[i], S[i+1], S12[suf12[t]-n0+1], S[j], S[j+1], S12[j/3+n0]);
      
      if(small12){
        suf[k] = i; t++;
      }else{
        suf[k] = j; p++;
      }
      k++;
    }
    while(t < n02){ suf[k++] = GetI(); t++;}
    while(p < n0 ){ suf[k++] = suf0[p++]; }
  }

  void build_lcp(){

    for(size_t i = 0; i < vec_size; i++){
      rank[suf_arr[i]] = i;
    }
    
    int h = 0;
    
    for(size_t i = 0; i < vec_size; i++){
      size_t j = vec_size;
      
      if((size_t)rank[i] + 1 < vec_size){
        j = suf_arr[rank[i] + 1];
      }
      
      if(h > 0) h--;
      
      for(;j + h < vec_size && i + h < vec_size; h++){
        if(org_vec[j + h] != org_vec[i + h]) break;
      }
      lcp_arr[rank[i]] = h;
    }
  }
  
  
public:
  SuffixArray(const Array &vec){ construct(vec);}
  SuffixArray() {}
  
  void construct(const Array &vec_){
    vec_size = vec_.size();
    org_vec  = vec_;
    
    for(int i = 0; i < 3; i++){
      org_vec.push_back(0);
    }
    
    suf_arr.resize(org_vec.size());
    lcp_arr.resize(org_vec.size());
    rank.resize(org_vec.size());
    
    build_sa (org_vec, suf_arr, vec_size);
    build_lcp();
  }
  
  int operator[](size_t pos) const { return suf_arr[pos]; }
  int height    (size_t pos) const { return lcp_arr[pos]; }
  // int rank(size_t pos) const { return rank[pos + 1]; }
  size_t size() const { return vec_size; }
};


// #define SARRAY
#ifndef SARRAY

#define STRLCP

#endif


#include <cstdio>
#include <cstring>
using namespace std;
 
#ifdef SARRAY
      
// solution for http://www.spoj.com/problems/SARRAY/

int main(){
  char buf[100010];
  scanf("%s", buf);
  string str(buf, buf + strlen(buf));
  
  vector<int> vec(str.size());
  for(size_t i = 0; i < str.size(); i++) vec[i] = str[i];
  SuffixArray sa(vec);
  
  for(size_t i = 0; i < vec.size(); i++){
    printf("%d\n", sa[i]);
  }
  return 0;
}

#endif

#ifdef STRLCP
 
int main(){
  int T;
  char buf[100010];
  
  scanf("%d", &T);
  while(T--){
    scanf("%s", buf);
    
    int n = strlen(buf);
    long long res = (long long)n * (n + 1) / 2;
    
    vector<int> str(buf, buf + n);
    SuffixArray sa(str);
    
    for(int i = 0; i < n; i++) res -= sa.height(i);

    printf("%lld\n", res);
  }
}
 
#endif
