#include <gtest/gtest.h>
#include <string>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include "Fraction.hpp"
using namespace std;

typedef long long ll;

vector<Fraction> python_calc(const vector<Fraction> &as,
                             const vector<Fraction> &bs,
                             const vector<char>     &ops){

  vector<Fraction> res;
  int   pipefd[2][2];
  int   backup;
  pid_t pid;
  ll    numer;
  ll    denom;
  
  if (as.size() != bs.size() || as.size() != ops.size()){
    return res;
  }

  for (int i = 0; i < 2; i++){
    if (pipe(pipefd[i]) == -1){
      return res;
    }
  }
  
  if ((pid = fork()) == -1){
    return res;
  } else if (pid == 0){
    FILE* tmp_out_file = fdopen(pipefd[0][1], "w");
    
    if (tmp_out_file == NULL){
      exit(EXIT_FAILURE);
    }

    fprintf(tmp_out_file, "%d\n", (int)as.size());
    for (size_t i = 0; i < as.size(); i++){
      fprintf(tmp_out_file, "%lld %lld %lld %lld %c\n",
              as[i].numerator,
              as[i].denominator,
              bs[i].numerator,
              bs[i].denominator,
              ops[i]);
    }
    fflush(tmp_out_file);
    fclose(tmp_out_file);
    
    dup2(pipefd[0][0], 0);
    dup2(pipefd[1][1], 1);
    close(pipefd[0][0]);
    close(pipefd[1][0]);
    
    char *argv[] = {NULL};
    if (execv("./fraction-calc.py", argv) == -1){
      exit(EXIT_FAILURE);
    };
    
  } else {
    backup = dup(0);
    dup2(pipefd[1][0], 0);
    close(pipefd[1][1]);
  
    while (scanf("%lld/%lld", &numer, &denom) != EOF){
      res.push_back(Fraction(numer, denom));
    }
    dup2(0, backup);
    close(backup);
    
    if (wait(NULL) < 0){
      return vector<Fraction>();
    }
  }
  return res;
}

TEST(ADD_TEST, SMALL){
  ASSERT_EQ(Fraction(3, 2) + Fraction(1, 2), Fraction(2, 1));
  ASSERT_EQ(Fraction(4, 5) + Fraction(1, 3), Fraction(17, 15));
  ASSERT_EQ(Fraction(3, 5) + Fraction(-2, 2), Fraction(-2, 5));
}


TEST(SUB_TEST, SMALL){
  ASSERT_EQ(Fraction(3, 2) - Fraction(1, 2), Fraction(1, 1));
  ASSERT_EQ(Fraction(4, 5) - Fraction(1, 3), Fraction(7, 15));
  ASSERT_EQ(Fraction(3, 5) - Fraction(-2, 2), Fraction(8, 5));
}

TEST(MUL_TEST, SMALL){
  ASSERT_EQ(Fraction(3, 2) * Fraction(1, 2), Fraction(3, 4));
  ASSERT_EQ(Fraction(4, 5) * Fraction(1, 3), Fraction(4, 15));
  ASSERT_EQ(Fraction(3, 5) * Fraction(-2, 2), Fraction(3, -5));
}

TEST(DIV_TEST, SMALL){
  ASSERT_EQ(Fraction(3, 2) / Fraction(1, 2), Fraction(3, 1));
  ASSERT_EQ(Fraction(4, 5) / Fraction(1, 3), Fraction(12, 5));
  ASSERT_EQ(Fraction(3, 5) / Fraction(-2, 2), Fraction(-3, 5));
}

TEST(RANDOM_TEST, RANDOM){
  string op = "+-/*";
  vector<Fraction> as;
  vector<Fraction> bs;
  vector<char>     ops;
  const int Q = 1000;
  for (int i = 0; i < Q; i++){
    as.push_back(Fraction(rand(), rand()));
    bs.push_back(Fraction(rand(), rand()));
    ops.push_back(op[rand() % 4]);
  }
  vector<Fraction> ans = python_calc(as, bs, ops);
  ASSERT_FALSE(ans.empty());
  for (int i = 0; i < Q; i++){
    Fraction c = Fraction(1, 1);
    switch(ops[i]){
    case '+':
      c = as[i] + bs[i];
      break;
    case '-':
      c = as[i] - bs[i];
      break;
    case '*':
      c = as[i] * bs[i];
      break;
    case '/':
      c = as[i] / bs[i];
      break;
    }
    ASSERT_EQ(c, ans[i]);
  }
}


int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
