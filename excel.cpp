#include "includes.hpp"

typedef struct date {
	int mm;
	int dd;
} Date;

int pos_of(const char* str, char c, int i) {
	int j = 0;
	for (int pos = 0; str[pos] != '\0'; pos++) {
		if (str[pos] == c)
			j++;
		if (i == j)
			return pos;
	}
}

int comp_dates(Date& date1, Date& date2) {
	if (date1.mm < date2.mm || (date1.mm == date2.mm && date1.dd < date2.dd))
		return -1;
	else if (date1.mm > date2.mm || (date1.mm == date2.mm && date1.dd > date2.dd))
		return 1;
	return 0;
}

void passed_on_time(const char* filename) {
	time_t rawtime;
	time(&rawtime);
	struct tm* now = (struct tm*)malloc(sizeof(struct tm));
	localtime_s(now, &rawtime);
	Date today = {0, 0};
	char* str_now = (char*)malloc(6);
	strftime(str_now, 6, "%d.%m", now);
	sscanf_s(str_now, "%d.%d", &today.dd, &today.mm);

	_fcloseall();
	FILE* file;
	if (fopen_s(&file, filename, "r")) {
		printf("File \"%s\" didn't found\n", filename);
		return;
	}
	Date* deadlines = (Date*)malloc(8 * sizeof(Date));

	time_t a;
	int row = 0;
	while (!feof(file)) {
		char* line = (char*)malloc(280);
		fgets(line, 280, file);
		if (row == 2) {
			line = line + 5;
			for (int j = 0; j < 8; j++) {
				sscanf_s(line, "Дедлайн: %d.%d;;;", &deadlines[j].dd, &deadlines[j].mm);
				line += 17;
			}
		}
		else if(row > 3 && line[0] >= '0' && line[0] <= '9'){
			Date* passing_dates = (Date*)malloc(8 * sizeof(Date));
			//take name
			line += (pos_of(line, ';', 1) + 1);
			char* name = (char*)malloc(256);
			int i = 0;
			for (; i < pos_of(line, ';', 1); i++)
				name[i] = line[i];
			name[i] = '\0';
			line += (i+3);
			name = (char*)realloc(name, strlen(name)+1);

			bool is_good = true;
			for (int i = 0; i < 8; i++) {
				line += (pos_of(line, ';', 2) + 1);
				passing_dates[i] = today;
				sscanf_s(line, "%d.%d", &passing_dates[i].dd, &passing_dates[i].mm);
				line += 7;
				if (comp_dates(passing_dates[i], deadlines[i]) == 1) {
					is_good = false;
					break;
				}
			}
			if (is_good)
				cout << name << " pass labs in time" << endl;
		}
		else if(row > 3)
			return;
		row++;
	}
	fclose(file);
}