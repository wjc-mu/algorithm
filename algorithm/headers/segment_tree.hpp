#pragma once
#include <vector>
#include <algorithm>
#include <limits>
#include <assert.h>
struct SegTree {
#define USE_SUM 1
#define USE_MIN 2
#define USE_MAX 4

private:
#ifdef USE_SUM
    std::vector<long long> sum;
#endif
#ifdef USE_MIN
    std::vector<long long> minv;
#endif
#ifdef USE_MAX
    std::vector<long long> maxv;
#endif

    std::vector<long long> lazy;
    std::vector<long long> a;
    int n;

public:
    SegTree() : n(0) {}
    SegTree(int size) {
        resize(size);
    }

    void resize(int size) {
        n = size;
#ifdef USE_SUM
        sum.assign(size * 4, 0);
#endif
#ifdef USE_MIN
        minv.assign(size * 4, 0);
#endif
#ifdef USE_MAX
        maxv.assign(size * 4, 0);
#endif
        lazy.assign(size * 4, 0);
        a.assign(n, 0);
    }

private:
    int ls(int x) { return 2 * x + 1; }
    int rs(int x) { return 2 * x + 2; }

    void push_up(int x) {
#ifdef USE_SUM
        sum[x] = sum[ls(x)] + sum[rs(x)];
#endif
#ifdef USE_MIN
        minv[x] = std::min(minv[ls(x)], minv[rs(x)]);
#endif
#ifdef USE_MAX
        maxv[x] = std::max(maxv[ls(x)], maxv[rs(x)]);
#endif
    }

    void push_down(int x, int ln, int rn) {
        if (lazy[x])
        {
            lazy[ls(x)] += lazy[x];
            lazy[rs(x)] += lazy[x];

#ifdef USE_SUM
            sum[ls(x)] += lazy[x] * ln;
            sum[rs(x)] += lazy[x] * rn;
#endif
#ifdef USE_MIN
            minv[ls(x)] = std::min(minv[ls(x)], minv[x]);
#endif
#ifdef USE_MAX
            maxv[ls(x)] = std::max(maxv[ls(x)], maxv[x]);
#endif

            lazy[x] = 0;
        }
    }
    void refresh_node(int pos, long long x) {
#ifdef USE_SUM
        sum[pos] = x;
#endif
#ifdef USE_MIN
        minv[pos] = x;
#endif
#ifdef USE_MAX
        maxv[pos] = x;
#endif
        return;
    }

    void build_rec(int start, int end, int node) {
        if (start == end) {
            refresh_node(node, a[start]);
            return;
        }
        int mid = (start + end) / 2;
        build_rec(start, mid, ls(node));
        build_rec(mid + 1, end, rs(node));
        push_up(node);
    }

    void update_pos_rec(int idx, long long val, int start, int end, int node) {
        if (start == end) {
            refresh_node(node, val);
            a[idx] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (start <= idx && idx <= mid) {
            update_pos_rec(idx, val, start, mid, ls(node));
        }
        else {
            update_pos_rec(idx, val, mid + 1, end, rs(node));
        }
        push_up(node);
    }

    void update_range_rec(int L, int R, long long val, int start, int end, int node) {
        if (L <= start && end <= R)
        {
#ifdef USE_SUM
            sum[node] += val * (end - start + 1);
#endif

#ifdef USE_MIN

#endif
#ifdef USE_MAX

#endif

            lazy[node] += val;
            return;
        }

        int mid = (start + end) / 2;
        push_down(node, mid - start + 1, end - mid);
        if (L <= mid) {
            update_range_rec(L, R, val, start, mid, ls(node));
        }
        if (R > mid) {
            update_range_rec(L, R, val, mid + 1, end, rs(node));
        }
        push_up(node);
    }

    long long query_sum_rec(int L, int R, int start, int end, int node) {
#ifdef USE_SUM  
        if (R < start || L > end)    // 没有交集
            return 0;
        else if (start == end)   // 区间缩小到1时
            return sum[node];
        else if (start <= L && R >= end) // 完全在区间内
            return sum[node];
        int mid = (start + end) / 2;

        push_down(node, mid - start + 1, end - mid);

        long long res = 0;
        res += query_sum_rec(L, R, start, mid, ls(node));
        res += query_sum_rec(L, R, mid + 1, end, rs(node));
        return res;
#else
        return 19260817;
#endif
    }

    long long query_max_rec(int L, int R, int start, int end, int node) {
#ifdef USE_MAX    
        if (R < start || L > end)    // 没有交集
            return INT64_MIN;
        else if (start == end)   // 区间缩小到1时
            return maxv[node];
        else if (start <= L && R >= end) // 完全在区间内
            return maxv[node];

        int mid = (start + end) / 2;
        push_down(node, mid - start + 1, end - mid);

        long long res_l = query_max_rec(L, R, start, mid, ls(node));
        long long res_r = query_max_rec(L, R, mid + 1, end, rs(node));
        return std::max<long long>(res_l, res_r);
#else
        return 19260817;
#endif
    }

    long long query_min_rec(int L, int R, int start, int end, int node) {
#ifdef USE_MIN
        if (R < start || L > end)    // 没有交集
            return INT64_MAX;
        else if (start == end)   // 区间缩小到1时
            return minv[node];
        else if (start <= L && R >= end) // 完全在区间内
            return minv[node];

        int mid = (start + end) / 2;
        push_down(node, mid - start + 1, end - mid);

        long long res_l = query_min_rec(L, R, start, mid, ls(node));
        long long res_r = query_min_rec(L, R, mid + 1, end, rs(node));
        return std::min<long long>(res_l, res_r);
#else
        return 19260817;
#endif
    }

public:
    void build(const std::vector<int>& v) {
        if (n < v.size()) {
            resize(v.size());
        }
        n = v.size();
        for (int i = 0; i < n; ++i) a[i] = v[i];
        build_rec(0, n - 1, 0);
    }

    //  单点修改，将pos处改为val
    void update(int pos, long long val) {
        update_pos_rec(pos, val, 0, n - 1, 0);
    }
    //  单点修改，在pos处加上val
    void add(int pos, long long val) {
        long long now = a[pos] + val;
        update(pos, now);
    }

    //  区间更新，[left, right)值更新为val
    void update(int left, int right, long long val) {
        update_range_rec(left, right - 1, val, 0, n - 1, 0);
    }

    //  区间查询，[left, right)的和
    long long query_sum(int left = 0, int right = INT_MAX) {
#ifdef USE_SUM
        right = std::min(right, n);
        long long res = query_sum_rec(left, right - 1, 0, n - 1, 0);
        return res;
#else
        return 19260817;
#endif
    }
    //  区间查询，[left, right)的最大值
    long long query_max(int left = 0, int right = INT_MAX) {
#ifdef USE_MAX
        right = std::min(right, n);
        long long res = query_max_rec(left, right - 1, 0, n - 1, 0);
        return res;
#else
        return 19260817;
#endif
    }

    long long query_min(int left = 0, int right = INT_MAX) {
#ifdef USE_MAX
        right = std::min(right, n);
        long long res = query_min_rec(left, right - 1, 0, n - 1, 0);
        return res;
#else
        return 19260817;
#endif
    }

#undef USE_SUM
#undef USE_MIN
#undef USE_MAX
};
