#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

struct StringAlgorithm {
	//  计算后缀数组，rk[i]表示i开头的后缀的排名，sa[i]表示第i名的后缀的起始位置
	//  s是正常的字符串，sa和rk返回以1为开始意义下的数组
	static void suffix_array(const std::string& s, std::vector<int>& sa, std::vector<int>& rk) {
		//  调整值域范围，s有时候可以变成vector<int>，此时范围就不能是写死的128了
		const int value_range = 256;

		int n = s.size();
		sa.resize(n + 1);
		rk.resize(n + 1);
		static std::vector<int> oldrk, id, key1, cnt(value_range);
		oldrk.resize(2 * n + 2), id.resize(n + 1), key1.resize(n + 1);

		auto cmp = [](int x, int y, int w) {
			return oldrk[x] == oldrk[y] && oldrk[x + w] == oldrk[y + w];
		};

		int m = value_range - 1, p;
		for (int i = 1; i <= n; ++i) ++cnt[rk[i] = s[i - 1]];
		for (int i = 1; i <= m; ++i) cnt[i] += cnt[i - 1];
		for (int i = n; i >= 1; --i) sa[cnt[rk[i]]--] = i;

		for (int w = 1; true; w <<= 1, m = p) {
			p = 0;
			for (int i = n; i > n - w; --i) id[++p] = i;
			for (int i = 1; i <= n; ++i) {
				if (sa[i] > w) id[++p] = sa[i] - w;
			}
			for (int i = 0; i < value_range; ++i) cnt[i] = 0;
			for (int i = 1; i <= n; ++i) ++cnt[key1[i] = rk[id[i]]];

			for (int i = 1; i <= m; ++i) cnt[i] += cnt[i - 1];
			for (int i = n; i >= 1; --i) sa[cnt[key1[i]]--] = id[i];
			for (int i = 1; i <= n; ++i) oldrk[i] = rk[i];

			p = 0;
			for (int i = 1; i <= n; ++i)
				rk[sa[i]] = cmp(sa[i], sa[i - 1], w) ? p : ++p;
			if (p == n) {
				for (int i = 1; i <= n; ++i) sa[rk[i]] = i;
				break;
			}
		}
	}

	//  在线构造next，保证len(s) - len(pi) == 1
	static void addToNext(char c, std::string& s, std::vector<int>& pi) {
		s.push_back(c);
		int n = pi.size();
		pi.push_back(0);
		if (n > 0) {
			int j = pi[n - 1];
			while (j > 0 && s[n] != s[j]) j = pi[j - 1];
			if (s[n] == s[j]) j++;
			pi[n] = j;
		}
	}

	//  计算pi数组，pi[i]表示s的前缀==s[1:i+1]的后缀的最长长度
	static std::vector<int> getNext(const std::string& s) {
		int n = s.size();
		std::vector<int> pi(n, 0);
		for (int i = 1; i < n; ++i) {
			int j = pi[i - 1];
			while (j > 0 && s[j] != s[i]) j = pi[j - 1];
			if (j == 0 && s[0] != s[i]) pi[i] = 0;
			else pi[i] = j + 1;
		}
		return pi;
	}

	static int kmp(const std::string& s, const std::string& t) {
		std::vector<int> pi = getNext(t);
		int m = s.size(), n = t.size();
		int j = 0;
		for (int i = 0; i < m; ++i) {
			while (j > 0 && s[i] != t[j]) j = pi[j - 1];
			if (s[i] == t[j]) j++;
			if (j == n) return i - n + 1;
		}
		return -1;
	}

	//  z[i]表示s和s[i:]的最长公共前缀长度
	static std::vector<int> z_algorithm(const std::string& s) {
		//  0--->………………
		//  …………i--->……
		int n = s.size();
		std::vector<int> z(n);
		for (int i = 1, l = 0, r = 0; i < n; ++i) {
			if (i <= r && z[i - l] < r - i + 1) {
				z[i] = z[i - l];
			}
			else {
				z[i] = r - i + 1;
				if (z[i] < 0) z[i] = 0;
				while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i] += 1;
			}
			if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
		}
		return z;
	}

	//  返回最长回文区间，左闭右开，下标以0开始
	std::pair<int, int> manacher(const std::string& str) {
		std::string s(str.size() * 2 + 1, '$');
		for (int i = 0; i < str.size(); ++i) s[i * 2 + 1] = str[i];
		int n = s.size();
		std::vector<int> r(n, 1);
		int right = 0, mid = -1;
		for (int i = 0; i < n; ++i) {
			if (i < right) r[i] = std::min(r[2 * mid - i], right - i);
			while (i + r[i] < n && i - r[i] >= 0 && s[i + r[i]] == s[i - r[i]])
				r[i]++;
			if (i + r[i] > right) {
				right = i + r[i];
				mid = i;
			}
		}
		int maxr = 0, maxi = -1;
		for (int i = 0; i < n; ++i) {
			if (r[i] > maxr) {
				maxi = i;
				maxr = r[i];
			}
		}
		std::pair<int, int> res((maxi - maxr + 1) / 2, (maxi + maxr) / 2);
		return res;
	}
};
