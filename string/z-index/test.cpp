#include <gtest/gtest.h>
#include <string>
#include <cstdlib>
#include "z.hpp"
using namespace std;

string random_array(size_t n, const string &alphabet){
    string res;
    for (size_t i = 0; i < n; i++){
        res.push_back(alphabet[rand() % alphabet.size()]);
    }
    return res;
}

void Check(const string &str){
    ZIndex z(str);
    for (size_t i = 0; i < str.size(); i++){
        int c = 0;
        while (i < str.size() && i + c < str.size() && str[c] == str[i + c]) c++;
        ASSERT_EQ(z[i], c);
    }
}

TEST(LCP_TEST, SMALL){
    string str = "abracadabra";
    Check(str);
}

TEST(LCP_TEST, MIDDLE0){
    string str = random_array(100000, "12");
    Check(str);
}

TEST(LCP_TEST, MIDDLE1){
    string alpha;
    for (int i  = 0; i < 26; i++) alpha.push_back('a' + i);
    string str = random_array(100000, alpha);
    Check(str);
}


int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
