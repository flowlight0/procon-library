#include <gtest/gtest.h>
#include <string>
#include <cstdlib>
#include <vector>
#include "segment_tree.hpp"
using namespace std;


typedef long long ll;

vector<ll> random_array(size_t n){
    vector<ll> array;
    for (size_t i = 0; i < n; i++)
        array.push_back(rand() - rand());
    return array;
}

TEST(ADD_SUM_TEST, RANDOM){
    const int N = 10000;
    const int Q = 100000;
    vector<ll> A = random_array(N);
    SegTree<ll> T(A);
    
    for (int i = 0; i < Q; i++){
        int l = rand() % N;
        int r = rand() % N + 1;
        if (l > r) swap(l, r);
        
        if (rand() % 2){        // ADD
            int v = rand() - rand();
            for (int j = l; j < r; j++) A[j] += v;
            T.Add(l, r, v);
        } else {                // SUM
            ll sum = 0;
            for (int j = l; j < r; j++) sum += A[j];
            ASSERT_EQ(sum, T.Sum(l, r));
        }
    }
}

TEST(ADD_MIN_TEST, RANDOM){
    const int N = 10000;
    const int Q = 100000;
    vector<ll> A = random_array(N);
    SegTree<ll> T(A);

    for (int i = 0; i < Q; i++){
        int l = rand() % N;
        int r = rand() % N + 1;
        if (l > r) swap(l, r);
        
        if (rand() % 2){        // ADD
            int v = rand() - rand();
            for (int j = l; j < r; j++) A[j] += v;
            T.Add(l, r, v);
            // for (int j = l; j < r; j++) ASSERT_EQ(T.Sum(j, j + 1), A[j]);
        } else if (l != r){                // SUM
            ll  m = 1e12;
            int pos;
            for (int j = l; j < r; j++) m = min(m, A[j]);
            ASSERT_EQ(m, T.Min(l, r, pos));
            ASSERT_EQ(m, A[pos]);
        }
    }
}

TEST(ADD_SUM_MIN_TEST, RANDOM){
    const int N = 10000;
    const int Q = 100000;
    vector<ll> A = random_array(N);
    SegTree<ll> T(A);

    for (int i = 0; i < Q; i++){
        int l = rand() % N;
        int r = rand() % N + 1;
        int t = rand() % 3;
        
        if (l > r) swap(l, r);
        
        if (t == 0){        // ADD
            int v = rand() - rand();
            for (int j = l; j < r; j++) A[j] += v;
            T.Add(l, r, v);
            // for (int j = l; j < r; j++) ASSERT_EQ(T.Sum(j, j + 1), A[j]);
        } else if (t == 1){
            ll sum = 0;
            for (int j = l; j < r; j++) sum += A[j];
            ASSERT_EQ(sum, T.Sum(l, r));
        } else if (l != r){                
            ll  m = 1e12;
            int pos;
            for (int j = l; j < r; j++) m = min(m, A[j]);
            ASSERT_EQ(m, T.Min(l, r, pos));
            ASSERT_EQ(m, A[pos]);
        }
    }
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
