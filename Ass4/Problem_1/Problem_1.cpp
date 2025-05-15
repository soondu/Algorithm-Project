#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>
using namespace std;

#define MAX_NUM 1e15

struct File {
    string input_file;
    string output_file;
};

struct Dot {
    double x;
    double y;
};

double distance(const Dot& a, const Dot& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void traceback(int i, int j, const vector<vector<double>>& S, const vector<vector<double>>& D,
    vector<pair<int, int>>& chords, vector<vector<bool>>& visited, double& total_length) {
    // 인접한 정점은 대각선을 추가할 수 없음
    if (j - i <= 1) {
        return;
    }    
    for (int k = i + 1; k < j; ++k) {
        if (abs(S[i][j] - (S[i][k] + S[k][j] + D[i][k] + D[k][j])) < 1e-6) {
            // 대각선 추가
            if (!visited[i][k]) {
                if (abs(k - i) > 1) {//인접한 꼭짓점은 안됨
                    chords.push_back({ i, k });
                    visited[i][k] = true;
                    total_length += D[i][k];
                }
            }
            if (!visited[k][j]) {
                if (abs(k - j) > 1) {
                    chords.push_back({ k, j });
                    visited[k][j] = true;
                    total_length += D[k][j];
                }
            }

            // 재귀 호출: 왼쪽과 오른쪽 다각형 처리
            traceback(i, k, S, D, chords, visited, total_length);
            traceback(k, j, S, D, chords, visited, total_length);
            break;
        }
    }
}



int main() {
    fstream file("MT_command.txt");
    if (!file.is_open()) {
        cout << "Cannot open file" << endl;
        return 0;
    }

    int file_num;
    file >> file_num;
    //file num개의 input output file 배열 할당
    vector<File> files(file_num);
    for (int i = 0; i < file_num; ++i) {
        file >> files[i].input_file >> files[i].output_file;
    }

    for (const auto& folder : files) {
        //file num개의 input file을 연다
        ifstream input_file(folder.input_file);
        if (!input_file.is_open()) {
            cout << "Cannot open input file: " << folder.input_file << endl;
            return -1;
        }

        //dot num개의 dot배열 할당
        int dot_num;
        input_file >> dot_num;
        vector<Dot> dots(dot_num);

        for (int i = 0; i < dot_num; ++i) {
            input_file >> dots[i].x >> dots[i].y;
        }
        input_file.close();

        // Calculate diagonal lengths
        vector<vector<double>> D(dot_num, vector<double>(dot_num, 0));
        for (int i = 0; i < dot_num; i++) {
            for (int j = 0; j < dot_num; j++) {
                if (abs(i - j) > 1 && abs(i - j) < dot_num - 1) {
                    D[i][j] = distance(dots[i], dots[j]);
                }
            }
        }

        // Dynamic programming table
        vector<vector<double>> S(dot_num, vector<double>(dot_num, 0));
        vector<vector<bool>> visited(dot_num, vector<bool>(dot_num, false));
        // Base cases
        for (int i = 0; i < dot_num; ++i) {
            S[i][(i + 1) % dot_num] = 0; // No cost s=2
            S[i][(i + 2) % dot_num] = 0; // No cost s=3
        }

        for (int s = 4; s <= dot_num; s++) {
            for (int i = 0; i <= dot_num - s; i++) {
                int j = (i + s - 1) % dot_num;
                S[i][j] = MAX_NUM;
                for (int k = i + 1; k < j; k++) {
                    S[i][j] = min(S[i][j],
                        S[i][k] + S[k][j] + D[i][k] + D[k][j]);
                }
            }
        }

        // Print S
        /*for (int i = 0; i < dot_num; ++i) {
            for (int j = 0; j < dot_num; ++j) {
                cout << S[i][j] << " ";
            }
            cout << endl;
        }*/
        double best = S[0][dot_num - 1];
        cout << best << endl;

        // Triangulation
        vector<pair<int, int>> chords; // 대각선
        double total_length = 0.0; // 총 길이
        
        //백 트래킹
        traceback(0, dot_num - 1, S, D, chords, visited, total_length);

        // 대각선 정렬 및 중복 제거
        sort(chords.begin(), chords.end());
        chords.erase(unique(chords.begin(), chords.end()), chords.end());

        //n-3개만
        /*while (chords.size() > dot_num - 3) {
            chords.pop_back();
        }*/

        // output file
        ofstream output_file(folder.output_file);
        if (!output_file.is_open()) {
            cout << "Cannot open output file: " << folder.output_file << endl;
            continue;
        }

        output_file << fixed << setprecision(3) << total_length << endl; //total
        for (const auto& chord : chords) {
            cout << chord.first << " " << chord.second << endl;
            output_file << chord.first << " " << chord.second << endl;
        }
        cout << endl;
        output_file.close();
    }

    return 0;
}
