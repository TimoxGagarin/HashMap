#include "includes.hpp"

/// <summary>
/// Constructor of struct "node"
/// </summary>
/// <returns>Pointer on the struct object</returns>
node* create_node(const char* key) {
	srand((int)time(NULL));
	node* new_node = (node*)malloc(sizeof(node));

	char* ip_str = create_ip();
	new_node->ip = (char*)malloc(strlen(ip_str) + 1);
	if (new_node->ip)
		strncpy_s(new_node->ip, strlen(ip_str) + 1, ip_str, strlen(ip_str) + 1);

	new_node->key = (char*)malloc(strlen(key) + 1);
	if (new_node->key) {
		strcpy_s(new_node->key, strlen(key) + 1, key);
		new_node->key[strlen(key)] = '\0';
	}
	new_node->next = NULL;
	return new_node;
}

/// <summary>
/// Constructor of struct "node"
/// </summary>
/// <returns>Pointer on the struct object</returns>
node* create_node_with_ip(const char* key, const char* ip) {
	srand((int)time(NULL));
	node* new_node = (node*)malloc(sizeof(node));

	new_node->ip = (char*)malloc(strlen(ip) + 1);
	if (new_node->ip)
		strncpy_s(new_node->ip, strlen(ip) + 1, ip, strlen(ip) + 1);

	new_node->key = (char*)malloc(strlen(key) + 1);
	if (new_node->key)
		strncpy_s(new_node->key, strlen(key) + 1, key, strlen(key) + 1);
	new_node->next = NULL;
	return new_node;
}

/// <summary>
/// Constructor of struct "dict"
/// </summary>
/// <returns>pointer on the struct object</returns>
dict* create_dict() {
	dict* res = (dict*)malloc(sizeof(dict));
	for (int i = 0; i < MAX_DICT_SIZE; i++) {
		res->elements[i].ip = NULL;
		res->elements[i].key = NULL;
		res->elements[i].next = NULL;
	}
	return res;
}

/// <summary>
/// Calculate all codes of chars and find remainder on the MAX_DICT_SIZE
/// </summary>
/// <returns>Hash</returns>
int hash_func(const char* key) {
	int num = 0;
	for (int i = 0; key[i] != '\0'; i++)
		num += key[i];
	return num % MAX_DICT_SIZE;
}

/// <summary>
/// Save realloc
/// </summary>
/// <param name="ptr">start pointer</param>
/// <param name="len">count of bytes</param>
void realloc_s(char** ptr, int len) {
	if (!(*ptr))
		*ptr = (char*)malloc(len);
	else {
		char* tmp = (char*)realloc(*ptr, len);
		if (tmp)
			*ptr = tmp;
	}
}

/// <summary>
/// Find info in file between borders 
/// </summary>
/// <param name="file">pointer on the file</param>
/// <returns>string between borders</returns>
char* str_in_borders(FILE* file, const char borders[2]) {
	char* res = (char*)malloc(256);
	char temp;
	int len = 0;
	while (true) {
		char temp = fgetc(file);
		if (feof(file)) {
			free(res);
			return NULL;
		}
		else if (temp == borders[0])
			break;
	}
	while (true) {
		temp = fgetc(file);
		if (feof(file)) {
			free(res);
			return NULL;
		}
		else if (temp == borders[1])
			break;
		realloc_s(&res, ++len + 1);
		res[len - 1] = temp;
	}
	res[len] = '\0';
	return res;
}

/// <summary>
/// Add node to the dictionary
/// </summary>
/// <param name="dictionary">pointer on dictionary struct</param>
/// <param name="new_node">pointer on node struct</param>
void add(dict** dictionary, node* new_node) {
	int hash = hash_func(new_node->key);
	node* curr = (*dictionary)->elements + hash;
	node* prev = NULL;
	if(curr->key)
		while (curr) {
			if (strcmp(curr->key, new_node->key) == 0) {
				strncpy_s(curr->ip, strlen(new_node->ip) + 1, new_node->ip, strlen(new_node->ip) + 1);
				return;
			}
			prev = curr;
			curr = curr->next;
		}
	if (!curr)
		prev->next = new_node;
	else if(curr == ((*dictionary)->elements + hash))
		(*dictionary)->elements[hash] = *new_node;
}

/// <summary>
/// Load nodes from .txt file to dictionary
/// </summary>
/// <param name="dictionary">pointer on dictionary struct</param>
/// <param name="count">count of nodes</param>
void upload_nodes(dict** dictionary, const char* filename, int count) {
	_fcloseall();
	FILE* file;
	if (fopen_s(&file, filename, "r")) {
		printf("File \"%s\" didn't found\n", filename);
		return;
	}
	for (int i = 0; i < count; i++) {
		char* key = str_in_borders(file, "()");
		char* value = str_in_borders(file, "[]");
		if(key && value)
			add(dictionary, create_node_with_ip(key, value));
		else {
			printf("Only %d nodes in the file \"%s\" were found\n", i, filename);
			break;
		}
	}
	fclose(file);
}

void upload_nodes_map(map<string, string>& dictionary, const char* filename, int count) {
	_fcloseall();
	FILE* file;
	if (fopen_s(&file, filename, "r")) {
		printf("File \"%s\" didn't found\n", filename);
		return;
	}
	for (int i = 0; i < count; i++) {
		char* key = str_in_borders(file, "()");
		char* value = str_in_borders(file, "[]");
		if (key && value)
			dictionary.insert({ key, value });
		else {
			printf("Only %d nodes in the file \"%s\" were found\n", i, filename);
			break;
		}
	}
	fclose(file);
}

/// <summary>
/// Remove node from a dictionary with value
/// </summary>
/// <param name="dictionary">pointer on dictionary struct</param>
void remove_node(dict** dictionary, const char* value) {
	for (int i = 0; i < MAX_DICT_SIZE; i++) {
		node* curr = (*dictionary)->elements + i;
		node* prev = NULL;

		if (curr->ip)
			while (curr) {
				if (strcmp(curr->ip, value) == 0) {
					if (prev)
						prev->next = curr->next;
					else if(curr->next)
						*curr = *(curr->next);
					else {
						curr->key = NULL;
						curr->ip = NULL;
					}
					return;
				}
				prev = curr;
				curr = curr->next;
			}
	}
	printf("Node with value \"%s\" didn\'t found\n", value);
}

/// <summary>
/// Find node in dictionary with key
/// </summary>
/// <param name="dictionary">pointer on dictionary struct</param>
/// <returns>value - IP-adress</returns>
char* find_node(dict** dictionary, const char* key) {
	int hash = hash_func(key);
	node* curr = (*dictionary)->elements + hash;
	while (curr) {
		if (curr->key && strcmp(curr->key, key) == 0)
 			return curr->ip;
		curr = curr->next;
	}
	return NULL;
}

/// <summary>
/// Print dictionary in the cmd
/// </summary>
/// <param name="dictionary">pointer on dictionary struct</param>
void print(dict* dictionary) {
	printf("%7cKey%23cValue\n", ' ', ' ');
	for (int i = 0; i < MAX_DICT_SIZE; i++) {
		if (dictionary->elements[i].key)
			printf("%16s\t%16s\n", dictionary->elements[i].key, dictionary->elements[i].ip);
		node* curr = dictionary->elements[i].next;
		while (curr) {
			printf("%16s\t%16s\n", curr->key, curr->ip);
			curr = curr->next;
		}
	}
}

void print_map(map<string, string> dictionary) {
	printf("%7cKey%23cValue\n", ' ', ' ');
	for (pair<string, string> x : dictionary)
		printf("%16s\t%16s\n", x.first.c_str(), x.second.c_str());
}

/// <summary>
/// Count length of the number
/// </summary>
/// <param name="num">number</param>
/// <returns>length of number</returns>
int num_len(int num) {
	int num_len = 0;
	int temp = num;
	for (; temp > 0; num_len++)
		temp /= 10;
	return num_len;
}

/// <summary>
/// Generate random IP-adress in the format ipv4
/// </summary>
/// <returns>string of IP-adress</returns>
char* create_ip() {
	srand((int)time(NULL));
	char* res = (char*)malloc(1);
	res[0] = '\0';
	char* str_num = NULL;

	int pos = 0;
	for (int i = 0; i < 4; i++) {
		int num = (rand() + RAND_MAX) % 256;
		if (!str_num)
			str_num = (char*)malloc(num_len(num));
		_itoa_s(num, str_num, num_len(num) + 1, 10);
		pos += ((int)strlen(str_num) + 1);
		realloc_s(&res, pos + 1);
		strcat_s(res, pos + 1, str_num);
		res[pos - 1] = (i != 3) ? '.' : ':';
		res[pos] = '\0';
	}
	unsigned short int port = rand() + RAND_MAX;
	_itoa_s(port, str_num, num_len(port)+1, 10);
	realloc_s(&res, (int)strlen(res) + (int)strlen(str_num));
	pos += (int)strlen(str_num);
	strcat_s(res, pos + 1, str_num);
	res[pos - 1] = '\0';
	return res;
}

/// <summary>
/// Save dictionary into file
/// </summary>
/// <param name="dictionary">pointer on dictionary struct</param>
/// <param name="filename">file name</param>
void save(dict* dictionary, const char* filename) {
	_fcloseall();
	FILE* file;
	if (fopen_s(&file, filename, "w")) {
		printf("File \"%s\" didn't found\n", filename);
		return;
	}
	for (int i = 0; i < MAX_DICT_SIZE; i++) {
		node* curr = dictionary->elements + i;
		while (curr && curr->key && strlen(curr->key) != 0) {
			fprintf(file, "(%s)[%s]\n", curr->key, curr->ip);
			curr = curr->next;
		}
	}
	fclose(file);
}