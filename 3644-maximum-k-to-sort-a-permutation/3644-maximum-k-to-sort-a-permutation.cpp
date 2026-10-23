class Solution {
public:
    int sortPermutation(vector<int>& nums) {
        vector<int> target = nums;
        sort(target.begin(), target.end());

        int n = nums.size(), answer = -1;

        for (int i = 0; i < n; i++) {
            if (nums[i] != target[i]) {
                answer &= nums[i];
            }
        }

        return answer == -1 ? 0 : answer;
    }
};