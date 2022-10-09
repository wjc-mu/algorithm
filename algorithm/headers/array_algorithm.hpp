#pragma once
#include <vector>
#include <algorithm>

struct ArrayAlgorithm {
	static int lis(const std::vector<int>& a) {
		int n = a.size();
		static std::vector<int> v;
		v.resize(0);
		auto cmp = [&a](int x, int y) -> bool {

		};
		for (int i = 0; i < n; ++i) {
			if (v.size() == 0 || a[v.back()] < a[i]) v.push_back(i);
			else {

			}
		}
	}
};