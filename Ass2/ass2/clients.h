// 
// CSE3081-1/AIE3051-1 Design and Analysis of Algorithm
// (c) 2024 Soang University
//
// Example: Using min heap to manage clients with priority
// clients.h
//

#pragma once
#pragma warning(disable:4996)

#include <vector>

#define DEFAULT_MAX_CLIENTS 1024
#define NULL_CLIENT_ID UINT_MAX

struct CLIENT_DATA {
	int dummy;
	// more data fields for a client 
};

struct CLIENT_RECORD { 
	int priority;
	int index_to_heap;
	CLIENT_DATA* client_data;
	CLIENT_RECORD() {
		priority = INT_MAX; // when priority has value INT_MAX, the client is not in the system
		index_to_heap = -1;
		client_data = nullptr;
	}
	~CLIENT_RECORD() {
		if (!client_data)
			delete client_data;
	}
};

struct HEAP_ELEMENT {
	unsigned int client_ID;
	HEAP_ELEMENT() {
		client_ID = UINT_MAX;
	}
	HEAP_ELEMENT(int client_ID) {
		this->client_ID = client_ID;
	}
};

class CLIENTS {
	int n_max_clients; // maximum possible number of clients
	CLIENT_RECORD *clients;
	std::vector<HEAP_ELEMENT> _heap;

	void min_heap_initialize();
	void min_heap_insert(unsigned int client_ID);
	unsigned int min_heap_delete();
	void min_heap_adjust_priority(int index_to_heap, int delta);

public:
	CLIENTS();
	CLIENTS(int n_max_clients);
	~CLIENTS();

	void reset();
	void reset(int n_max_clients);
	bool active_client_ID(unsigned int client_ID);
	void add_client(unsigned int client_ID, int priority, CLIENT_DATA* client_data);
	unsigned int delete_client(int* priority, CLIENT_DATA** client_data);
	void adjust_client_priority(unsigned int client_ID, int priority_increment);
	// Note: priority_increment may be either positive, zero, or negative!!!

	void print_clients(const char* header);
	void print_clients_into_file(const char* filename); // this file will be used to grade HW2
	void print_min_heap(const char* header);
	void print_min_heap_into_file(const char* filename); // this file will be used to grade HW2
};


