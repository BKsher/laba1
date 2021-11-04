#pragma once
#include <map>
#include <vector>
using namespace std;

template <typename T>
class SparseMatrix
{
public:
	int n, m;
	map <pair<int, int>, T> matrix;

	SparseMatrix(int n, int m, vector <pair < pair <int, int>,  T> > points) {
		this->n = n;
		this->m = m;
		for (auto p : points)
			if(p.first.first <= n && p.first.second <= m) matrix[p.first] = p.second;
	}

	void addValue(int i, int j, T value) {
		if (i <= n && j <= m) matrix[{i, j}] = value;
	}

	void addValue(pair < pair <int, int>, T> p) {
		if (p.first.first <= n && p.first.second <= m) matrix[p.first] = p.second;
	}

	T findByIndex(int i, int j) {
		if (matrix.find({ i, j }) == matrix.end())
			return 0;
		return matrix[{i, j}];
	}

	vector <T> findByValue(T value) {
		vector <pair<int, int> > res;
		for (auto p : matrix)
			if (p.second == value) res.push_back(p.first);
		return res;
	}

	T findByCondition(bool (*f)(T)) {
		for (auto p : matrix)
			if (f(p.second))
				return p.second;
	}

	void print() {
		cout << "matrix has " << matrix.size() << " elements:\n";
		for (auto p : matrix)
			cout << "[" << p.first.first << "," << p.first.second << "]: " << p.second << '\n';
	}

	SparseMatrix add(SparseMatrix b) {
		SparseMatrix res(n, m, {});
		if (n != b.n || m != b.m) {
			cout << "Matrices have different size\n";
			return res;
		}
		auto apos = matrix.begin(), bpos = b.matrix.begin();
		while (apos != matrix.end() && bpos != b.matrix.end()) {
			if ((*apos).first < (*bpos).first) {
				res.addValue(*apos);
				apos++;
			} else if ((*apos).first > (*bpos).first) {
				res.addValue(*bpos);
				bpos++;
			}
			else {
				res.addValue((*apos).first.first, (*apos).first.second, (*apos).second + (*bpos).second);
				apos++;
				bpos++;
			}
		}
		while (apos != matrix.end()) res.addValue(*apos), apos++;
		while (bpos != b.matrix.end()) res.addValue(*bpos), bpos++;
		return res;
	}

	SparseMatrix transpose() {
		SparseMatrix res(m, n, {});
		for (auto p : matrix)
			res.addValue(p.first.second, p.first.first, p.second);
		return res;
	}

	SparseMatrix multiply(SparseMatrix b) {
		SparseMatrix res(n, b.m, {});
		if (m != b.n) {
			cout << "invalid dimensions\n";
			return res;
		}
		b = b.transpose();
		auto apos = matrix.begin(), bpos = b.matrix.begin();
		while(apos != matrix.end()) {
			int r = (*apos).first.first;
			while (bpos != b.matrix.end()) {
				int c = (*bpos).first.first;
				auto tempa = apos, tempb = bpos;
				T sum = 0;
				while (tempa != matrix.end() && (*tempa).first.first == r &&
					tempb != b.matrix.end() && (*tempb).first.first == c)
				{
					if ((*tempa).first.second < (*tempb).first.second)
						tempa++;
					else if ((*tempa).first.second > (*tempb).first.second)
						tempb++;
					else
						sum += (*tempa).second * (*tempb).second, tempa++, tempb++;
				}
				if (sum != 0) res.addValue(r, c, sum);
				while (bpos != b.matrix.end() && (*bpos).first.first == c) bpos++;
			}
			bpos = b.matrix.begin();
			while (apos != matrix.end() && (*apos).first.first == r) apos++;
		}
		return res;
	}
};

