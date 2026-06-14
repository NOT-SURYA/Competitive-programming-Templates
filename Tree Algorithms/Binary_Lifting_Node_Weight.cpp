#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 3e5 + 5;
const int M = 20;
const ll INF = 2e18;

struct Node {
    ll mn, mx, sum, g;
    bool empty;
    Node() { mn = INF; mx = -INF; sum = 0; g = 0; empty = true; }
    Node(ll v) { mn = v; mx = v; sum = v; g = abs(v); empty = false; }
};

Node merge(Node a, Node b) {
    if (a.empty) return b;
    if (b.empty) return a;
    Node res;
    res.mn = min(a.mn, b.mn);
    res.mx = max(a.mx, b.mx);
    res.sum = a.sum + b.sum;
    res.g = __gcd(a.g, b.g);
    res.empty = false;
    return res;
}

vector<int> adj[N];
int Parent[N][M], deep[N];
Node info[N][M];
ll val[N];

void dfs(int node, int par) {
    deep[node] = deep[par] + 1;
    Parent[node][0] = par;
    info[node][0] = Node(val[node]);

    for (int i = 1; i < M; i++) {
        Parent[node][i] = Parent[Parent[node][i - 1]][i - 1];
        info[node][i] = merge(info[node][i - 1], info[Parent[node][i - 1]][i - 1]);
    }
    for (int r : adj[node]) if (r != par) dfs(r, node);
}

Node query_path(int a, int b) {
    Node res;
    if (deep[a] < deep[b]) swap(a, b);
    
    // Lift a to b's level
    for (int i = M - 1; i >= 0; i--) {
        if (deep[a] - (1 << i) >= deep[b]) {
            res = merge(res, info[a][i]);
            a = Parent[a][i];
        }
    }
    
    if (a == b) return merge(res, Node(val[a]));

    for (int i = M - 1; i >= 0; i--) {
        if (Parent[a][i] != Parent[b][i]) {
            res = merge(res, merge(info[a][i], info[b][i]));
            a = Parent[a][i];
            b = Parent[b][i];
        }
    }
    // Merge a, b, and their common parent (LCA)
    res = merge(res, merge(info[a][0], info[b][0]));
    res = merge(res, Node(val[Parent[a][0]]));
    return res;
}