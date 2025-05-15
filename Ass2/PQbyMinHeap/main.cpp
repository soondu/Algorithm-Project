// 
// CSE3081-1/AIE3051-1 Design and Analysis of Algorithm
// (c) 2024 Soang University
//
// Example: Using min heap to manage clients with priority
// main.cpp
//

#include "clients.h"

CLIENTS my_clients(65536);

void example_1() {
	my_clients.add_client(10037, 59, nullptr); 
	my_clients.add_client(10128, 14, nullptr);
	my_clients.add_client(10055, 13, nullptr);
	my_clients.add_client(10355, 77, nullptr);
	my_clients.add_client(10152, 19, nullptr);
	my_clients.add_client(10015, 66, nullptr);
	my_clients.add_client(10111, 54, nullptr);
	my_clients.add_client(10125, 37, nullptr);
	my_clients.add_client(10512, 25, nullptr);
	my_clients.add_client(10001, 2, nullptr);

	my_clients.print_clients("example_1");
	my_clients.print_min_heap("example_1");
	my_clients.print_min_heap_into_file("result_mh_ex_1.txt");
	my_clients.print_clients_into_file("result_cl_ex_1.txt");
}

void example_2() {
	int client_ID, priority;
	CLIENT_DATA* client_data;

	my_clients.add_client(10037, 59, nullptr);
	my_clients.add_client(10128, 14, nullptr);
	my_clients.add_client(10055, 13, nullptr);
	my_clients.add_client(10355, 77, nullptr);
	my_clients.add_client(10152, 19, nullptr);
	my_clients.add_client(10015, 66, nullptr);
	my_clients.add_client(10111, 54, nullptr);
	my_clients.add_client(10125, 37, nullptr);
	my_clients.add_client(10512, 25, nullptr);
	my_clients.add_client(10001, 2, nullptr);
	client_ID = my_clients.delete_client(&priority, &client_data);
	client_ID = my_clients.delete_client(&priority, &client_data);
	client_ID = my_clients.delete_client(&priority, &client_data);

	my_clients.print_clients("example_2");
	my_clients.print_min_heap("example_2");
	my_clients.print_min_heap_into_file("result_mh_ex_2.txt");
	my_clients.print_clients_into_file("result_cl_ex_2.txt");
}

void example_3() {
	int client_ID, priority;
	CLIENT_DATA* client_data;

	my_clients.add_client(10037, 59, nullptr);
	my_clients.add_client(10128, 14, nullptr);
	my_clients.add_client(10055, 13, nullptr);
	my_clients.add_client(10355, 77, nullptr);
	my_clients.add_client(10152, 19, nullptr);
	my_clients.add_client(10015, 66, nullptr);
	my_clients.add_client(10111, 54, nullptr);
	my_clients.add_client(10125, 37, nullptr);
	my_clients.add_client(10512, 25, nullptr);
	my_clients.add_client(10001, 2, nullptr);
	client_ID = my_clients.delete_client(&priority, &client_data);
	client_ID = my_clients.delete_client(&priority, &client_data);
	client_ID = my_clients.delete_client(&priority, &client_data);

	my_clients.add_client(20125, 18, nullptr);
	my_clients.add_client(20537, 49, nullptr);
	my_clients.adjust_client_priority(10125, +35);
	my_clients.add_client(20299, 22, nullptr);
	my_clients.adjust_client_priority(20537, -40);

	my_clients.print_clients("example_3");
	my_clients.print_min_heap("example_3");
	my_clients.print_min_heap_into_file("result_mh_ex_3.txt");
	my_clients.print_clients_into_file("result_cl_ex_3.txt");	 	
}

int main() {
	example_1();

	my_clients.reset();
	example_2();

	my_clients.reset();
	example_3();

	return 0;
}