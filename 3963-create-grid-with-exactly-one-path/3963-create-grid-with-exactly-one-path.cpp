class Solution {
public:
    vector<string> createGrid(int m, int n) {
        vector<string> grid(m, string(n, '.'));

   
        for (int i = 0; i < m - 1; i++) {
            grid[i][n - 1] = '#';
        }

   
        for (int i = 1; i < m; i++) {
            grid[i][0] = '#';
        }

        return grid;
    }
};
