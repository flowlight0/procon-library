#include <gtest/gtest.h>
#include <string>
#include <cstdlib>
#include "suffix_array.hpp"
using namespace std;

vector<int> random_array(size_t n, const vector<int> &alphabet){
    vector<int> res;
    for (size_t i = 0; i < n; i++){
        res.push_back(alphabet[rand() % alphabet.size()]);
    }
    return res;
}

void SufCheck(SuffixArray sa, const vector<int> &array){
    for (size_t i = 0; i < array.size(); i++){                  
        for (size_t j = 0; j < i; j++){                         
            if (sa[i] < sa[j]){                                 
                vector<int> ai(array.begin() + i, array.end()); 
                vector<int> bi(array.begin() + i, array.end()); 
                ASSERT_TRUE(ai <= bi);                          
            }                                                   
        }                                                       
    }                                                           
}


void LCPCheck(SuffixArray sa, const vector<int> &array){
    for (size_t i = 0; i + 1 < array.size(); i++){
        int c = 0;
        size_t a = sa[i];
        size_t b = sa[i + 1];
        while (a + c < array.size() && b + c < array.size()){
            if (array[a + c] == array[b + c]) c++;
            else break;
        }
        ASSERT_EQ(sa.get_height(i), c);
    }
    ASSERT_EQ(sa.get_height(array.size() - 1), 0);
} 

TEST(SUFARY_TEST, SMALL){
    string str = "abracadabra";
    vector<int> array;
    for (char c: str) array.push_back(c);
    SuffixArray sa(array);
    SufCheck(sa, array);
}

TEST(SUFARY_TEST, MIDDLE0){
    vector<int> array = random_array(100, {1, 2});
    SuffixArray sa(array);
    SufCheck(sa, array);
}

TEST(SUFARY_TEST, MIDDLE1){
    vector<int> alpha;
    for (int i  = 0; i < 26; i++) alpha.push_back(i);
    vector<int> array = random_array(500, alpha);
    SuffixArray sa(array);
    SufCheck(sa, array);
}

TEST(LCP_TEST, SMALL){
    string str = "abracadabra";
    vector<int> array;
    for (char c: str) array.push_back(c);
    SuffixArray sa(array);
    for (size_t i = 0; i < array.size(); i++) ASSERT_EQ(i, sa.get_rank(sa[i]));
    LCPCheck(sa, array);
}

TEST(LCP_TEST, MIDDLE0){
    vector<int> array = random_array(100000, {1, 2});
    SuffixArray sa(array);
    LCPCheck(sa, array);
}

TEST(LCP_TEST, MIDDLE1){
    vector<int> alpha;
    for (int i  = 0; i < 26; i++) alpha.push_back(i);
    vector<int> array = random_array(100000, alpha);
    SuffixArray sa(array);
    LCPCheck(sa, array);
}



int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
