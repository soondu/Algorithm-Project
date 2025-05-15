#include <stdio.h>
#include <stdlib.h>
#include "measure_cpu_time.h"

#pragma warning(disable:4996)
#define CMAX 1000000000 
#define ll long long

int wid, hei, mmax;
int index_i, index_j, index_k, index_l;

int algo3(int **A) {
	int** T = (int**)malloc(hei * sizeof(int*));
	for (int j = 0; j < hei; j++) {
		T[j] = (int*)malloc(wid * sizeof(int));
		memset(T[j], 0, wid * sizeof(int));
	}
	int* prevSum = (int*)malloc((hei + 1) * sizeof(int));
	memset(prevSum, 0, (hei + 1) * sizeof(int));

	for (int i = 0; i < wid; i++) {
		int sum = 0;
		for (int j = 0; j < hei; j++) {
			sum += A[j][i];
			T[j][i] = sum + prevSum[j];
			prevSum[j] = T[j][i];  // 이전 합 저장
		}
	}
	free(prevSum);
	int maxSum = -2e9;
	for (int i = 0; i < wid; i++) {
		for (int j = i; j < wid; j++) {
			for (int k = 0; k < hei; k++) {
				for (int l = k; l < hei; l++) {
					int s = T[l][j];
					if (k) s -= T[k - 1][j];
					if (i) s -= T[l][i - 1];
					if (k > 0 && i > 0)s += T[k - 1][i - 1];
					if (s > maxSum) {
						maxSum = s;
						index_k = k;
						index_l = l;
						index_i = i;
						index_j = j;
					}
				}
			}
		}
	}

	for (int j = 0; j < hei; j++) {
		free(T[j]);
	}
	free(T);

	return maxSum;
}

int kadanes(int A[], int n, int* startIdx, int* endIdx) {
	int ThisSum, MaxSum, tmpStart=0;
	ThisSum = MaxSum = *startIdx= 0;
	for (int j = 0; j < n; j++) {
		ThisSum += A[j];
		if (ThisSum > MaxSum) {
			*startIdx = tmpStart;
			*endIdx = j;
			MaxSum = ThisSum;
		}
		else if (ThisSum < 0) {
			ThisSum = 0;
			tmpStart = j + 1;
		}
	}
	return MaxSum;
}

int algo5(int** A) {
	int maxSum = -2e9;
	index_i = index_j = index_k = index_l = 0;

	for (int left = 0; left < hei; left++) {
		int* temp = (int*)calloc(wid, sizeof(int));
		memset(temp, 0, wid);

		for (int right = left; right < hei; right++) {
			for (int i = 0; i < wid; i++) {
				temp[i] += A[i][right];
			}
			int currentStartRow, currentEndRow;
			int currentMax = kadanes(temp, wid - 1, &currentStartRow, &currentEndRow);

			if (currentMax > maxSum) {
				maxSum = currentMax;
				index_k = currentStartRow;
				index_l = currentEndRow;
				index_i = left;
				index_j = right;
			}
		}

		free(temp);
	}

	return maxSum;
}

int max1D(const int A[], int left, int right, int *startIdx, int *endIdx) {

	int MaxLeftSum, MaxRightSum, MaxLeftBorderSum, MaxRightBorderSum;
	int LeftBorderSum, RightBorderSum, center;
	int leftStart, leftEnd, rightStart, rightEnd, centerStart, centerEnd;

	if (left == right) {
		if (A[left] > 0) {
			*startIdx = *endIdx = left;
			return A[left];
		}
		else return 0;
	}

	center = (left + right) / 2;
	MaxLeftSum = max1D(A, left, center, &leftStart,&leftEnd);
	MaxRightSum = max1D(A, center + 1, right, &rightStart, &rightEnd);

	MaxLeftBorderSum = LeftBorderSum = 0;
	centerStart = center;
	for (int i = center; i >= left; i--) {
		LeftBorderSum += A[i];
		if (LeftBorderSum > MaxLeftBorderSum) {
			MaxLeftBorderSum = LeftBorderSum;
			centerStart = i;
		}		
	}

	MaxRightBorderSum = RightBorderSum = 0;
	centerEnd = center + 1;
	for (int i = center+1; i <= right; i++) {
		RightBorderSum += A[i];
		if (RightBorderSum > MaxRightBorderSum) {
			MaxRightBorderSum = RightBorderSum;
			centerEnd = i;
		}
	}

	if (MaxLeftSum >= MaxRightSum && MaxLeftSum >= (MaxLeftBorderSum + MaxRightBorderSum)) {
		*startIdx = leftStart;
		*endIdx = leftEnd;
		return MaxLeftSum;
	}
	else if (MaxRightSum >= MaxLeftSum && MaxRightSum >= (MaxLeftBorderSum + MaxRightBorderSum)) {
		*startIdx = rightStart;
		*endIdx = rightEnd;
		return MaxRightSum;
	}
	else {
		*startIdx = centerStart;
		*endIdx = centerEnd;
		return MaxLeftBorderSum + MaxRightBorderSum;
	}
}

int algo4(int** A) {
	int maxSum = -2e9;
	index_i = index_j = index_k = index_l = 0;

	for (int left = 0; left < hei; left++) {
		int* temp = (int*)calloc(wid, sizeof(int));
		memset(temp, 0, wid);

		for (int right = left; right < hei; right++) {
			for (int i = 0; i < wid; i++) {
				temp[i] += A[i][right];
			}
			int startRow, endRow;
			int currentMax = max1D(temp, 0, wid - 1, &startRow, &endRow);

			if (currentMax > maxSum) {
				maxSum = currentMax;
				index_k = startRow;
				index_l = endRow;
				index_i = left;
				index_j = right;
			}
		}

		free(temp);
	}

	return maxSum;
}

FILE* open_file(const char* filename, const char* mode) {
	char FileName[100] = "Data\\";
	strcat(FileName, filename);

	FILE* stream = fopen(FileName, mode);
	if (stream == NULL) {
		perror("FILE OPEN ERROR");
	}
	return stream;  // NULL 또는 파일 포인터를 반환
}


void main(void) {
	FILE* fp = open_file("HW1_config.txt", "r");

	int n, num;
	char pgm_input[100], avg_input[100], bri_input[100],format[3];
	FILE* fpgm = NULL;
	FILE* favg = NULL;
	FILE* fbri = NULL;

	fscanf(fp, "%d", &n);
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%d %s %s %s", &num, pgm_input, avg_input, bri_input);

		//open pgm file
		fpgm = open_file(pgm_input, "r");
		printf("Open %s by Algorithm %d\n", pgm_input, num);

		//read pgm file first
		fscanf(fpgm, "%s", format);
		fscanf(fpgm, "%d %d", &wid, &hei);
		fscanf(fpgm, "%d", &mmax); //최대 그레이 스케일 값

		//open avg file
		favg = open_file(avg_input, "r");
		int avg_val;
		fscanf(favg, "%d", &avg_val);

		// allocate 2D array based on the information of pgm file
		int** twoD = (int**)malloc(hei * sizeof(int*));
		for (int j = 0; j < hei; j++) {
			twoD[j] = (int*)malloc(wid * sizeof(int));
		}

		// read values on the pgm file
		for (int y = 0; y < hei; y++) {
			for (int x = 0; x < wid; x++) {
				fscanf(fpgm, "%d ", &(twoD[y][x]));
				twoD[y][x] -= avg_val;
			}
		}

		// oepn bri file and write
		unsigned int n, count, count2;
		fbri = open_file(bri_input, "w");

		CHECK_TIME_START(_start, _freq);
		int res = 0;
		if (num == 3) {
			res = algo3(twoD);
		}
		else if (num == 4) {
			res = algo4(twoD);
		}
		else if (num == 5) {
			res = algo5(twoD);
		}
		else exit(0);
		CHECK_TIME_END(_start, _end, _freq, _compute_time);

		printf("%d %d %d %d %d\n", res, index_k, index_i, index_l, index_j);
		fprintf(fbri, "%d %d %d %d %d\n",res, index_k, index_i, index_l, index_j);
		printf("run time = %.3fms \n", _compute_time);

		// free 2D array
		for (int j = 0; j < hei; j++) {
			free(twoD[j]);
		}
		free(twoD); fclose(fpgm); fclose(favg); fclose(fbri);
	}
	fclose(fp);
}