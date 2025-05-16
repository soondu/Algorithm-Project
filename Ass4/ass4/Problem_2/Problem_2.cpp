#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct File {
    string input_file;
    string output_file;
};

struct String {
    int string_length;
    string str;
};

pair<int, string> LPS(int str_length, const string& str) {
    int n = str_length;
    vector<vector<int>> dp(n, vector<int>(n, 0)); // DP ���̺� �ʱ�ȭ

    // ���� 1�� �κ� ���ڿ��� �Ӹ���� ���� 1
    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

    // Filling DP
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1; // �κ� ���ڿ��� �� ��ġ
            if (str[i] == str[j]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
            }
            else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
        }
    }

    // backtracking LPS
    string lps;
    int i = 0, j = n - 1;
    while (i <= j) {
        if (str[i] == str[j]) {
            // ���� �� ���ڰ� ������ �߰�
            lps.push_back(str[i]);
            i++;
            j--;
        }
        else if (i + 1 < n && dp[i + 1][j] > dp[i][j - 1]) { // �Ʒ��� ��� ����
            i++;
        }
        else if (j - 1 >= 0) { // ���� ��� ����
            j--;
        }
    }

    // LPS�� Ȧ�� ���̸� �߾� ���� ������ ������ ���ڿ� �߰�
    string lps_reversed = lps;
    if (lps.size() * 2 - 1 == dp[0][n - 1]) {
        lps_reversed.pop_back();
    }
    reverse(lps_reversed.begin(), lps_reversed.end());
    lps += lps_reversed;

    return { dp[0][n - 1], lps };

}

int main() {
    ifstream file("LPS_command.txt");
    if (!file.is_open()) {
        cout << "Cannot open file" << endl;
        return 0;
    }

    int file_num;
    file >> file_num;
    //file num���� input output file �迭 �Ҵ�
    vector<File> files(file_num);
    for (int i = 0; i < file_num; ++i) {
        file >> files[i].input_file >> files[i].output_file;
    }

    for (const auto& folder : files) {
        //file num���� input file�� ����
        ifstream input_file(folder.input_file);
        if (!input_file.is_open()) {
            cout << "Cannot open input file: " << folder.input_file << endl;
            return -1;
        }

        //string num���� string�迭 �Ҵ�
        int string_num;
        input_file >> string_num;
        vector<String> strings(string_num);

        for (int i = 0; i < string_num; ++i) {
            input_file >> strings[i].string_length >> strings[i].str;
        }
        input_file.close();


        // output file
        ofstream output_file(folder.output_file);
        if (!output_file.is_open()) {
            cout << "Cannot open output file: " << folder.output_file << endl;
            continue;
        }
        output_file << string_num << endl;
        for (int i = 0; i < string_num;i++) {
            auto result = LPS(strings[i].string_length, strings[i].str);
            cout << result.first << "\n" << result.second << endl;
            output_file << result.first << "\n" << result.second << endl;
        }
        cout << endl;
        output_file.close();
    }

}