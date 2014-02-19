/***********************************************************
 Z algorithm
 与えられた文字列Sの各0 <= i < |S|に対して
 S[i...i+k) == S[0...k)となる最大のkをO(|S|)求める。

 verified at
 http://jag2013summer-day3.contest.atcoder.jp/tasks/icpc2013summer_day3_h
 
 参考文献
 http://codeforces.com/blog/entry/3107
***********************************************************/

#ifndef GUARD_Z_INDEX
#define GUARD_Z_INDEX

#include <vector>
#include <string>

class ZIndex{
    std::vector<int> z;
public:
    ZIndex(const std::string &S) : z(S.size()){
        int L = 0, R = 0;
        int n = S.size();
        for(int i = 1; i < n; i++){
            if(i > R){
                L = R= i;
                while(R < n && S[R-L] == S[R]) R++;
                z[i] = R - L; R--;
            }else if(z[i - L] < R - i + 1){
                z[i] = z[i - L];
            }else{
                L = i;
                while(R < n && S[R-L] == S[R]) R++;
                z[i] = R - L; R--;
            }
        }
        z[0] = S.size();
    }
    int operator[](int pos) const { return z[pos]; }
};

#endif
