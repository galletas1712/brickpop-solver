#include "board.cpp"
#include <unordered_map>
#include <cassert>
using namespace std;

unordered_map<int, int> mp;
unordered_map<long long, pair<int,int> > to;

int solve(Board b, int depth) {
	if(b.empty()) return 0;
	if(b.invalid()) return -MOD;

	long long hsh = b.getHash();
	int colors = b.getColors();
	if(mp.count(hsh)) return mp[hsh]; // change;
	vector<pair<int,pair<int,int> > > costs;
	bool mark[N][N];
	memset(mark, 0, sizeof(mark));

	for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) {
		if(b.root(i*N + j) != i*N + j || b.getCost(i, j) == -MOD) continue; 
		costs.emplace_back(b.getCost(i, j), make_pair(i, j));
	}
	sort(costs.begin(), costs.end(), greater<pair<int, pair<int,int> > >());
	int ret = -MOD;
	for(int i = 0; i < min(6 - colors, (int) costs.size()); i++) {
		int x, y; tie(x, y) = costs[i].second;
		Board tmp = b;
		int cost = b.getCost(x, y);
		tmp.del(x, y);
		int res = solve(tmp, depth+1) + cost * (cost - 1);
		if(res > ret) {
			ret = res;
			to[hsh] = costs[i].second;
		}
	}
	if(ret < 0) ret = -MOD;
	// cout << ret << ' ' << depth << ' ' << hsh << ' ' << to[hsh].first << ' ' << to[hsh].second << endl;
	// b.debug();
	// cout << endl;
	mp[hsh] = max(ret, mp[hsh]);
	return ret;
}

int main() {
	string s;
	cin >> s;
	Board b(s);
	int cost = solve(b, 0);
	long long hsh = b.getHash();
	vector<pair<int,int> > sol;
	while(to.count(hsh)) {
		sol.push_back(to[hsh]);
		// cout << b.getCost(to[hsh].first, to[hsh].second) << endl;
		b.del(to[hsh].first, to[hsh].second);
		hsh = b.getHash();
		b.debug();
		cout << endl;
		// cout << !b.invalid() << ' ' << b.getHash() << endl;
	}
	cout << "Score: " << cost << endl;
	cout << "Moves:\n";
	for(auto p: sol) {
		cout << p.first+1 << ' ' << p.second+1 << endl;
	}
}
