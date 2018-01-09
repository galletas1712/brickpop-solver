#include <bits/stdc++.h>
using namespace std;

int main() {
	freopen("data.txt", "w", stdout);
	srand(time(NULL));
	string s;
	for(int i = 0; i < 100; i++) s += '0' + rand() % 5 + 1;
	printf("%s", s.c_str());
}
