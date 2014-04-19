#include <unistd.h>
#include <gtest/gtest.h>
#include <string>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include "Fraction.hpp"
using namespace std;

typedef long long ll;

TEST(ADD_TEST, SMALL){
    ASSERT_EQ(Fraction(3, 2) + Fraction(1, 2), Fraction(2, 1));
    ASSERT_EQ(Fraction(4, 5) + Fraction(1, 3), Fraction(17, 15));
    ASSERT_EQ(Fraction(3, 5) + Fraction(-2, 2), Fraction(-2, 5));
}

Fraction python_calc(Fraction A, Fraction B, char op){
    int pid;
    int backup;
    int pipe_fds[2];
    int an = A.numerator, ad = A.denominator;
    int bn = B.numerator, bd = B.denominator;
    string s;
    
    if (pipe(pipe_fds) == -1){
        perror("pipe");
        goto ERROR;
    }
    
    if ((pid = fork()) == -1){
        perror("fork()");
        goto ERROR;
    } else if (pid == 0){
        dup2 (pipe_fds[1], 1);
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        ostringstream out;
        out <<
            "from fractions import Fraction \n"
            "A = Fraction(" << an << ", " << ad << ") \n"
            "B = Fraction(" << bn << ", " << bd << ") \n"
            "print(A " << op << "B)" << endl;
        char *argv[4] = {
            (char*)"/usr/bin/python3",
            (char*)"-c",
            (char*)out.str().c_str(),
            NULL
        };
        if (execv("/usr/bin/python3", argv) == -1){
            perror("execve");
            goto ERROR;
        }
    }

    backup = dup(0);
    dup2(pipe_fds[0], 0);
    close(pipe_fds[0]);
    close(pipe_fds[1]);
    
    cin >> s;
    {
        dup2(backup, 0);
        close(backup);
        int status;
        pid = waitpid(pid, &status, 0);
        if (pid < 0){
            perror("waitpid");
            goto ERROR;
        }
    }
    goto SUCCESS;
 ERROR:
    return Fraction(1, 0);
 SUCCESS:
    ll  cn, cd;
    for (size_t i = 0; i < s.size(); i++) if (s[i] == '/') s[i] = ' ';
    istringstream iss(s);
    iss >> cn >> cd;
    return Fraction(cn, cd);
}

TEST(ADD_TEST, RANDOM){
    const int Q = 10;
    for (int i = 0; i < Q; i++){
        Fraction A(rand(), rand());
        Fraction B(rand(), rand());
        ASSERT_EQ(A + B, python_calc(A, B, '+'));
    }
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

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
