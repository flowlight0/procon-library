/*******************************************************

vector<int> vecに対してO(N)でSuffix Arrayの構築
Suffix ArrayからのLCPのO(N)構築

ただしvecの各要素はmax(vec.size(), 256)以下の非負整数

コードは元論文のものから写経
Juha Karkkainen and Peter Sanders "Simple Linear Work Suffix Array Construction"

verified at
http://www.spoj.com/problems/SARRAY/

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

  void build(const Array &S, Array &suf, int n){
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
      build(S12, suf12, n02);
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

  
public:
  SuffixArray(const Array &vec){ construct(vec);}
  SuffixArray() {}
  
  void construct(const Array &vec_){
    vec_size = vec_.size();
    org_vec  = vec_;
    
    for(int i = 0; i < 3; i++){
      org_vec.push_back(0);
    }
    
    suf_arr = Array(org_vec.size());
    build(org_vec, suf_arr, vec_size);
  }

  int operator[](size_t pos) const {return suf_arr[pos]; }
};




// solution for http://www.spoj.com/problems/SARRAY/
#include <cstdio>
#include <cstring>
using namespace std;

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
  
}

