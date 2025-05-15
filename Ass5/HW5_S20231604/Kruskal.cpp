#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <queue>
#include "measure_cpu_time.h"

#define ll long long

using namespace std;

class DisjointSet {
    vector<int> parent, rank;

public:
    vector<int> componentSize;
    vector<ll> componentWeight;

    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
        componentSize.resize(n, 1);
        componentWeight.resize(n, 0);
    }
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unionSets(int x, int y, int edgeWeight) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
                componentSize[rootX] += componentSize[rootY];
                componentWeight[rootX] += componentWeight[rootY] + edgeWeight;
            }
            else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
                componentSize[rootY] += componentSize[rootX];
                componentWeight[rootY] += componentWeight[rootX] + edgeWeight;
            }
            else {
                parent[rootY] = rootX;
                rank[rootX]++;
                componentSize[rootX] += componentSize[rootY];
                componentWeight[rootX] += componentWeight[rootY] + edgeWeight;
            }
        }
        else {
            componentWeight[rootX] += edgeWeight;
        }
    }
};

class MinHeap {
    vector<int> indices;
    vector<int> weight;

public:
    MinHeap(int size, int MAX, const vector<int>& unsorted_weight) {
        indices.reserve(size);
        weight.resize(size, MAX);

        for (int i = 0; i < unsorted_weight.size(); i++) {
            indices.push_back(i);
            weight[i] = unsorted_weight[i];
        }

        sort();
    }

    void sort() {
        for (int i = (indices.size() / 2) - 1; i >= 0; i--) {
            adjust(i);
        }
    }
    void adjust(int idx) {
        int curIdx = idx;
        while (1) {
            int leftChild = 2 * curIdx + 1;
            int rightChild = 2 * curIdx + 2;
            int smallest = curIdx;

            if (leftChild < indices.size() && weight[indices[leftChild]] < weight[indices[smallest]]) {
                smallest = leftChild;
            }
            if (rightChild < indices.size() && weight[indices[rightChild]] < weight[indices[smallest]]) {
                smallest = rightChild;
            }
            if (smallest == curIdx) break;

            swap(indices[curIdx], indices[smallest]);
            curIdx = smallest;
        }
    }

    int extractMin() {
        int minIdx = indices[0];
        indices[0] = indices.back();
        indices.pop_back();
        adjust(0);

        return minIdx;
    }

    bool empty() {
        return indices.empty();
    }

};


int main() {
    fstream command_file("commands.txt");
    if (!command_file.is_open()) {
        cout << "Cannot open file" << endl;
        return 0;
    }
    string locat;
    command_file >> locat;
    vector<string> file_names;
    string file_name;

    while (command_file >> file_name) {
        file_names.push_back(file_name);        
    }
    command_file.close();

    ofstream output_file("commands.txt");
    if (!output_file.is_open()) {
        cout << "Cannot open file" << endl;
        return 0;
    }
    output_file << locat << endl;

    for (string file_name : file_names) {
        string path = locat + "\\" + file_name;
        ifstream input_file(path);
        if (!input_file.is_open()) {
            cout << "Cannot open file" << path << endl;
            continue;
        }

        cout << file_name << endl;
        output_file << file_name << endl;

        int n_vertices, n_edges, MAX_WEIGHT;
        input_file >> n_vertices >> n_edges >> MAX_WEIGHT;

        vector<int> from(n_edges), to(n_edges), weight(n_edges, MAX_WEIGHT);
        for (int i = 0; i < n_edges; i++) {
            input_file >> from[i] >> to[i] >> weight[i];
            weight[i] = (weight[i] > MAX_WEIGHT) ? MAX_WEIGHT : weight[i];
        }
        input_file.close();

        MinHeap minheap(n_edges, MAX_WEIGHT, weight);
        DisjointSet ds(n_vertices);
        ll cnt = 0;
        CHECK_TIME_START(_start, _freq);
        while (!minheap.empty()) {
            cnt++;
            int idx = minheap.extractMin();
            int u = from[idx];
            int v = to[idx];
            int w = weight[idx];
            if (ds.find(u) != ds.find(v)) {
                ds.unionSets(u, v, w);
            }
        }
        CHECK_TIME_END(_start, _end, _freq, _compute_time);
        //cout << "\n*** Time for sorting with " << file_name << " = " << _compute_time << "ms ***\n";

        //cout << "n_vertices=" << n_vertices << ", n_edges=" << n_edges << ", k_scanned=" << cnt << endl;

        vector<int> componentNum;
        for (int i = 0; i < n_vertices; i++) {
            if (i == ds.find(i)) {
                componentNum.push_back(i);
                cout << i << endl;
            }
        }

        string mst_result;
        mst_result = file_name.substr(0,file_name.size() - 4) + "_MST_result.txt";
        output_file << mst_result << endl;

        ofstream MSTresult(mst_result);

        int total_component = componentNum.size();
        cout << total_component << "\n";
        MSTresult << total_component << "\n";

        // vertex 오름차순으로 정렬
        sort(componentNum.begin(), componentNum.end(), [&](int a, int b) {
            if (ds.componentSize[a] != ds.componentSize[b])
                return ds.componentSize[a] < ds.componentSize[b];
            return ds.componentWeight[a] < ds.componentWeight[b];
            });

        for (int i = 0; i < total_component; i++) {
            cout << ds.componentSize[componentNum[i]] << " " << ds.componentWeight[componentNum[i]] << "\n\n";
            MSTresult << ds.componentSize[componentNum[i]] << " " << ds.componentWeight[componentNum[i]] << "\n";
        }
        MSTresult.close();
    }
    output_file.close();

    return 0;
}