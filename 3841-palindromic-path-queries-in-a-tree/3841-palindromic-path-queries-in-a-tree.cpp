class Solution {
    struct Fenwick {
        int n;
        vector<int> bit;

        Fenwick(int n) : n(n), bit(n + 2, 0) {}

        void update(int i, int mask) {
            for (; i <= n + 1; i += i & -i)
                bit[i] ^= mask;
        }

        int query(int i) {
            int res = 0;
            for (; i > 0; i -= i & -i)
                res ^= bit[i];
            return res;
        }

        // Apply XOR to the Euler interval [l, r].
        void rangeUpdate(int l, int r, int mask) {
            update(l + 1, mask);
            update(r + 2, mask);
        }

        // Get the XOR update affecting a node at Euler position pos.
        int pointQuery(int pos) {
            return query(pos + 1);
        }
    };

public:
    vector<bool> palindromePath(
        int n,
        vector<vector<int>>& edges,
        string s,
        vector<string>& queries
    ) {
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        int LOG = 1;
        while ((1 << LOG) <= n) ++LOG;

        vector<vector<int>> up(LOG, vector<int>(n, 0));
        vector<int> depth(n, 0), tin(n), tout(n);
        vector<int> baseMask(n, 0);

        // Iterative DFS to avoid stack overflow.
        // State 0 = entering a node, state 1 = exiting it.
        vector<array<int, 3>> st;
        st.push_back({0, 0, 0});

        int timer = 0;

        while (!st.empty()) {
            auto [u, p, state] = st.back();
            st.pop_back();

            if (state == 0) {
                tin[u] = timer++;

                up[0][u] = p;
                for (int j = 1; j < LOG; ++j)
                    up[j][u] = up[j - 1][up[j - 1][u]];

                baseMask[u] =
                    (u == 0 ? 0 : baseMask[p]) ^
                    (1 << (s[u] - 'a'));

                st.push_back({u, p, 1});

                for (int i = (int)adj[u].size() - 1; i >= 0; --i) {
                    int v = adj[u][i];
                    if (v == p) continue;

                    depth[v] = depth[u] + 1;
                    st.push_back({v, u, 0});
                }
            } else {
                tout[u] = timer - 1;
            }
        }

        auto lca = [&](int u, int v) {
            if (depth[u] < depth[v])
                swap(u, v);

            int diff = depth[u] - depth[v];

            for (int j = 0; j < LOG; ++j)
                if (diff & (1 << j))
                    u = up[j][u];

            if (u == v) return u;

            for (int j = LOG - 1; j >= 0; --j) {
                if (up[j][u] != up[j][v]) {
                    u = up[j][u];
                    v = up[j][v];
                }
            }

            return up[0][u];
        };

        Fenwick fw(n);
        vector<bool> ans;

        for (const string& q : queries) {
            string type;
            int u;
            stringstream ss(q);
            ss >> type >> u;

            if (type == "update") {
                char c;
                ss >> c;

                if (s[u] == c) continue;

                int delta = (1 << (s[u] - 'a')) ^
                            (1 << (c - 'a'));

                // Changing u affects root-to-node masks
                // of every node in u's subtree.
                fw.rangeUpdate(tin[u], tout[u], delta);
                s[u] = c;
            } else {
                int v;
                ss >> v;

                int pu = baseMask[u] ^ fw.pointQuery(tin[u]);
                int pv = baseMask[v] ^ fw.pointQuery(tin[v]);

                int w = lca(u, v);

                int mask = pu ^ pv ^ (1 << (s[w] - 'a'));

                // A palindrome permutation allows at most
                // one character with an odd frequency.
                ans.push_back((mask & (mask - 1)) == 0);
            }
        }

        return ans;
    }
};
