#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 3e5 + 5;
const int M = 20;
const ll INF = 2e18;

struct EdgeInfo {
    ll mn, mx, sum, g;
    bool empty;
    EdgeInfo() { mn = INF; mx = -INF; sum = 0; g = 0; empty = true; }
    EdgeInfo(ll v) { mn = v; mx = v; sum = v; g = abs(v); empty = false; }
};

EdgeInfo mergeEdges(EdgeInfo a, EdgeInfo b) {
    if (a.empty) return b;
    if (b.empty) return a;
    EdgeInfo res;
    res.mn = min(a.mn, b.mn);
    res.mx = max(a.mx, b.mx);
    res.sum = a.sum + b.sum;
    res.g = __gcd(a.g, b.g);
    res.empty = false;
    return res;
}

vector<pair<int, int>> adj[N];
int Parent[N][M], deep[N];
EdgeInfo info[N][M];

void dfs_edges(int node, int par, int w = 0) {
    deep[node] = deep[par] + 1;
    Parent[node][0] = par;
    // Edge (node -> parent) is only valid if node is not root
    if (node != 1) info[node][0] = EdgeInfo(w);
    else info[node][0] = EdgeInfo(); // Empty for root

    for (int i = 1; i < M; i++) {
        Parent[node][i] = Parent[Parent[node][i - 1]][i - 1];
        info[node][i] = mergeEdges(info[node][i - 1], info[Parent[node][i - 1]][i - 1]);
    }
    for (auto &edge : adj[node]) {
        if (edge.first != par) dfs_edges(edge.first, node, edge.second);
    }
}

EdgeInfo query_edge_path(int a, int b) {
    EdgeInfo res;
    if (deep[a] < deep[b]) swap(a, b);
    
    for (int i = M - 1; i >= 0; i--) {
        if (deep[a] - (1 << i) >= deep[b]) {
            res = mergeEdges(res, info[a][i]);
            a = Parent[a][i];
        }
    }
    
    if (a == b) return res;

    for (int i = M - 1; i >= 0; i--) {
        if (Parent[a][i] != Parent[b][i]) {
            res = mergeEdges(res, mergeEdges(info[a][i], info[b][i]));
            a = Parent[a][i];
            b = Parent[b][i];
        }
    }
    // Final edges connecting to the LCA
    return mergeEdges(res, mergeEdges(info[a][0], info[b][0]));
}