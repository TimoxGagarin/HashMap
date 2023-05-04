#include "includes.hpp"

void getInt(int* S, int min, int max) {
	int res = 1;
	do {
		int code = scanf_s("%d", S);
		res = code;
		char temp;
		while ((temp = getchar()) != '\n') {
			if ((temp > '9') || (temp < '0')) {
				res = -2;
				break;
			}
		}
		rewind(stdin);
		if (*S < min || *S > max)
			res = -3;
	} while (res != 1);
}
