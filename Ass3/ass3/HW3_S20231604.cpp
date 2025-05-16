#include "records.h"

void RECORDS::sort_records_insertion(int start_index, int end_index) {
	// Classic Insertion sort
	for (int i = start_index + 1; i <= end_index; i++) {
		tmp = records[i];
		int j = i;
		while ((j > start_index) && (compare_keys((const void*)&tmp, (const void*)(&records[j - 1])) < 0)) {
			records[j] = records[j - 1];
			j--;
		}
		records[j] = tmp;
	}
}

void RECORDS::adjust_max(RECORD arr[], int root, int n) {
	int child, rootkey;
	RECORD temp = arr[root];
	rootkey = arr[root].key;
	child = 2 * root + 1; //left child
	while (child <= n) {
		if ((child < n) && (arr[child].key < arr[child + 1].key))
			child++;
		if (rootkey > arr[child].key)
			break; // compare root and maxchild whether child is bigger or not
		else { // we need to keep going until child is bigger than rootkey
			arr[root] = arr[child]; //move to parent
			root = child;
			child = child * 2 + 1;
		}
	}
	arr[root] = temp;
}

void RECORDS::sort_records_heap(int start_index, int end_index) {
	// Classic heap sort
	RECORD temp;
	ll size = end_index - start_index;
	RECORD* tempRecord = (RECORD*)malloc((size+ 1) * sizeof(RECORD));
	if (tempRecord != nullptr) {
		memcpy(tempRecord, records + start_index, (size + 1) * sizeof(RECORD));
	}
	else printf("Allocation Error\n");

	// 1. Make a max heap
	for (int i = (size + 1) / 2; i >= 0; i--) {
		adjust_max(tempRecord, i, size);
	}

	// 2. Extract items one by one.
	for (int i = size; i > 0; i--) {
		temp = tempRecord[0];
		tempRecord[0] = tempRecord[i];
		tempRecord[i] = temp;
		adjust_max(tempRecord, 0, i - 1);
	}

	memcpy(records + start_index, tempRecord, (size + 1) * sizeof(RECORD));
	free(tempRecord);
}

void RECORDS::adjust_min( int root, int n) {
	int child, rootkey;
	RECORD temp = records[root];
	rootkey = records[root].key;
	child = 2 * root + 1; //left child
	while (child <= n) {
		if ((child < n) && (records[child].key > records[child + 1].key))
			child++;
		if (rootkey < records[child].key)
			break; // compare root and maxchild whether child is bigger or not
		else { 
			records[root] = records[child]; //move to parent
			root = child;
			child = child * 2 + 1;
		}
	}
	records[root] = temp;
}

void RECORDS::sort_records_weird(int start_index, int end_index) {
	// A weird sort with a make min heap operation followed by insertion sort
	// 1. Make a min heap
	for (int i = (end_index - start_index + 1) / 2; i >= start_index; i--) {
		adjust_min(i, end_index);
	}
	// 2. Apply insertion sort
	sort_records_insertion(start_index, end_index);
}

void RECORDS::sort_records_quick_classic(int start_index, int end_index) {
	// Classic quick sort without any optimization techniques applied
	int pivot;
	if (end_index - start_index > 0) {
		pivot = partition_origin(start_index, end_index);

		sort_records_quick_classic(start_index, pivot - 1);
		sort_records_quick_classic(pivot + 1, end_index);
	}
}
int RECORDS::partition_origin(int left, int right) {
	int j = left;
	int pivot = right;

	// right가 pivot임
	for (int i = left; i < right; i++) {
		if (records[i].key < records[pivot].key) {
			SWAP(&records[i], &records[j]);
			j++;
		}
	}
	SWAP(&records[pivot], &records[j]);
	pivot = j;

	return pivot;
}

int RECORDS::partition_three(int left, int right) {
	//셋 중 하나로 pivot 선택
	int j = left;
	int pivot = (right + left) / 2;
	if ((records[pivot].key < records[left].key && records[left].key < records[right].key)||
		(records[right].key < records[left].key && records[left].key < records[pivot].key)) {
		pivot = left;
	}
	else if ((records[pivot].key < records[right].key && records[right].key < records[left].key)||
		(records[left].key < records[right].key && records[right].key < records[pivot].key)) {
		pivot = right;
	}
	// pivot을 맨 뒤(right)로 옮기기
	SWAP(&records[right], &records[pivot]);

	// right가 pivot임
	for (int i = left; i < right; i++) {
		if (records[i].key < records[right].key) {
			SWAP(&records[i], &records[j]);
			j++;
		}
	}
	SWAP(&records[right], &records[j]);
	pivot = j;

	return pivot;
}

void RECORDS::sort_records_intro(int start_index, int end_index) {
	int n = end_index - start_index + 1;
	int maxdepth = floor(log2(n)) * 2;
	real_intro(start_index, end_index, maxdepth);
}


void RECORDS::real_intro(int start_index, int end_index, int maxdepth) {
	// Introsort described in https://en.wikipedia.org/wiki/Introsort
	ll n = end_index - start_index + 1;
	if (n <= 8) {
		sort_records_insertion(start_index, end_index);
	}
	else if (maxdepth == 0) {
		sort_records_heap(start_index, end_index);
	}
	else {
		if (end_index - start_index < 6e4)
			bookkeeping(start_index, end_index);
		else {
			int pivot = partition_three(start_index, end_index);
			real_intro(start_index, pivot - 1, maxdepth - 1);
			real_intro(pivot + 1, end_index, maxdepth - 1);
		}
	}
}

void RECORDS::bookkeeping(int first, int end) {
	int first1, first2, end1, end2;
	first2 = first; end2 = end;
	while (end2 - first2 > 0) {
		tmp = records[first];
		int pivot = partition_three(first2, end2);
		if (pivot < (first2 + end2) / 2) {
			first1 = first2; end1 = pivot - 1;
			first2 = pivot + 1; end2 = end2;
		}
		else {
			first1 = pivot + 1; end1 = end2;
			first2 = first2; end2 = pivot - 1;
		}
		bookkeeping(first1, end1);
	}
}

void RECORDS::sort_records_merge_with_insertion(int start_index, int end_index) {
	// Merge sort optimized by insertion sort only
	// Use insertion sort instead of combine 
	if (end_index - start_index + 1 <= 16) {
		sort_records_insertion(start_index, end_index);
		return;
	}
	if (start_index < end_index) {
		int middle = (start_index + end_index) / 2;
		sort_records_merge_with_insertion(start_index, middle);
		sort_records_merge_with_insertion(middle + 1, end_index);
		merge(start_index, middle, end_index);
	}
}

void RECORDS::merge(int left, int mid, int right) {
	
	memcpy(buffer+left, records + left, sizeof(RECORD) * (right - left + 1));
	int i_left = left, i_right = mid + 1, i = left;

	while ((i_left <= mid) && (i_right <= right)) {
		if (buffer[i_left].key < buffer[i_right].key) {
			records[i++] = buffer[i_left++];
		}
		else {
			records[i++] = buffer[i_right++];
		}
	}
	while (i_left <= mid) {
		records[i++] = buffer[i_left++];
	}
	while (i_right <= right) {
		records[i++] = buffer[i_right++];
	}
}