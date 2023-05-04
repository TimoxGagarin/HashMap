#include "includes.hpp"
#include "excel.hpp"
#include <Windows.h>

enum options {
	LOAD = 1,
	ADD,
	REMOVE,
	FIND,
	PRINT,
	EXCEL,
	QUIT,
};

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	dict* name_ip = create_dict();
	map<string, string> name_ip_cpp;

	while (true) {
		cout << "Menu of dictionary:" << endl;
		cout << "1.Load file of dictionary" << endl;
		cout << "2.Add element into dictionary" << endl;
		cout << "3.Remove element from dictionary" << endl;
		cout << "4.Find element by ip in dictionary" << endl;
		cout << "5.Print dictionary" << endl;
		cout << "6.Print passed in time" << endl;
		cout << "7.Quit" << endl;

		int option;
		getInt(&option, LOAD, QUIT);
		if (option == LOAD) {
			char* filename = (char*)malloc(MAX_STRING_SIZE);
			int length = 0;

			printf("Enter file name: ");
			cin.getline(filename, MAX_STRING_SIZE);
			filename = (char*)realloc(filename, strlen(filename) + 1);
			rewind(stdin);
			
			printf("Enter count of strings(1, %d): ", INT_MAX);
			getInt(&length, 1, INT_MAX);

			clock_t start = clock();
			upload_nodes(&name_ip, filename, length);
			printf("Time of upload in my hash-table: %d ms\n", (int)clock() - (int)start);

			start = clock();
			upload_nodes_map(name_ip_cpp, filename, length);
			printf("Time of upload in embedded hash-table: %d ms\n", (int)clock() - (int)start);
		}
		else if (option == ADD) {
			char* name = (char*)malloc(MAX_STRING_SIZE);

			printf("Enter key(name): ");
			cin.getline(name, MAX_STRING_SIZE);
			name = (char*)realloc(name, strlen(name) + 1);
			node* new_node = create_node(name);

			clock_t start = clock();
			add(&name_ip, new_node);
			printf("Time of adding in my hash-table: %d ms\n", (int)clock() - (int)start);

			start = clock();
			name_ip_cpp[name] = new_node->ip;
			printf("Time of adding in embedded hash-table: %d ms\n", (int)clock() - (int)start);
		}
		else if (option == REMOVE) {
			char* value = (char*)malloc(MAX_STRING_SIZE);
			printf("Enter value(IP): ");
			cin.getline(value, MAX_STRING_SIZE);
			value = (char*)realloc(value, strlen(value) + 1);

			clock_t start = clock();
			remove_node(&name_ip, value);
			printf("Time of removing from my hash-table: %d ms\n", (int)clock() - (int)start);

			start = clock();
			for (map<string, string>::iterator it = name_ip_cpp.begin(); it != name_ip_cpp.end(); it++)
				if (strcmp(it->second.c_str(), value) == 0) {
					name_ip_cpp.erase(it);
					break;
				}
			printf("Time of removing from embedded hash-table: %d ms\n", (int)clock() - (int)start);
		}
		else if (option == FIND) {
			char* key = (char*)malloc(MAX_STRING_SIZE);
			printf("Enter key(name): ");
			cin.getline(key, MAX_STRING_SIZE);
			key = (char*)realloc(key, strlen(key) + 1);

			clock_t start = clock();
			char* ip = find_node(&name_ip, key);
			if (ip)
				puts(ip);
			else
				puts("Value with this key didn\'t find");
			printf("Time of finding in my hash-table: %d ms\n", (int)clock() - (int)start);

			start = clock();
			map<string, string>::iterator it = name_ip_cpp.find(key);
			if(it != name_ip_cpp.end())
				printf("%s\n", name_ip_cpp.find(key)->second.c_str());
			else
				puts("Value with this key didn\'t find");
			printf("Time of finding in embedded hash-table: %d ms\n", (int)clock() - (int)start);
		}
		else if (option == PRINT) {
			print(name_ip);
			print_map(name_ip_cpp);
		}
		else if (option == EXCEL) {
			passed_on_time("tables/250502.csv");
			passed_on_time("tables/250503.csv");
		}
		else if (option == QUIT) {
			puts("Would you like to save your results?(y/n)");
			char anwser = ' ';
			while(anwser != 'y' && anwser != 'n')
				anwser = getchar();
			rewind(stdin);
			if (anwser == 'y') {
				char* filename = (char*)malloc(256);
				puts("Enter file name: ");
				cin.getline(filename, MAX_STRING_SIZE);
				filename = (char*)realloc(filename, strlen(filename) + 1);
				save(name_ip, filename);
			}
			exit(0);
		}
		_fcloseall();
	}
}