#pragma once

typedef struct Node {
	char* key;
	char* ip;
	struct Node* next;
} node;

typedef struct Dictionary {
	node elements[MAX_DICT_SIZE];
} dict;

node* create_node(const char* key);
char* create_ip();
dict* create_dict();

void upload_nodes(dict** dictionary, const char* filename, int count);
void upload_nodes_map(map<string, string>& dictionary, const char* filename, int count);

void add(dict** dictionary, node* new_node);

void remove_node(dict** dictionary, const char* key);

char* find_node(dict** dictionary, const char* value);

void print(dict* dictionary);
void print_map(map<string, string> dictionary);

void save(dict* dictionary, const char* filename);