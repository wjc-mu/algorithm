#pragma once
#include <vector>

struct BIT {
    int n;
    std::vector<long long> c;
    BIT(long long _n) : c(_n + 3, 0), n(_n) {}

    int lowbit(int x) { return x & (-x); }

    void add(int i, long long k) {
        for (; i <= n; i += lowbit(i)) c[i] += k;
    }

    //  求和[1, i)
    long long getSum(int i) {
        i--;
        long long res = 0;
        for (; i > 0; i -= lowbit(i)) res += c[i];
        return res;
    }

    // 求和[left, right)
    long long getSum(int left, int right) {
        return getSum(right) - getSum(left);
    }
};
