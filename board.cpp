#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include <tuple>
#include <string>
#include <climits>
#include <cstring>
using namespace std;
const int N = 10, dx[4] = {0, 1, -1, 0}, dy[4] = {1, 0, 0, -1};
const long long MOD = 1e9+7;

inline bool valid(int i, int j) {
	return i >= 0 && i < N && j >= 0 && j < N;
}

struct Board {
	int a[N][N], dsu[N*N];
	int root(int v) { return (dsu[v] < 0 ? v : dsu[v] = root(dsu[v])); }
	void merge(int u, int v) {
		if((u = root(u)) == (v = root(v))) return;
		if(dsu[u] > dsu[v]) swap(u, v);
		dsu[u] += dsu[v];
		dsu[v] = u;
	}
	Board(string boardString) {
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++)
			a[i][j] = boardString[i*N + j] - '0';
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) dsu[N*i+j] = -1;
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) {
			for(int k = 0; k < 4; k++) if(valid(i+dx[k], j+dy[k]) && a[i][j] == a[i+dx[k]][j+dy[k]])
				merge(N*i+j, N*(i+dx[k])+j+dy[k]);
		}
	}
	bool operator=(Board& rhs) {
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++)
			a[i][j] = rhs.a[i][j];
	}
	int del(int x, int y) { // O(n^2 lg n^2) ~= 300 * c
		if(!a[x][y] || dsu[root(N*x+y)] == -1) return -MOD;
		int ret = getCost(x, y);
		bool mark[N][N];
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) mark[i][j] = 0;
		vector<int> ls[N];
		queue<pair<int,int> > q;
		q.emplace(x, y);
		mark[x][y] = true;
		ls[y].push_back(x);
		while(!q.empty()) {
			int i, j; tie(i, j) = q.front();
			q.pop();
			for(int k = 0; k < 4; k++) {
				if(valid(i+dx[k], j+dy[k]) && !mark[i+dx[k]][j+dy[k]] && a[i+dx[k]][j+dy[k]] == a[i][j]) {
					q.emplace(i+dx[k], j+dy[k]);
					mark[i+dx[k]][j+dy[k]] = true;
					ls[j+dy[k]].push_back(i+dx[k]);
				}
			}
		}
		// perform erase
		for(int j = 0; j < N; j++) if(ls[j].size()) { // running sum of delta
			sort(ls[j].begin(), ls[j].end(), greater<int>());
			for(int i = N-1, idx = 0; i >= 0; i--) {
				if(idx < ls[j].size() && ls[j][idx] == i) ++idx;
				else a[i+idx][j] = a[i][j];
			}
			for(int i = 0; i < ls[j].size(); i++) a[i][j] = 0;
		}
		//refactor
		int tmp[N][N];
		memset(tmp, 0, sizeof(tmp));
		vector<int> nonempty;
		for(int j = 0; j < N; j++) {
			bool rowempty = true;
			for(int i = 0; i < N; i++) rowempty &= !a[i][j];
			if(!rowempty) nonempty.push_back(j);
		}
		for(int j = 0; j < nonempty.size(); j++) {
			for(int i = 0; i < N; i++) tmp[i][j] = a[i][nonempty[j]];
		}
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) a[i][j] = tmp[i][j];
		// revamp dsu
		memset(dsu, -1, sizeof(dsu));
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) if(a[i][j]) {
			for(int k = 0; k < 4; k++) if(valid(i+dx[k], j+dy[k]) && a[i][j] == a[i+dx[k]][j+dy[k]])
				merge(N*i+j, N*(i+dx[k])+j+dy[k]);
		}
		return ret;
	}
	int getCost(int i, int j) {
		int sz = -dsu[root(N*i+j)];
		if(sz == 1) return -MOD;
		return sz;
	}
	bool empty() {
		bool ret = true;
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) ret &= !a[i][j];
		return ret;
	}
	bool invalid() {
		bool ret = true;
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++)
			ret &= getCost(i, j) == -MOD;
		return ret;
	}
	long long getHash() {
		long long h = 0;
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++)
			h = (h * 6 + a[i][j] + 1) % MOD;
		return h;
	}
	int getColors() {
		int mx = 0;
		for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) mx = max(a[i][j], mx);
		return mx;
	}
	void debug() {
		for(int i = 0; i < N; i++, cout << endl) for(int j = 0; j < N; j++) cout << a[i][j] << ' ';
		cout << endl;
		// for(int i = 0; i < N; i++, cout << endl) for(int j = 0; j < N; j++) cout << dsu[N*i+j] << ' ';
	}
};