#! /usr/bin/env python3

import sys
from fractions import Fraction

def calc(A, B, op):
    if op == '+':
        return A + B
    elif op == '-':
        return A - B
    elif op == '*':
        return A * B
    elif op == '/':
        return A / B
    else:
        assert(False)

        
num_lines = int(sys.stdin.readline())

while num_lines > 0:
    a_n, a_d, b_n, b_d, op = sys.stdin.readline().strip().split(" ")
    A = Fraction(int(a_n), int(a_d))
    B = Fraction(int(b_n), int(b_d))
    print(calc(A, B, op))
    num_lines -= 1
