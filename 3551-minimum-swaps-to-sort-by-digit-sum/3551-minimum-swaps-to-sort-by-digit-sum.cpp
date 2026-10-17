
class Solution {
public:
    int digitSum(int n) {
        int sum = 0;

        while (n > 0) {
            sum += n % 10;
            n /= 10;
        }

        return sum;
    }

    int minSwaps(vector<int>& nums) {
        vector<int> target = nums;

        sort(target.begin(), target.end(), [&](int a, int b) {
            int sa = digitSum(a);
            int sb = digitSum(b);

            if (sa == sb)
                return a < b;

            return sa < sb;
        });

        unordered_map<int, int> correctIndex;

        for (int i = 0; i < target.size(); i++) {
            correctIndex[target[i]] = i;
        }

        int swaps = 0;

        for (int i = 0; i < nums.size(); i++) {
            while (nums[i] != target[i]) {
                int j = correctIndex[nums[i]];

                swap(nums[i], nums[j]);
                swaps++;
            }
        }

        return swaps;
    }
};
