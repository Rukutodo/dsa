// Problem: Count Digits
// Given an integer n, count how many digits it has.
// Approach: repeatedly divide by 10 until it becomes 0, counting each division.
// Time: O(log10(n))  Space: O(1)

#include <bits/stdc++.h>
using namespace std;

int countDigits(int n) {
    n = abs(n);
    if (n == 0) return 1;
    int count = 0;
    while (n > 0) {
        n /= 10;
        count++;
    }
    return count;
}

int main() {
    vector<int> tests = {0, 7, 123, -4567, 100000};
    for (int t : tests) {
        cout << "countDigits(" << t << ") = " << countDigits(t) << endl;
    }
    return 0;
}
