#include <bits/stdc++.h>
using namespace std;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count()); // or mt19937 for int
long long Rand(long long l, long long r) {
    if (r < l) return 0;
    return l + rng() % (r - l + 1);
}


int main() {
    for (int n = 10; n <= 50; n += 10) {
        for (int mx = 10; mx <= 100; mx *= 10) {
            string filePath = "../data/n_" + to_string(n) + "_mx_" + to_string(mx);
            ofstream writer(filePath);
            writer << n << '\n';
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    writer << (i == j ? 0 : Rand(1, mx)) << " ";
                }
                writer << '\n';
            }
            writer.close();
        }
    }
}