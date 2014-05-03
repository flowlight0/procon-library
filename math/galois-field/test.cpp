#include <unistd.h>
#include <gtest/gtest.h>
#include <string>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include "galois_field.hpp"
using namespace std;

typedef long long ll;



template <int modulo> void do_test(char op, const int test_cases){
  for (int q = 0; q < test_cases; q++){
    int s = rand() % RAND_MAX;
    int t = rand() % RAND_MAX;
    GaloisField<modulo> A = GaloisField<modulo>(s);
    GaloisField<modulo> B = GaloisField<modulo>(t);
    switch (op){
    case '+':
      ASSERT_EQ((A + B).get(), GaloisField<modulo>((long long)s + t).get());
      break;
    case '-':
      ASSERT_EQ((A - B).get(), GaloisField<modulo>((long long)s - t).get());
      break;
    case '*':
      ASSERT_EQ((A * B).get(), GaloisField<modulo>((long long)s * t).get());
      break;
    case '/':
      if (B.get() > 0){
        ASSERT_EQ((A / B * B).get(), A.get());
      }
      break;
    case '~':
      if (A.get() > 0){
        ASSERT_EQ((A * A.inverse()).get(), GaloisField<modulo>(1).get());
      }
      break;
    } 
  }
}

TEST(ADD_TEST, RANDOM){
  do_test<1000000007> ('+', 100000);
  do_test<1000000009> ('+', 100000);
  do_test<2>          ('+', 100000);
  do_test<7>          ('+', 100000);
}

TEST(SUB_TEST, RANDOM){
  do_test<1000000007> ('-', 100000);
  do_test<1000000009> ('-', 100000);
  do_test<2>          ('-', 100000);
  do_test<7>          ('-', 100000);
}

TEST(MUL_TEST, RANDOM){
  do_test<1000000007> ('*', 100000);
  do_test<1000000009> ('*', 100000);
  do_test<2>          ('*', 100000);
  do_test<7>          ('*', 100000);
}

TEST(INV_TEST, RANDOM){
  do_test<1000000007> ('~', 100000);
  do_test<1000000009> ('~', 100000);
  do_test<2>          ('~', 100000);
  do_test<7>          ('~', 100000);
}

TEST(DIV_TEST, RANDOM){
  do_test<1000000007> ('/', 100000);
  do_test<1000000009> ('/', 100000);
  do_test<2>          ('/', 100000);
  do_test<7>          ('/', 100000);
}


int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
