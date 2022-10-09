#pragma once
#include <vector>

struct UnionFind {
    std::vector<int> fa;
    std::vector<int> size;

    UnionFind(int n) : fa(n), size(n, 1) {
        for (int i = 0; i < n; ++i) fa[i] = i;
    }

    int getFa(int i) {
        static std::vector<int> st;
        while (fa[i] != i) {
            st.push_back(i);
            i = fa[i];
        }
        return i;
    }

    int merge(int i, int j) {
        int fi = getFa(i), fj = getFa(j);
        if (fi == fj) return fj;
        if (size[fi] > size[fj]) {
            size[fi] += size[fj];
            fa[fj] = fi;
            return fi;
        }
        else {
            size[fj] += size[fi];
            fa[fi] = fj;
            return fj;
        }
    }
};