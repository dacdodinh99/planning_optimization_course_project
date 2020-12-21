#include <bits/stdc++.h>
using namespace std;

const int REPS = 10;
const int NITERATIONS = 1000;

int n;
vector<vector<int>> c;

struct Solution {
    vector<vector<pair<int, int>>> round;
    vector<vector<int>> pos;

    Solution(int n) {
        round.resize(2 * n);
        pos.resize(2 * n, vector<int>(n, 0));
    }

    void preparePos() {
        for (int i = 0; i < n; i++) pos[0][i] = i, pos[2 * n - 1][i] = i;
        for (int i = 1; i < 2 * n - 1; i++) {
            for (auto foo : round[i]) {
                pos[i][foo.first] = pos[i][foo.second] = foo.first;
            }
        }
    }

    int evaluate(vector<vector<int>> &c) {
        int res = 0;
        for (int i = 1; i < 2 * n; i++) {
            for (int j = 0; j < n; j++) res += c[pos[i - 1][j]][pos[i][j]];
        }
        return res;
    }

};

Solution genSolution() {
    Solution res = Solution(n);
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = i;
    random_shuffle(a.begin(), a.end());

    for (int i = 1; i <= n - 1; i++) {
        for (int j = 0; j < n / 2; j++) {
            res.round[i].push_back({a[j], a[n - j - 1]});
        }
        rotate(a.begin() + 1, a.begin() + n - 1, a.end());
    }    

    random_shuffle(a.begin(), a.end());
    for (int i = n; i <= 2 * n - 2; i++) {
        for (int j = 0; j < n / 2; j++) {
            res.round[i].push_back({a[j], a[n - j - 1]});
        }
        rotate(a.begin() + 1, a.begin() + n - 1, a.end());
    }
    
    vector<vector<int>> cnt(n, vector<int>(n, 0));

    for (int i = 0; i < 2 * n; i++) {
        for (auto &foo : res.round[i]) {
            if (foo.first > foo.second) swap(foo.first, foo.second);
            if (cnt[foo.first][foo.second]) swap(foo.first, foo.second);
            ++cnt[foo.first][foo.second];
        }
    }

    res.preparePos();

    return res;
}

void readData() {
    cin >> n;

    c.resize(n, vector<int>(n, 0));
    
    for (auto &row : c) {
        for (auto &e : row) cin >> e;
    }
    
    return;
}

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count()); // or mt19937 for int
long long Rand(long long l, long long r) {
    if (r < l) return 0;
    return l + rng() % (r - l + 1);
}

void solve() {

    readData();
    
    Solution bestSolution = genSolution();

    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n ;j++) cout << c[i][j] << endl;
    // }
    
    for (int rep = 0; rep < REPS; rep++) {
        Solution curSolution = genSolution();
        
        int counter = 0;
        for (int iter = 0; iter < NITERATIONS; iter++) {
            // cout << "rep " << rep << ' ' << "iter " << iter << ' ' << curSolution.evaluate(c) << endl;
            if (counter == 5) break;
            bool type = Rand(0, INT16_MAX) % 2;
            Solution tmpSol = curSolution;
            int curCost = curSolution.evaluate(c);
            pair<int, int> best = {-1, -1};


            if (type == 0) { // Swap round
                for (int i = 1; i < 2 * n - 1; i++) {
                    for (int j = i + 1; j < 2 * n - 1; j++) {
                        swap(tmpSol.round[i], tmpSol.round[j]);
                        tmpSol.preparePos();
                        int tmpCost = tmpSol.evaluate(c);
                        if (tmpCost < curCost) {
                            curCost = tmpCost;
                            best = {i, j};
                        }
                        tmpSol = curSolution;
                    }
                }
                if (best == make_pair(-1, -1)) {
                    counter++;
                    continue;
                }
                swap(curSolution.round[best.first], curSolution.round[best.second]);
                curSolution.preparePos();
            } else { // Swap home - guest
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        for (int round = 1; round < 2 * n - 1; round++) {
                            for (auto &foo : tmpSol.round[round]) {
                                if (foo == make_pair(i, j) || foo == make_pair(j, i)) swap(foo.first, foo.second);
                            }
                        }
                        tmpSol.preparePos();
                        int tmpCost = tmpSol.evaluate(c);
                        if (tmpCost < curCost) {
                            curCost = tmpCost;
                            best = {i, j};
                        }
                        tmpSol = curSolution;
                    }
                }
                if (best == make_pair(-1, -1)) {
                    counter++;
                    continue;
                }
                for (int round = 1; round < 2 * n - 1; round++) {
                    for (auto &foo : curSolution.round[round]) {
                        if (foo == best || foo == make_pair(best.second, best.first)) swap(foo.first, foo.second);
                    }
                }
                curSolution.preparePos();
            }

            if (curSolution.evaluate(c) < bestSolution.evaluate(c)) bestSolution = curSolution;

            counter = 0;
        }
    }

    cout << bestSolution.evaluate(c) << endl;
    // for (int i = 1; i < 2 * n - 1; i++) {
    //     cout << "Round " << i << "th: ";
    //     for (auto foo : bestSolution.round[i]) cout << "(" << foo.first << ' ' << foo.second << ")";
    //     cout << endl;
    // } 
}

int main() {
    solve();    
    cout << "Time elapsed: " << clock() / CLOCKS_PER_SEC << " seconds" << endl;
    cerr << "Time elapsed: " << clock() / CLOCKS_PER_SEC << " seconds" << endl;
    return 0;
}